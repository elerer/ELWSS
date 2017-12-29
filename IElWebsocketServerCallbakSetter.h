#ifndef ___IElWebsocketServerCallbakSetter___
#define  ___IElWebsocketServerCallbakSetter___
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>


class IElWebsocketServerCallbakSetter
{
public:
  virtual ~IElWebsocketServerCallbakSetter(){};
  virtual void setCallbacks() = 0;
};

#endif
