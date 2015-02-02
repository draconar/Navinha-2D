//Marcado

#ifndef _ENEMY_H
#define _ENEMY_H

#include <iostream>
#include "CEntity.h"

#include "debug.h"

class Enemy : public CEntity {

public:

	void OnMove(float MoveX, float MoveY);

	void Fire();

protected:
	bool canFire;
};

#endif