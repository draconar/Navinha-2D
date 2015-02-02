//Marcado

#include "CAnimation.h"

#include "debug.h"

CAnimation::CAnimation() {
    CurrentFrame    = 0;
    MaxFrames       = 0;
    FrameInc        = 1;

    FrameRate       = 100; //Milliseconds
    OldTime         = 0;

    Oscillate       = false;
}

void CAnimation::OnAnimate() {

	//só sai dessa etapa quando GetTicks > oldTime+FrameRate; i.e. espera até 
	//checar o tempo para andar 1 frame
    if( (OldTime + FrameRate) > SDL_GetTicks()) {
        return;
    }

    OldTime = SDL_GetTicks();

    CurrentFrame += FrameInc;

    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= (MaxFrames - 1) ) {
				//FrameInc vai de [-1, 1], i.e. crescendo ou decrescendo
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= MaxFrames - 1) {
            CurrentFrame = 0;
        }
    }
}

void CAnimation::SetFrameRate(int Rate) {
    FrameRate = Rate;
}

void CAnimation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame >= MaxFrames) return;

    CurrentFrame = Frame;
}

int CAnimation::GetCurrentFrame() {
    return CurrentFrame;
}