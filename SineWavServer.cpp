#include "WebsocketServer.h"
#include "ElWSSine.h"

int main()
{
  WebsocketServer wss(8082);
  wss.setLogging();
  wss.init();

  ElWSSine sineWav(wss.getServerToSetCallback(),"/home/eranl/WAV_FILES/cello.wav",440);
  sineWav.setCallbacks();

  wss.start();

}
