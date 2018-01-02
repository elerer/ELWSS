#include <uWS/uWS.h>
#include "ElWSSine.h"

int main()
{
	uWS::Hub h;

	ElWSSine sineWav(h,"/home/eranl/WAV_FILES/cello.wav",440);
	sineWav.setCallbacks();

	if (h.listen(3000))
	{
	        h.run();
	}
}
