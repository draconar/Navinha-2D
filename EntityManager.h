#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include <iostream>
#include <map>
#include <list>
#include <string>
#include "Timer.h"
#include "EntityFactory.h"

class EntityManager;
class EntityFactory;

//---- EXTRAS ------
//typedef for timing events
typedef enum {EE_TIMER_EXPIRED=0} ENTITY_EVENT_TYPE;


/** 
Scene listener class: abstract base class whose event method is to be overriden
and redefined as a callback function, to be called at specific times and/or intervals
**/

class EntityListener {
public:
	ENTITY_EVENT_TYPE listenFor;
	//method to be overridden
	virtual void event(EntityManager* manager, void* customData) const = 0;
};



class EntityManager {
private:
	bool addEntityToMap(int, EntityFactory*);
	EntityManager(){};
	static EntityManager mEntityManager;
	//listener list
	std::list<EntityListener*> listeners;
	//timer list
	std::list<Timer*> timers;
	void checkTimerExpired();
	void clearTimers();
	void clearEntities();
	void bindSpriteToEntity(std::map<int, std::list<SpriteObject*> >& sprite_map);
	EntityFactory* EntityManager::findEntityInMapById(int id);
	
public:

	/**return entity manager singleton*/
	static EntityManager* getEntityManager();

	std::map<int, std::list<EntityFactory*> > entities_map;
	void update();
	void addTimer(unsigned int ID, float interval);
	void addListener(EntityListener* object);
	/** Todo: will be initialized with a list of Entities, probably using the JScript engine */
	bool init(std::map<int, std::list<SpriteObject*> >& sprite_map);
	EntityFactory* getSingleEntity(const std::string& name);
	bool removeEntityFromSimulation(EntityFactory* entity);
	bool clear();

};

class cEntityTestListener : public EntityListener
{

    public:

        cEntityTestListener()
        {
            listenFor = EE_TIMER_EXPIRED;
        }

		void event(EntityManager* manager, void* customData) const
        {
			//std::cout << "estou no event to entity listener" << std::endl;
        }
};

#endif