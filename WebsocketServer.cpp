#include "WebsocketServer.h"
#include <iostream>

WebsocketServer::WebsocketServer(const uint& port):
_port(port)
{}

void WebsocketServer::setLogging()
{
  // Set logging settings
  _server.set_access_channels(websocketpp::log::alevel::all);
  _server.clear_access_channels(websocketpp::log::alevel::frame_payload);
}

void WebsocketServer::init()
{
  // Initialize Asio
  _server.init_asio();
}

websocketpp::server<websocketpp::config::asio>& WebsocketServer::getServerToSetCallback()
{
  return _server;
}

bool WebsocketServer::start()
{
  bool retVal = true;
  try{
    // Listen on port 9002
    _server.listen(_port);

    std::cout << "start accept" << std::endl;
    // Start the server accept loop
    _server.start_accept();
    // Start the ASIO io_service run loop
    _server.run();
  }
  catch (websocketpp::exception const & e)
  {
      std::cout << e.what() << std::endl;
      retVal = false;
  }
  catch (...)
   {
      std::cout << "other exception" << std::endl;
      retVal = false;
  }
  return retVal;
}
