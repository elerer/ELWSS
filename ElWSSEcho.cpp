#include "ElWSSEcho.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

ElWSSEcho::ElWSSEcho(websocketpp::server<websocketpp::config::asio>&  server):
_server(server)
{}

void ElWSSEcho::on_message(websocketpp::connection_hdl hdl, message_ptr msg)
{
  try
  {
    std::string echo(msg->get_payload());
    _server.send(hdl, echo.c_str(), echo.size(), websocketpp::frame::opcode::text);  
  }
  catch (const websocketpp::lib::error_code& e)
  {
    std::cout << "Echo failed because: " << e
              << "(" << e.message() << ")" << std::endl;
  }

}

void ElWSSEcho::setCallbacks()
{
  _server.set_message_handler(bind(&ElWSSEcho::on_message,this,::_1,::_2));
}
