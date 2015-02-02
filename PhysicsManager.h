#ifndef _PSX_MANAGER_
#define _PSX_MANAGER_

#include <iostream>
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include <list>
#include <vector> 
#include <algorithm>
#include "EntityFactory.h"
#include "constants.h"
#include "EntityManager.h"

class PhysicsManager;

/** 
Physics listener class: abstract base class whose event method is to be overriden
and redefined as a callback function, to be called at specific times and/or intervals
**/

class PhysicsListener {
public:
	PSX_EVENT_TYPE listenFor;
	//method to be overridden
	virtual void event(PhysicsManager* manager, b2Body* bodyA, b2Body* bodyB, int entityA_ID_range, int entityB_ID_range) const = 0;
};


struct MyContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class MyContactListener : public b2ContactListener {
 
public:
    std::vector<MyContact> contacts;
 
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);  
	virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
 
};

class PhysicsManager {
private:
	//b2MouseJoint test
	b2MouseJoint* mouseJoint;
	b2Vec2 mouseWorld;

	//entity manager singleton -> after adding / removing bodies from the 
	//physics manager
	
	std::list<PhysicsListener*> listeners;
	std::map<int, std::list<b2Body*> > bodies_map;
	b2Body* createBody(EntityFactory* entity);
	void updateCollision();
	void updateMovement();
	void raiseAndHandleEvent(PSX_EVENT_TYPE eType, b2Body* bodyA, b2Body* bodyB, int entityA_ID_range, int entityB_ID_range);
	void removeMarkedBodies();
	bool removeBodyFromSimulation(b2Body* body);
	bool removeEntityFromMap(b2Body* body);

	//copied from Box2D's testbed
	b2Vec2 ConvertScreenToWorld(int32 x, int32 y);

//TODO: otimizar -> passar código abaixo para conteiner? teste de listeners
	MyContactListener* contactListener;
//######################WORLD###############################//
	
	//Constants
	static const int PTM = 64; /**< PTM = 1m; Screen to world calculations: SCREEN/PTM; World to screen calculations: WORLD*PTM; */


	// Define the gravity vector.
	b2Vec2 gravity;
	// Do we want to let bodies sleep?
	bool doSleep;
	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world;

// Define the ground body.
	b2BodyDef groundBodyDef;
	b2Body* groundBody;

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep;
	int32 velocityIterations;
	int32 positionIterations;

	//bodies marked for removel after physics loop
	std::vector<b2Body*> removal;

public:
	PhysicsManager() : doSleep(true), gravity(0.0f, 5.0f), world(gravity, doSleep), timeStep(1.f/60.f), velocityIterations(6), positionIterations(10){}
	bool init(std::map<int, std::list<EntityFactory*> >& entities_map);
	void update();
	bool clear();
	c2DSceneManager* sceneManager;
	EntityManager* entityManager;
	void addListener(PhysicsListener* listenTo);
	void markForRemoval(b2Body* b);
};


class testPSXlistener : public PhysicsListener {
	public:

		testPSXlistener()
        {
            listenFor = PE_PLAYER_WALL;
        }

		void event(PhysicsManager* manager, b2Body* bodyA, b2Body* bodyB, int entityA_ID_range, int entityB_ID_range) const
        {
			//std::cout << "estou no chão " << listenFor <<std::endl;
        }
};

class mousePSXListener : public PhysicsListener {
	public:

		mousePSXListener()
        {
            listenFor = PE_METEOR_ENEMY;
        }

		void event(PhysicsManager* manager, b2Body* bodyA, b2Body* bodyB, int entityA_ID_range, int entityB_ID_range) const
        {
			std::cout << "estou no GELO :: listenFor " << listenFor <<std::endl;
			b2Body* ice = entityA_ID_range == METEOR ? bodyB : bodyA;
			std::cout << "body do ice eh " << ((EntityFactory*) ice->GetUserData())->spriteObj->resourceUID << std::endl;

			manager->markForRemoval(ice);
        }
};

#endif