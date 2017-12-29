#ifndef ____WEBSOCKETSERVER_____
#define ____WEBSOCKETSERVER_____
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>


class WebsocketServer
{
public:
  WebsocketServer(const uint& port);
  void setLogging();
  void init();
  websocketpp::server<websocketpp::config::asio>& getServerToSetCallback();
  bool start();

private:
  uint _port;
  websocketpp::server<websocketpp::config::asio>  _server;

};

#endif
