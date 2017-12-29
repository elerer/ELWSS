#include "WebsocketServer.h"
#include "ElWSSEcho.h"

int main()
{
  WebsocketServer wss(8082);
  wss.setLogging();
  wss.init();

  ElWSSEcho echo(wss.getServerToSetCallback());
  echo.setCallbacks();

  wss.start();

}
