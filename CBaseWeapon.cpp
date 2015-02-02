//Marcado

#include "CBaseWeapon.h"
#include <iostream>

#include "debug.h"

CBaseWeapon::CBaseWeapon() {
}

bool CBaseWeapon::OnLoad(char* File, const int Width, const int Height, const int MaxFrames) {

	if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }	
    return true;
}

void CBaseWeapon::OnLoop() {
    CEntity::OnLoop();
}

void CBaseWeapon::OnRender(SDL_Surface* Surf_Display) {
    CEntity::OnRender(Surf_Display);
}

void CBaseWeapon::OnCleanup() {
    CEntity::OnCleanup();
}

void CBaseWeapon::OnAnimate() {
	//override da OnAnimate da CEntity
    if(SpeedX != 0) {
		Anim_Control.MaxFrames = this->MaxFrames;
    }else{
        Anim_Control.MaxFrames = 0;
    }

    Anim_Control.OnAnimate();
}

bool CBaseWeapon::OnCollision(CEntity* Entity) {
	return true;
}