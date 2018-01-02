#ifndef __ELWSSINE___
#define __ELWSSINE___

#include "IElWebsocketServerCallbakSetter.h"
#include <uWS/uWS.h>
#include <WavHeader.h>
#include <fstream>
#include <uWS/HTTPSocket.h>
#include <thread>


class ElWSSine : public IElWebsocketServerCallbakSetter
{
public:
  ElWSSine(uWS::Hub& h,const std::string& wavFile,const size_t& freq);
  ~ElWSSine();
  void setCallbacks();

private:
  void on_message(uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode);
  void on_connect(uWS::WebSocket<uWS::SERVER> *ws,uWS::HttpRequest req);
  void sendToSocket();
  void streamToConnection();
  uWS::Hub&   _server;
  void caculateStep();
  void generateOneSample(double& val);

  size_t _samplesPerSend = 10000;
  char _buffer[40044];
  Wav_header _wHeader;
  std::ifstream _fs;
  size_t _origHeaderLength = 0 ;
  size_t _frequency;
  short _bytesPerSample = 0;
  long _normalizeSinSample = 0;
  double _step = 0;
  double _pi = 3.14159;
  size_t _pos = 0;
  size_t _millisecPerSend = 0;
  std::thread _th;
  uWS::WebSocket<uWS::SERVER> *_ws = nullptr;


};

#endif
