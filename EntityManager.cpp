#include "EntityManager.h"

#include "debug.h"

EntityManager EntityManager::mEntityManager;

EntityManager* EntityManager::getEntityManager() {
	return &mEntityManager;
}


bool EntityManager::init(std::map<int, std::list<SpriteObject*> >& sprite_map) {
	bindSpriteToEntity(sprite_map);
	return true;
}

EntityFactory* EntityManager::getSingleEntity(const std::string& name) {
	if(name == "player")
		return findEntityInMapById(1);
	else if (name == "meteor")
		return findEntityInMapById(2);
	else
		return NULL;
}

EntityFactory* EntityManager::findEntityInMapById(int id){

	std::map<int, std::list<EntityFactory*> >::iterator map_it;	
	//search through scopes
	for(map_it = entities_map.begin(); map_it != entities_map.end(); ++map_it) {
		if( !map_it->second.empty() ) {
			std::list<EntityFactory*>::iterator list_it;
			//search through resources of scope
			for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
				//bind sprite to entity: but first create a entity through the factory 
				if( (*list_it)->spriteObj->resourceUID == id ){
					return (*list_it);
				}

			}
		}
	}
return NULL;
}

void EntityManager::bindSpriteToEntity(std::map<int, std::list<SpriteObject*> >& sprite_map) {
	std::map<int, std::list<SpriteObject*> >::iterator map_it;	
	//search through scopes
	for(map_it = sprite_map.begin(); map_it != sprite_map.end(); ++map_it) {
		if( !map_it->second.empty() ) {
			std::list<SpriteObject*>::iterator list_it;
			//search through resources of scope
			for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
				//bind sprite to entity: but first create a entity through the factory 
				if( *list_it ){
					EntityFactory* entity = EntityFactory::makeEntity( (*list_it)->resourceUID );	
					entity->bindResourceToEntity(map_it->first, (*list_it)->resourceUID);
					entities_map[ map_it->first ].push_back(entity);
				}
			}
		}
	}
}

bool EntityManager::addEntityToMap(int sceneID, EntityFactory* entity) {
	entities_map[sceneID].push_back(entity);
	return true;
}

void EntityManager::addListener(EntityListener* object) {
	listeners.push_back(object);
}

/** add timer to the entity manager*/
void EntityManager::addTimer(unsigned int ID, float interval) {
	Timer* timer = new Timer();
	timer->ID=ID;
	timer->interval=interval;
	timers.push_back(timer);
	//start the timer
	timer->start();
}

void EntityManager::checkTimerExpired() {
	std::list<Timer*>::iterator list_it;
	//check all timers
	for(list_it = timers.begin(); list_it != timers.end(); list_it++) {
		(*list_it)->update();
		//if expired
		if( (*list_it)->expired) {
			std::list<EntityListener*>::iterator listener_it;
			for(listener_it=listeners.begin(); listener_it != listeners.end(); listener_it++) {
				//if event handled
				if( (*listener_it)->listenFor == SE_TIMER_EXPIRED) {
					//Raise Event <==> 'Process event'
					(*listener_it)->event(this, NULL);
					//Restart timer
					(*list_it)->start();
				}
			}
		}
	}
}

void EntityManager::update()
{
    checkTimerExpired();
}

bool EntityManager::removeEntityFromSimulation(EntityFactory* entity) {
	std::map<int, std::list<EntityFactory*> >::iterator map_it;	
	//search through scopes
	for(map_it = entities_map.begin(); map_it != entities_map.end(); ++map_it) {
		if( !map_it->second.empty() ) {
			std::list<EntityFactory*>::iterator list_it;
			//search through resources of scope
			for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
				//bind sprite to entity: but first create a entity through the factory 

				if( (*list_it) == entity ){
					//TODO OLHA ISSO>>>>>>>>>>>>>>>>>>>>>
					map_it->second.remove(*list_it);
					return true;
				}

			}
		}
	}
return false;	
}

bool EntityManager::clear() {
	//Limpar todos os timers com delete
	clearTimers();
	listeners.clear();
	//Ir em cada entity e matar com delete
	clearEntities();
	return true;
}

void EntityManager::clearTimers() {
	std::list<Timer*>::iterator list_it;
	for (list_it = timers.begin(); list_it != timers.end(); ++list_it) {
		delete(*list_it);
		(*list_it) = NULL;
	}
	timers.clear();
}

void EntityManager::clearEntities() {
	std::map<int, std::list<EntityFactory*> >::iterator it;
	for (it = entities_map.begin(); it != entities_map.end(); ++it) {
		if (!it->second.empty()) {
			std::list<EntityFactory*>::iterator list_it;
			for (list_it = it->second.begin(); list_it != it->second.end(); ++list_it) {
				(*list_it)->spriteObj = NULL;
				delete((*list_it)->timer);
				delete(*list_it);
				(*list_it) = NULL;
			}
			it->second.clear();
		}
	}
	entities_map.clear();
}