#ifndef ___IElWebsocketServerCallbakSetter___
#define  ___IElWebsocketServerCallbakSetter___

class IElWebsocketServerCallbakSetter
{
public:
  virtual ~IElWebsocketServerCallbakSetter(){};
  virtual void setCallbacks() = 0;
};

#endif
