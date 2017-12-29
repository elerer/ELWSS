#ifndef __ELWSSTREAM___
#define __ELWSSTREAM___

#include "IElWebsocketServerCallbakSetter.h"
#include <WavHeader.h>
#include <fstream>

typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;

class ElWSStream : public IElWebsocketServerCallbakSetter
{
public:
  ElWSStream(websocketpp::server<websocketpp::config::asio>&  server,const std::string& wavFile);
  void setCallbacks();

private:
  void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
  websocketpp::server<websocketpp::config::asio>&  _server;

//members
  char _buffer[1044];
  Wav_header _wHeader;
  std::ifstream _fs;
  size_t _origHeaderLength = 0 ;
};

#endif
