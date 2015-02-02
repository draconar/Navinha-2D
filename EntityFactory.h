#ifndef _ENTITY_FACTORY_
#define _ENTITY_FACTORY_

#include <iostream>
#include <map>
#include <list>
#include <string>
#include "RenderData.h"
#include "2DSceneManager.h"
#include "Timer.h"

enum {
	NONE = 0,
	LEFT = 0x0000001,
	RIGHT = 0x0000002, 
	UP = 0x0000004,
	DOWN = 0x0000008, 
};

enum {
	STOP_LEFT,
	STOP_RIGHT,
	WALK_LEFT,
	WALK_RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	IDLE_LEFT,
	IDLE_RIGHT,
	CRY_LEFT,
	CRY_RIGHT,
	PUSH_LEFT,
	PUSH_RIGHT,
};



/** every single entity in the game will be instantiated by this class: be
it a Player, Sprite, Enemy, etc **/

class EntityFactory {
	
public:
	int mouseX;
	int mouseY;

	SpriteObject* spriteObj; /**< This variable will be initialized to zero. 
							 Each entity must be linked to a sprite object previously loaded */	

	Timer* timer;

	bool canJump;
	bool isMoving;
	int direction;
	int stateX;
	int stateY;
	int lastStateX;

	bool bindResourceToEntity(int sceneUID, int uid);

		

	/** Factory function, returns an instance of any Entity */
	static EntityFactory* makeEntity(const int choice);


	/** update entity info as a whole, e.g. position, check colision, physics... */
	virtual void update()=0;
	virtual void move(bool& mov, int dir){};
	virtual void mouseMove(bool& mov, int mouseX, int mouseY){};
	virtual void stop(bool& mov)=0;
	virtual ~EntityFactory(){};

};


class Player : public EntityFactory {
public:		
	Player() {
		canJump = true;
		isMoving=false;
		timer = new Timer;
		timer->ID=0;
		timer->interval=3000;
		timer->start();
		direction = NONE;
		stateX = NONE;
		stateY = NONE;
		lastStateX = RIGHT;
	}
	void update() {			

		//if(isMoving && timer->expired) {
			//TODO: DO I NEED THE CODE BELOW?
			/*
				if(direction & LEFT)
					spriteObj->posX--;	
				else if (direction & RIGHT)
					spriteObj->posX++;	
			*/
		//	timer->start();
		//}		

		//Modificação Fábio - Timer para verificar estado idle
		if (spriteObj->rowNumber != STOP_RIGHT && 
			spriteObj->rowNumber != STOP_LEFT &&
			spriteObj->rowNumber != IDLE_RIGHT && 
			spriteObj->rowNumber != IDLE_LEFT)
			timer->start();

		//Modificação Fábio - Estados de animação
		
		//logica animacao pulo
		if ((stateX & RIGHT) && (stateY & UP))
			spriteObj->rowNumber = UP_RIGHT;
		if ((stateX & LEFT) && (stateY & UP))
			spriteObj->rowNumber = UP_LEFT;
		if ((stateX == NONE) && (stateY & UP)) {
			if (lastStateX & LEFT)
				spriteObj->rowNumber = UP_LEFT;
			else if (lastStateX & RIGHT)
				spriteObj->rowNumber = UP_RIGHT;
		}
		
		//logica animacao queda
		if ((stateX & RIGHT) && (stateY & DOWN))
			spriteObj->rowNumber = DOWN_RIGHT;
		if ((stateX & LEFT) && (stateY & DOWN))
			spriteObj->rowNumber = DOWN_LEFT;
		if ((stateX == NONE) && (stateY & DOWN)) {
			if (lastStateX & LEFT)
				spriteObj->rowNumber = DOWN_LEFT;
			else if (lastStateX & RIGHT)
				spriteObj->rowNumber = DOWN_RIGHT;
		}
		
		//logica animacao andar
		if ((stateX & RIGHT) && (stateY == NONE))
			spriteObj->rowNumber = WALK_RIGHT;
		if ((stateX & LEFT) && (stateY == NONE))
			spriteObj->rowNumber = WALK_LEFT;
		
		//logica animacao parado
		if ((stateX == NONE) && (stateY == NONE) && !timer->expired) {
			if (lastStateX & RIGHT)
				spriteObj->rowNumber = STOP_RIGHT;
			else if (lastStateX & LEFT)
				spriteObj->rowNumber = STOP_LEFT;
		}

		if ((stateX == NONE) && (stateY == NONE) && timer->expired) {
			if (lastStateX & RIGHT)
				spriteObj->rowNumber = IDLE_RIGHT;
			else if (lastStateX & LEFT)
				spriteObj->rowNumber = IDLE_LEFT;
		}


		timer->update();
	}
	void move(bool& mov, int dir) {				
		isMoving = mov;
		direction = dir;
	}
	void stop(bool& mov) {
		isMoving = mov;
		direction = NONE;
	}

};

class Meteor : public EntityFactory {
public:	
	Meteor() {
		canJump = true;
		isMoving=false;
		timer = new Timer;
		timer->ID=0;
		timer->interval=3.3;
		timer->start();
		direction = NONE;
		oldMouseX = 0;
		oldMouseY = 0;
	}
	void update() {			

		if(isMoving && timer->expired) {
			//TODO: DO I NEED THE CODE BELOW?
			/*
			if(direction & LEFT)
				spriteObj->posX--;	
			else if (direction & RIGHT)
				spriteObj->posX++;	
			*/
			timer->start();
		}		
		timer->update();
	}
	using EntityFactory::move;
	void mouseMove(bool& mov, int mouseX, int mouseY) {	
		
		if(mouseX != oldMouseX || mouseY != oldMouseY) {
		
		isMoving = mov;
		

		if(mouseX > oldMouseX){
			direction = RIGHT;
		} else if (mouseX < oldMouseX) {
			direction = LEFT;
		}
		
		oldMouseX = mouseX;
		this->mouseX = mouseX;

		if(mouseY > oldMouseY){
			direction = UP;
		} else if (mouseY < oldMouseY) {
			direction = DOWN;
		}
		oldMouseY = mouseY;
		this->mouseY = mouseY;

		} else {
			direction = NULL;
			isMoving = false;
		}
	}
	void stop(bool& mov) {
		isMoving = mov;
		direction = NONE;
	}
private:
	int oldMouseX;
	int oldMouseY;
};

class Apagar : public EntityFactory {
public:	
	Apagar() {
		isMoving=false;
		timer = new Timer;
		timer->ID=0;
		timer->interval=3.3;
		timer->start();
		direction = NONE;
	}
	void update() {			
	}
	void move(bool& mov, int dir) {				
		isMoving = mov;
		direction = dir;
	}
	void stop(bool& mov) {
		isMoving = mov;
		direction = NONE;
	}
};

#endif