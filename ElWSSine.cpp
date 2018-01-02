#include "ElWSSine.h"
#include <functional>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std::placeholders;

std::mutex mu;
std::condition_variable cv;

ElWSSine::ElWSSine(uWS::Hub& h,const std::string& wavFile,const size_t& freq):
_server(h),
_fs(wavFile.c_str(),std::ifstream::binary),
_frequency(freq)
{
  //Prepare header template
  _fs.read(_buffer,1044);
  _fs.close();
  bool mod = false;
  //Set header and remove unwanted segments
  _wHeader.NormalizeWavHeaderOfBuffer(_buffer,1000,_origHeaderLength,mod);
  //set template header
  _wHeader.setSizeFieldsRelatedToDataLength(_samplesPerSend);

  std::cout << "Samplerate " << _wHeader._SampleRate << '\n';
  std::cout << "_NumChannels " << _wHeader._NumChannels << '\n';
  std::cout << "_BitsPerSample " << _wHeader._BitsPerSample << '\n';

  _millisecPerSend = ((float)_samplesPerSend/(_wHeader._SampleRate*_wHeader._NumChannels))*1000;

  std::cout << "_millisecPerSend " << _millisecPerSend << '\n';

  _bytesPerSample = _wHeader._BitsPerSample/8;
  std::cout << "bytes per sample " << _bytesPerSample << '\n';
  _normalizeSinSample = pow(2,_wHeader._BitsPerSample);
  caculateStep();
  //Set wavHeader as the prefix for all sends
  memcpy(_buffer,(char*)&_wHeader,44);


  _th = std::move(std::thread(&ElWSSine::sendToSocket,this));

}

ElWSSine::~ElWSSine()
{
  _th.join();
}

//Caculates the step interval for generating frequency at a given SR
void ElWSSine::caculateStep()
{
  _step = _frequency*2*_pi/(_wHeader._SampleRate);
  std::cout << "step is " << _step << '\n';
}

void ElWSSine::on_connect(uWS::WebSocket<uWS::SERVER> *ws,uWS::HttpRequest req)
{
  std::unique_lock<std::mutex> lock(mu);
  std::cout << "got ws socket" << '\n';
  _ws = ws;
  cv.notify_one();
}

void ElWSSine::on_message(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode)
{
  std::cout << "got msg" << '\n';
}

void ElWSSine::sendToSocket()
{

    double dSample = 0;
    float flSample = 0;
    short sixteenBitSample = 0;
    char  eightBitSample = 0;
    int32_t twentyfourBitSample = 0;
    char* source = nullptr;
    size_t iterations = _samplesPerSend/(_bytesPerSample*_wHeader._NumChannels);
    size_t totalBytesToSend = iterations*(_bytesPerSample*_wHeader._NumChannels);

    bool loop = true;

    {
          std::unique_lock<std::mutex> lock(mu);
          while(!_ws)
          {
            cv.wait(lock);
          }

    }

    while(loop)
    {
      size_t bytes = 0;

      for (size_t i = 0; i < iterations; i++)
       {
          generateOneSample(dSample);
          if (_bytesPerSample == 1)
          {
            eightBitSample = (char)dSample*_normalizeSinSample;
            source = (char*)&eightBitSample;
          }
          else if(_bytesPerSample == 2)
          {
            sixteenBitSample = (short)(dSample*_normalizeSinSample);
            source = (char*)&sixteenBitSample;
          }
          else if(_bytesPerSample == 3)
          {
            twentyfourBitSample = (int32_t)dSample*_normalizeSinSample;
            source = (char*)&twentyfourBitSample;
          }
          else if(_bytesPerSample == 4)
          {
            flSample = (float)dSample;
            source = (char*)&flSample;
          }

          for (size_t index = 0; index < _wHeader._NumChannels; index++)
          {
            memcpy((_buffer + 44 + bytes),source,_bytesPerSample);
            bytes += _bytesPerSample;
          }

      }

      size_t size =  44 + totalBytesToSend;
      _ws->send(_buffer,size , uWS::OpCode::BINARY);
    //  std::cout << "send to ws" << '\n';
      std::this_thread::sleep_for(std::chrono::milliseconds(_millisecPerSend/10));

      //loop = false

    }

}



void ElWSSine::generateOneSample(double& val)
{
  val = sin(_step*(_pos));
  _pos++;
  _pos %= _wHeader._SampleRate;
}

void ElWSSine::setCallbacks()
{
  _server.onMessage(std::bind(&ElWSSine::on_message,this,::_1,::_2,::_3,::_4));
  _server.onConnection(std::bind(&ElWSSine::on_connect,this,::_1,::_2));
}
