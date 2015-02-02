//Marcado

#ifndef _BULLET_H_
#define _BULLET_H_
#include "CBaseWeapon.h"

class Bullet :  public CBaseWeapon
{
public:
	Bullet();
	Bullet(int playerX, int playerY);
	virtual ~Bullet();
	int Fire();	
	void OnMove(float MoveX, float MoveY);
	Bullet* next;

	bool checkIfDead();
	bool isDead;
	
	void OnRender(SDL_Surface* Surf_Display);
	int IsReady;
	int IsCollide;	
	int Speed;
	int ID;
	int initXpos;
	int initYpos;

};

#endif