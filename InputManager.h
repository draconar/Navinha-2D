#ifndef _INPUT_MANAGER_
#define _INPUT_MANAGER_

#include <OIS.h>
#include <list>
#include <map>
#include "2DRenderManager.h"
#include "EntityFactory.h"
#include "2DSceneManager.h"
#include "constants.h"

/**
	Input Listener class to be used as a callback for InputManager
	- Abstract base class
	- Handles input events
***/

class InputListener {
public:
	EntityFactory* entity;
	//KBD events
	virtual bool keyPressed( const OIS::KeyEvent &e) {return true;}
	virtual bool keyReleased( const OIS::KeyEvent &e) {return true;}
	//Mouse events
	virtual bool mouseMoved(const OIS::MouseEvent &e) {return true;}
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {return true;}
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {return true;}
	//TODO: JOYSTICK movs. pg. 233
};

/**
	The input manager is responsible both for reading input from devices and for notifying
	a game when input occurs. 
	- The hardware <-> game notification occurs by means of a list of instances of the callback
	classes InputListener, whose method it calls appropriately when input is received.
*/
class InputManager : public OIS::KeyListener, public OIS::MouseListener /*TODO: quando fizer joystick, continuar daqui, public OIS::JoyStickListener*/ { 
public:
	OIS::InputManager* oisInputManager;
	OIS::Keyboard* keyboard;
	OIS::Mouse* mouse;
	
	std::list<InputListener*> inputListeners;

	InputManager();

	//mngr mngement =)
	void init();
	void clear();
	void update();
	void addListener(InputListener* listener);

	//input events
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	

	//mouse events
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID is);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID is);
};

class TestInputListener : public InputListener {

public:	
	bool isMoving;
	OIS::KeyCode lastKey;

	TestInputListener(EntityFactory* player) {
		this->entity = player;
		CCamera::CameraControl.TargetMode = TARGET_MODE_NORMAL;
		//CCamera::CameraControl.SetTarget(&player->spriteObj->posX, &player->spriteObj->posY);
	}
	bool keyPressed(const OIS::KeyEvent &e) {

		isMoving = true;

		if (e.key == 205) {
			entity->move(isMoving, RIGHT);	
		} else if (e.key == 203) {
			entity->move(isMoving, LEFT);		
		} else if (e.key == 200) {
			if (entity->canJump) {				
				entity->move(isMoving, UP);				
				entity->canJump = true;
			}
			
		}

		lastKey = e.key;

		if (e.key == 28 ) {
			c2DSceneManager* g_sceneManager = c2DSceneManager::get2DSceneManager();
			g_sceneManager->swap(2);
		}


		std::cout << " KeyPressed {" << e.key << ", " << 
			", " << ((OIS::Keyboard*)(e.device))->getAsString(e.key) <<			
			"} || Character (" << 
			(char)e.text << ")" << std::endl;

		return true;
	}

	bool keyReleased(const OIS::KeyEvent &e) {
		isMoving = false;
		if (e.key == lastKey) {
			entity->stop(isMoving);
		}
		std::cout << "key released " << std::endl;
		return true;
	}

};

class MouseInputListener : public InputListener {
private:
	bool isMoving;

public:
	
	MouseInputListener(EntityFactory* meteor) {
		entity = meteor;
	}

	bool mouseMoved( const OIS::MouseEvent &e ) {		
		isMoving = true;
		
		const OIS::MouseState& s = e.state;		
		s.width = screenW;
		s.height = screenH;
		/*
		std::cout << entity->spriteObj->resourceUID << "\nMouseMoved: Abs("
				  << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel("
				  << s.X.rel << ", " << s.Y.rel << ", " << s.Z.rel << ")\n"
				  << "width: " << s.width << " height: " << s.height << std::endl; 
		*/
		entity->mouseMove(isMoving, s.X.abs, s.Y.abs);

		return true;
	}
};

#endif
