//Marcado

#include "CFPS.h"

#include "debug.h"

CFPS CFPS::FPSControl;

CFPS::CFPS() {
    OldTime     = 0;
    LastTime    = 0;

    SpeedFactor = 0;
	
    Frames      = 0;
    NumFrames   = 0;
}

void CFPS::OnLoop() {
	//a cada 1s, recalculo os FPS (NUMFRAMES)
    if(OldTime + 1000 < SDL_GetTicks()) {
		//o instante em que passou de 1s, guardo valor para esperar + 1s
        OldTime = SDL_GetTicks();

        NumFrames = Frames;

        Frames = 0;
    }

    SpeedFactor = ((SDL_GetTicks() - LastTime) / 1000.0f) * 32.0f;

    LastTime = SDL_GetTicks();

    Frames++;
}

int CFPS::GetFPS() {
    return NumFrames;
}

float CFPS::GetSpeedFactor() {
    return SpeedFactor;
}
 