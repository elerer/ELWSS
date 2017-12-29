#include "ElWSSine.h"
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

ElWSSine::ElWSSine(websocketpp::server<websocketpp::config::asio>&  server,const std::string& wavFile,const size_t& freq):
_server(server),
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
  _wHeader.setSizeFieldsRelatedToDataLength(1000);

  std::cout << "Samplerate " << _wHeader._SampleRate << '\n';
  std::cout << "_NumChannels " << _wHeader._NumChannels << '\n';
  std::cout << "_BitsPerSample " << _wHeader._BitsPerSample << '\n';

  _bytesPerSample = _wHeader._BitsPerSample/8;
  std::cout << "bytes per sample " << _bytesPerSample << '\n';
  _normalizeSinSample = pow(2,_wHeader._BitsPerSample);
  caculateStep();
  //Set wavHeader as the prefix for all sends
  memcpy(_buffer,(char*)&_wHeader,44);

  std::thread th(&ElWSSine::streamToConnection,this);
  _th = std::move(th);

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


void ElWSSine::streamToConnection()
{
  try
  {
    double dSample = 0;
    float flSample = 0;
    short sixteenBitSample = 0;
    char  eightBitSample = 0;
    int32_t twentyfourBitSample = 0;
    char* source = nullptr;
    size_t iterations = 1000/(_bytesPerSample*_wHeader._NumChannels);
    size_t totalBytesToSend = iterations*(_bytesPerSample*_wHeader._NumChannels);

    std::unique_lock<std::mutex> lock(_lock);

    while(_connectios.empty())
    {
        _cond.wait(lock);
    }

    auto it_hdl = _connectios.begin();

    while(true)
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
      _server.send(*it_hdl, _buffer,size , websocketpp::frame::opcode::binary);
      std::cout << "send to ws" << '\n';
    }
  }

  catch (const websocketpp::lib::error_code& e)
  {
    std::cout << "send failed because: " << e
              << "(" << e.message() << ")" << std::endl;
  }

}

void ElWSSine::on_open(websocketpp::connection_hdl hdl)
{
    {
        std::cout << "on_open" << '\n';
        std::lock_guard<std::mutex> guard(_lock);
        if (_connectios.empty())
         {
           _connectios.insert(hdl);
           _cond.notify_one();
         }
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
  _server.set_open_handler(bind(&ElWSSine::on_open,this,::_1));
}
