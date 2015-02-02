#include "EntityFactory.h"

#include "debug.h"

EntityFactory* EntityFactory::makeEntity(const int choice) {
	if (choice == 1) {
		return new Player;
	} else if (choice == 2) {
		return new Meteor;
	} else {
		return new Apagar;
	}
	return false;
}

bool EntityFactory::bindResourceToEntity(int sceneUID, int uid) {
	SpriteObject* resource = c2DSceneManager::get2DSceneManager()->getSceneObjPtr(sceneUID, uid);
	if(resource) {
		spriteObj = resource;
		return true;
	} else {
		return false;
	}
}