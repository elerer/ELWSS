#include "WebsocketServer.h"
#include "ElWSStream.h"

int main()
{
  WebsocketServer wss(8082);
  wss.setLogging();
  wss.init();

  ElWSStream streamWav(wss.getServerToSetCallback(),"/home/eranl/WAV_FILES/cello.wav");
  streamWav.setCallbacks();

  wss.start();

}
