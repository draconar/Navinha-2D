//Marcado

#ifndef _CBASEWEAPON_H_
#define _CBASEWEAPON_H_

#include "CEntity.h"

class CBaseWeapon : public CEntity {
    public:
        CBaseWeapon();

        bool OnLoad(char* File, const int Width, const int Height, const int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        bool OnCollision(CEntity* Entity);
};

#endif