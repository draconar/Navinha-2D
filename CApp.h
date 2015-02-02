//Marcado

#ifndef _CAPP_H_
    #define _CAPP_H_

#include <SDL.h>
#include "CSurface.h"
#include "CEvent.h"
#include "CAnimation.h"
#include "CEntity.h"
#include "Define.h"
#include "CArea.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "Enemy.h"

class CApp : public CEvent {

private:

	bool running;
	SDL_Surface*    Surf_Display;
	SDL_Surface*    Surf_Grid;
	SDL_Surface*    Surf_X;
    SDL_Surface*    Surf_O;
	CEntity         Entity1;
	CEntity         Entity2;
	CEntity         Entity3;
	CEntity         Entity4;
	CPlayer Player;
	CPlayer Player2;
	Enemy enemy;
 

	int Grid[9];
	int CurrentPlayer;

	enum {
    GRID_TYPE_NONE = 0,
    GRID_TYPE_X,
    GRID_TYPE_O
	};

public:

        CApp();

        int OnExecute();

		bool OnInit();

        void OnEvent(SDL_Event* Event);

		void OnExit();

		void OnLButtonDown(int mX, int mY);

        void OnLoop();

        void OnRender();

        void OnCleanup();

		void Reset();

		void SetCell(int ID, int Type);

		void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

		void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

};

#endif