//Marcado

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include <iostream>
#include <list>
#include "CEntity.h"
#include "Bullet.h"

class CPlayer : public CEntity {
private:
		std::list<Bullet*> bullet;
		
public:
	
        CPlayer();
		
		bool canFire;

		bool OnLoad(char* File, const int Width, const int Height, const int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        bool OnCollision(CEntity* Entity);

		void Fire();
		
		bool checkIfDead();
};

#endif