//Marcado

#include "Bullet.h"
#include <iostream>

#include "debug.h"

Bullet::Bullet() {
	MoveRight = true;
	Flags = ENTITY_FLAG_GHOST;
	Type = ENTITY_TYPE_PLAYER;
	this->OnLoad("./gfx/bullet1.png", 22, 8, 1);	
}

Bullet::~Bullet(){
	std::cout << "mórri" << std::endl;
}

void Bullet::OnMove(float MoveX, float MoveY){
	if ( (X-initXpos)  > 250 ) 
		isDead = true;
	else
		CEntity::OnMove(MoveX, MoveY);
}

Bullet::Bullet(int playerX, int playerY) {
	MoveRight = true;
	isDead = false;
	Flags = ENTITY_FLAG_GHOST;
	Type = ENTITY_TYPE_PLAYER;
	this->OnLoad("./gfx/bullet1.png", 22, 8, 1); 
	this->X = playerX;
	this->Y = playerY;
	this->initXpos = playerX;
	this->initYpos = playerY;
}

bool Bullet::checkIfDead() {
	return isDead;
}

void Bullet::OnRender(SDL_Surface* Surf_Display) {
	if(!isDead)
		CEntity::OnRender(Surf_Display);
}