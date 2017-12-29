#ifndef __ELWSSINE___
#define __ELWSSINE___

#include "IElWebsocketServerCallbakSetter.h"
#include <WavHeader.h>
#include <fstream>
#include <set>
#include <mutex>

typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;

class ElWSSine : public IElWebsocketServerCallbakSetter
{
public:
  ElWSSine(websocketpp::server<websocketpp::config::asio>&  server,const std::string& wavFile,const size_t& freq);
  ~ElWSSine();
  void setCallbacks();

private:
  void on_open(websocketpp::connection_hdl hdl);
  void streamToConnection();
  websocketpp::server<websocketpp::config::asio>&  _server;
  void caculateStep();
  void generateOneSample(double& val);

//members
  std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > _connectios ;
  std::mutex _lock;
  std::condition_variable _cond;
  std::thread _th;

  char _buffer[1044];
  Wav_header _wHeader;
  std::ifstream _fs;
  size_t _origHeaderLength = 0 ;
  size_t _frequency;
  short _bytesPerSample = 0;
  long _normalizeSinSample = 0;
  double _step = 0;
  double _pi = 3.14159;
  size_t _pos = 0;


};

#endif
