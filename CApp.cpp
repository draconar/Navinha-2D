//Marcado

#include "CApp.h"
#include <iostream>

#include "debug.h"

CApp::CApp() {
	running = true;
	CurrentPlayer = 0; 
    Surf_Grid = NULL;
    Surf_X = NULL;
    Surf_O = NULL;
    Surf_Display = NULL;
}

int CApp::OnExecute() {
	//chama método OnInit() e compara seu retorno; se retonar false, sai do game.
	 if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;

    while(running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}

bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }

    if(CArea::AreaControl.OnLoad("./1.area") == false) {
    	return false;
    }

    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

    if(Player.OnLoad("./gfx/ship.png", 64, 64, 0) == false) {
    	return false;
    }

    if(Player2.OnLoad("./gfx/yoshi.png", 64, 64, 8) == false) {
    	return false;
    }

	if(enemy.OnLoad("./gfx/enemy1.png", 50, 44, 0) == false) {
		return false;
	}
	
    Player2.X = 500;
	enemy.X = 300;

	CEntity::EntityList.push_back(std::tr1::shared_ptr<CEntity>(&Player) );
    CEntity::EntityList.push_back(std::tr1::shared_ptr<CEntity>(&Player2) );
	CEntity::EntityList.push_back(std::tr1::shared_ptr<CEntity>(&enemy) );

	CCamera::CameraControl.TargetMode = TARGET_MODE_CENTER;
    CCamera::CameraControl.SetTarget(&Player.X, &Player.Y);

    return true;
}

void CApp::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}

void CApp::OnLButtonDown(int mX, int  mY) {
    int ID  = mX / 200;
    ID = ID + ((mY / 200) * 3);

    if(Grid[ID] != GRID_TYPE_NONE) {
        return;
    }

    if(CurrentPlayer == 0) {
        SetCell(ID, GRID_TYPE_X);
        CurrentPlayer = 1;
    }else{
        SetCell(ID, GRID_TYPE_O);
        CurrentPlayer = 0;
    }
}


void CApp::OnLoop() {
    //loop nas entities
	for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnLoop();
    }

	//Collision Events
for(int i = 0;i < CEntityCol::EntityColList.size();i++) {
	//pega 2 entities envolvidas na colisão, e itera
    std::tr1::shared_ptr<CEntity> EntityA = CEntityCol::EntityColList[i].EntityA;
    std::tr1::shared_ptr<CEntity> EntityB = CEntityCol::EntityColList[i].EntityB;

    if(EntityA == NULL || EntityB == NULL) continue;
	//'only entities that are moving check for collision'
	//se chegou até aqui, é pq as entidades se encontram no vetor EntityColList
    if( EntityA->OnCollision(EntityB)) {
        EntityB->OnCollision(EntityA);
    }
}
	
	CEntityCol::EntityColList.clear();

	//remove DEAD entities 
	for(int i = 0;i < CEntity::EntityList.size();i++) {
		if(CEntity::EntityList[i]->checkIfDead()) 
			CEntity::EntityList.erase( CEntity::EntityList.begin() + i );			
    }
	

    CFPS::FPSControl.OnLoop();

    char Buffer[255];
    sprintf(Buffer, "%d", CFPS::FPSControl.GetFPS());
    SDL_WM_SetCaption(Buffer, Buffer);

}

void CApp::OnRender() {
	SDL_Rect Rect;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = WWIDTH;
	Rect.h = WHEIGHT;

	SDL_FillRect(Surf_Display, &Rect, 0);

	CArea::AreaControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnRender(Surf_Display);
    }

	SDL_Flip(Surf_Display);
}

void CApp::OnCleanup() {   	
	
	CArea::AreaControl.OnCleanup();

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnCleanup();
    }

    CEntity::EntityList.clear();

    SDL_FreeSurface(Surf_Display);
    SDL_Quit();
}

void CApp::OnExit() {
    running = false;
}

void CApp::Reset() {
    for(int i = 0;i < 9;i++) {
        Grid[i] = GRID_TYPE_NONE;
    }
}

void CApp::SetCell(int ID, int  Type) {
    if(ID < 0 || ID >= 9) return;
    if(Type < 0 || Type > GRID_TYPE_O) return;

    Grid[ID] = Type;
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_LEFT: {
            Player.MoveLeft = true;
            break;
        }

        case SDLK_RIGHT: {
            Player.MoveRight = true;
            break;

        }
		case SDLK_UP: {
			Player.MoveUp = true;
			break;			  
		}
		case SDLK_DOWN: {
			Player.MoveDown = true;
			break;
		}
		case SDLK_SPACE: {
			Player.Fire();			
			break;
		}
        default: {
        }
    }
}

void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_LEFT: {
            Player.MoveLeft = false;
            break;
        }

        case SDLK_RIGHT: {
            Player.MoveRight = false;
            break;
        }
		case SDLK_UP: {
			Player.MoveUp = false;
			break;			  
		}
		case SDLK_DOWN: {
			Player.MoveDown = false;
			break;
		}
		case SDLK_SPACE: {
			Player.canFire = true;			
			break;
		}
        default: {
        }
    }
}



