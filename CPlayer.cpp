//Marcado

#include "CPlayer.h"
#include <list>

#include "debug.h"

CPlayer::CPlayer() : canFire(true){
	Type = ENTITY_TYPE_PLAYER;
}

bool CPlayer::OnLoad(char* File, const int Width, const int Height, const int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }
	
    return true;
}

void CPlayer::OnLoop() {
    CEntity::OnLoop();
}

void CPlayer::OnRender(SDL_Surface* Surf_Display) {
    CEntity::OnRender(Surf_Display);
}

void CPlayer::OnCleanup() {
    CEntity::OnCleanup();
}

void CPlayer::OnAnimate() {
	//override da OnAnimate da CEntity
    if(SpeedX != 0) {
		Anim_Control.MaxFrames = this->MaxFrames;
    }else{
        Anim_Control.MaxFrames = 0;
    }

    Anim_Control.OnAnimate();
		
}

bool CPlayer::OnCollision(CEntity* Entity) {
    return true;
}

void CPlayer::Fire() {	
	if(canFire) {
		std::cout << "entity.size() " << CEntity::EntityList.size() << std::endl;			
		CEntity::EntityList.push_back(std::tr1::shared_ptr<CEntity>(new Bullet(X + 100, Y + 25)));	
		canFire = false;
	}
		
}

bool CPlayer::checkIfDead() {
	return isDead;
}