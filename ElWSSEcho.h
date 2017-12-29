#ifndef __ELWSSECHO___
#define __ELWSSECHO___

#include "IElWebsocketServerCallbakSetter.h"

typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;

class ElWSSEcho : public IElWebsocketServerCallbakSetter
{
public:
  ElWSSEcho(websocketpp::server<websocketpp::config::asio>&  server);
  void setCallbacks();
private:
  void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
  websocketpp::server<websocketpp::config::asio>&  _server;
};

#endif
