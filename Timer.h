#ifndef _TIMER_
#define	_TIMER_	

#include <SDL.h>
#include <windows.h>

/**
Timer class: fully defined here
- measures the elapsed time and initiate events when necessary
- works together with sceneListener
**/

class Timer {
public:
	unsigned int ID;
	DWORD startTime;
	DWORD interval;
	bool expired;

	Timer(){
		ID = 0;
		startTime = 0;
		interval = 0;
		expired = false;
	}

	void start() {
		startTime = SDL_GetTicks();
		expired = false;
	}

	void update() {
		if(expired)
			return;

		DWORD elapsedTime = SDL_GetTicks() - startTime;

		if(elapsedTime >= interval)
			expired = true;
	}
};
#endif