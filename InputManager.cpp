#include "InputManager.h"

#include "debug.h"

InputManager::InputManager() {
	oisInputManager = NULL;
	keyboard = NULL;
	mouse = NULL;
}

void InputManager::init() {
	try {
		//get 2d render manager
		c2DRenderManager* g_renderManager = c2DRenderManager::get2DRenderManager();

		OIS::ParamList paramList;
		std::ostringstream windowHndStr;
		size_t handle = (size_t) g_renderManager->windowHandle;
		windowHndStr << handle;
		paramList.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));
		//TODO: essa linha é system dependent, fazer para todos systems
		paramList.insert(std::make_pair(std::string("w32_mouse"),std::string("DISCL_FOREGROUND")));
		paramList.insert(std::make_pair(std::string("w32_mouse"),std::string("DISCL_NONEXCLUSIVE")));

		oisInputManager = OIS::InputManager::createInputSystem(paramList);

		//if there is a kbd
		if(oisInputManager->getNumberOfDevices(OIS::OISKeyboard) > 0) {
			keyboard = static_cast<OIS::Keyboard*>(oisInputManager->createInputObject(OIS::OISKeyboard, true));
			keyboard->setEventCallback(this);
		}
		//if there is a mouse
		if(oisInputManager->getNumberOfDevices(OIS::OISMouse) > 0) {
			mouse = static_cast<OIS::Mouse*>(oisInputManager->createInputObject(OIS::OISMouse, true));
			mouse->setEventCallback(this);
		}

		
	} catch(...) {
		;
	}
}

void InputManager::clear() {
	//Confirmar
	if(keyboard)
		oisInputManager->destroyInputObject(keyboard);
	if(mouse)
		oisInputManager->destroyInputObject(mouse);
	//todo: pode haver erros aqui. A função que faz o destroy está chamando a si mesma e destruindo corretamente?
	if(oisInputManager)
		oisInputManager->destroyInputSystem(oisInputManager);
}

void InputManager::update() {
	inputListeners.front()->entity->update();
	if(mouse)
		mouse->capture();

	if(keyboard){
		keyboard->capture();		
	}
}

void InputManager::addListener(InputListener *listener) {
	inputListeners.push_back(listener);
}

/**
	events overriden from OIS 
	TODO: entender melhor
**/

bool InputManager::keyPressed(const OIS::KeyEvent &e) {
	std::list<InputListener*>::iterator list_it;
	for(list_it = inputListeners.begin(); list_it != inputListeners.end(); list_it++)
		(*list_it)->keyPressed(e);
	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &e) {
	std::list<InputListener*>::iterator list_it;
	for(list_it = inputListeners.begin(); list_it != inputListeners.end(); list_it++)
		(*list_it)->keyReleased(e);
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &e) {
	std::list<InputListener*>::iterator list_it;
	for(list_it = inputListeners.begin(); list_it != inputListeners.end(); list_it++)
		(*list_it)->mouseMoved(e);
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	std::list<InputListener*>::iterator list_it;
	for(list_it = inputListeners.begin(); list_it != inputListeners.end(); list_it++)
		(*list_it)->mousePressed(e, id);
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	std::list<InputListener*>::iterator list_it;
	for(list_it = inputListeners.begin(); list_it != inputListeners.end(); list_it++)
		(*list_it)->mouseReleased(e, id);
	return true;
}