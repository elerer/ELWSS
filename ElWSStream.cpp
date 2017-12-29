#include "ElWSStream.h"
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

ElWSStream::ElWSStream(websocketpp::server<websocketpp::config::asio>&  server,const std::string& wavFile):
_server(server),
_fs(wavFile.c_str(),std::ifstream::binary)
{
  _fs.read(_buffer,1044);
  bool mod = false;
  //Set header and remove unwanted segments
  _wHeader.NormalizeWavHeaderOfBuffer(_buffer,1000,_origHeaderLength,mod);
  //Rewind the position to the end of the header
  _fs.seekg(_origHeaderLength);
  //set template header
  _wHeader.setSizeFieldsRelatedToDataLength(1000);

  memcpy(_buffer,(char*)&_wHeader,44);
}


void ElWSStream::on_message(websocketpp::connection_hdl hdl, message_ptr msg)
{
  try
  {
    while(_fs.read(_buffer+44,1000))
    {
      std::cout << "r";

      size_t size =  44 + _fs.gcount();
      _server.send(hdl, _buffer,size , websocketpp::frame::opcode::binary);
    }
    std::cout << "rewinding" << '\n';
    _fs.seekg(_origHeaderLength);

  }
  catch (const websocketpp::lib::error_code& e)
  {
    std::cout << "send failed because: " << e
              << "(" << e.message() << ")" << std::endl;
  }

}

void ElWSStream::setCallbacks()
{
  _server.set_message_handler(bind(&ElWSStream::on_message,this,::_1,::_2));
}
