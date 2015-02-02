#include "PhysicsManager.h"

#include "debug.h"
/** Physics Manager Impl. */
/** Create a box2d body based on the entities properties (should occour conversion from pixel to meters) */
b2Body* PhysicsManager::createBody(EntityFactory* entity) {
	
	float PTM_f = PTM * 1.;
	
	float iniBodyW = (entity->spriteObj->frameWidth/PTM_f) * .98;
	float iniBodyH = (entity->spriteObj->frameHeight/PTM_f) * .98;
		
	iniBodyW /= 2;
	iniBodyH /= 2;

	float iniBodyXPos = entity->spriteObj->posX/PTM_f + iniBodyW;
	float iniBodyYPos = entity->spriteObj->posY/PTM_f + iniBodyH;

	if(entity->spriteObj->resourceUID == 1){
		std::cout << (entity->spriteObj->frameHeight/PTM_f) << "   " << iniBodyH << std::endl;
}
	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	b2Body* body;

	if(entity->spriteObj->resourceUID < WALL) {
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(iniBodyXPos, iniBodyYPos);
	bodyDef.userData = entity;
	bodyDef.fixedRotation = true;
	body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(iniBodyW, iniBodyH);


	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;

	fixtureDef.shape = &dynamicBox;

/********** IF NOT WALL, THEN SET THESE PROPERTIES *******************/
	if(entity->spriteObj->resourceUID < WALL) {
		
		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;
		if(entity->spriteObj->resourceUID == 2)
			fixtureDef.density = 0.000001f;

		// Override the default friction.
		fixtureDef.friction = .9f;
		fixtureDef.restitution = 0.1;
		

	}  else {
		fixtureDef.density = 0;
		fixtureDef.restitution = 0.0;
		fixtureDef.friction = .3;
	}
/********** end: IF NOT WALL, THEN SET THESE PROPERTIES *******************/
		
		body->CreateFixture(&fixtureDef);	
	
/******** mouse joint teste ====== ~~~~~ **********/
		if(entity->spriteObj->resourceUID == 2) {
			
			// a ground body is created only for use with the mouse joint -- body A
			b2BodyDef bodyDef;
			groundBody = world.CreateBody(&bodyDef);
			b2MouseJointDef md;
			md.bodyA = groundBody;
			md.bodyB = body;
			md.target = b2Vec2(0, 0);
			md.maxForce = 8 * body->GetMass();
			mouseJoint = (b2MouseJoint*)world.CreateJoint(&md);
			mouseJoint->SetDampingRatio(15);
			mouseJoint->SetFrequency(2);
			body->SetAwake(true);
		}
/******** end: mouse joint teste ====== ~~~~~ **********/

	return body;

}

bool PhysicsManager::init(std::map<int, std::list<EntityFactory*> >& entities_map) {

	//############### Box2D LISTENERS #############################################
	contactListener = new MyContactListener();
	world.SetContactListener(contactListener);

	//############### Box2D MOUSE JOINT #############################################
	mouseJoint = NULL;
	

	std::map<int, std::list<EntityFactory*> >::iterator map_it;	
	//search through scopes
	for(map_it = entities_map.begin(); map_it != entities_map.end(); ++map_it) {
		if( !map_it->second.empty() ) {
			std::list<EntityFactory*>::iterator list_it;
			//search through entities of a scope
			for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
				//if found 
				if( map_it->first == sceneManager->getCurScene()->uid && (*list_it)->spriteObj->resourceUID < BG)				
					bodies_map[ map_it->first ].push_back(createBody( *list_it ));				
			}
		}
	}   
	
	

	return true;
}

void PhysicsManager::update() {

		removeMarkedBodies();

		// listeners & event handling
		updateCollision();

		updateMovement();

		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Clear applied body forces. We didn't apply any forces, but you
		// should know about this function.
		world.ClearForces();

}

void PhysicsManager::updateCollision() {

std::vector<MyContact>::iterator vec_it;
for(vec_it = contactListener->contacts.begin(); vec_it != contactListener->contacts.end(); ++vec_it) {
    MyContact contact = *vec_it;
 
	
	EntityFactory* entityA = (EntityFactory*) contact.fixtureA->GetBody()->GetUserData();
	EntityFactory* entityB = (EntityFactory*) contact.fixtureB->GetBody()->GetUserData();

	b2Body* bodyA = contact.fixtureA->GetBody();
	b2Body* bodyB = contact.fixtureB->GetBody();

	//if entity == NULL, then is a WALL; else if ( CHECK IDs as in the constants.h file)
	int entityA_ID = entityA ? entityA->spriteObj->resourceUID : WALL;
	int entityB_ID = entityB ? entityB->spriteObj->resourceUID : WALL;

	int entityA_ID_range = (entityA_ID == PLAYER || entityA_ID == METEOR) ? entityA_ID : checkIDrange(entityA_ID);
	int entityB_ID_range = (entityB_ID == PLAYER || entityB_ID == METEOR) ? entityB_ID : checkIDrange(entityB_ID);

	//std::cout << entityA_ID_range << entityB_ID_range  << std::endl;

	if ((entityA_ID_range == PLAYER && entityB_ID_range == WALL) ||
        (entityB_ID_range == PLAYER && entityA_ID_range == WALL)) {								
			raiseAndHandleEvent(PE_PLAYER_WALL, bodyA, bodyB, entityA_ID_range, entityB_ID_range );
	} 

	if ((entityA_ID_range == METEOR && entityB_ID_range == PLAYER) ||
        (entityB_ID_range == METEOR && entityA_ID_range == PLAYER)) {					
		
			raiseAndHandleEvent(PE_METEOR_PLAYER, bodyA, bodyB, entityA_ID_range, entityB_ID_range);
					
	} 

	if ((entityA_ID_range == METEOR && entityB_ID_range == ENEMY) ||
		(entityB_ID_range == METEOR && entityA_ID_range == ENEMY)) {	
			std::cout << "raiseeeeeeeeeeeeeeeeeeeee! gelo no meteoruuuuuu " << std::endl;
			raiseAndHandleEvent(PE_METEOR_ENEMY, bodyA, bodyB, entityA_ID_range, entityB_ID_range );
	}

}

}

void PhysicsManager::updateMovement() {

	float PTM_f = PTM * 1.;

	std::map<int, std::list<b2Body*> >::iterator map_it;	
	//search through scopes
	for(map_it = bodies_map.begin(); map_it != bodies_map.end(); ++map_it) {
		if( !map_it->second.empty() ) {
			std::list<b2Body*>::iterator list_it;
			//search through entities of a scope
			for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
				//if found & entity is moving
				EntityFactory* entity = (EntityFactory*)(*list_it)->GetUserData();

				if( entity->isMoving && entity->spriteObj->resourceUID == 1)  {
					(*list_it)->SetAwake(true);//Wakes the body up if its sleeping
					if (entity->direction & LEFT)
						(*list_it)->SetLinearVelocity( b2Vec2(-2, (*list_it)->GetLinearVelocity().y) );
					else if (entity->direction & RIGHT)
						(*list_it)->SetLinearVelocity( b2Vec2( 2, (*list_it)->GetLinearVelocity().y) );
					else if ( entity->direction & UP) {
						if( (*list_it)->GetLinearVelocity().y == 0 )
						(*list_it)->ApplyLinearImpulse( b2Vec2(0,-18) ,(*list_it)->GetWorldCenter());
					}
				}

				//Modificações - Fábio Picchi 

				if (entity->spriteObj->resourceUID == 1) {
					if ( (*list_it)->GetLinearVelocity().y < 0) {
						entity->stateY = UP;
					}
					else if ( (*list_it)->GetLinearVelocity().y > 0) {
						entity->stateY = DOWN;
					}
					else {
						entity->stateY = NONE;
					}

					if ( (*list_it)->GetLinearVelocity().x > 0) {
						entity->stateX = RIGHT;
					}
					else if ( (*list_it)->GetLinearVelocity().x < 0) {
						entity->stateX = LEFT;
					}
					else {
						if (entity->stateX != NONE) entity->lastStateX = entity->stateX;
						entity->stateX = NONE;
					}
				}

				if( entity->isMoving && entity->spriteObj->resourceUID == 2) {
						
					b2Vec2 p = ConvertScreenToWorld( entity->mouseX, entity->mouseY );

 					mouseJoint->SetTarget(p);
				}
				
				
				// Now print the position and angle of the body.
				b2Vec2 position = (*list_it)->GetPosition();
				float32 angle = (*list_it)->GetAngle();
		
				
				entity->spriteObj->posX = position.x*PTM_f - entity->spriteObj->frameWidth/2.;
				entity->spriteObj->posY = position.y*PTM_f - entity->spriteObj->frameHeight/2.;

				if(entity->spriteObj->resourceUID == 1) {
					//std::cout << "type " << entity->spriteObj->resourceUID << " x: " << position.x*PTM_f << "y: " << position.y*PTM_f << " height " << entity->spriteObj->frameHeight << " angle " << angle << std::endl;
				}

				}
	}

		}

}

void PhysicsManager::raiseAndHandleEvent(PSX_EVENT_TYPE eType, b2Body* bodyA, b2Body* bodyB, int entRangeA, int entRangeB){
	std::list<PhysicsListener*>::iterator listener_it;
			for(listener_it=listeners.begin(); listener_it != listeners.end(); listener_it++) {
				//if event handled
				if( (*listener_it)->listenFor == eType) {
					//Raise Event <==> 'Process event'
					(*listener_it)->event(this, bodyA, bodyB, entRangeA, entRangeB);
				} 
	}

}

void PhysicsManager::addListener(PhysicsListener* listenTo) {
	listeners.push_back(listenTo);
}

//** copied from the Box2D's testbed
b2Vec2 PhysicsManager::ConvertScreenToWorld(int32 x, int32 y)
{
	float32 u = x / PTM;
	float32 v = y / PTM;

	b2Vec2 p;
	p.x = u;
	p.y = v;
	return p;
}

void PhysicsManager::markForRemoval(b2Body* a){
	removal.push_back(a);
}

void PhysicsManager::removeMarkedBodies(){

	//loop no vector
	std::vector<b2Body*>::iterator vec_it = removal.begin();
	std::vector<b2Body*>::iterator vec_end = removal.end();

	for (vec_it; vec_it != vec_end; ++vec_it) {
		
		std::cout << "removeMarkedBodies " << (*vec_it)->GetUserData() << std::endl;
		removeBodyFromSimulation(*vec_it);
		world.DestroyBody( *vec_it );
		
		//remove all other references
		
	}
	removal.clear();	

}

bool PhysicsManager::removeBodyFromSimulation(b2Body* body){

	removeEntityFromMap(body);
	EntityFactory* entity = (EntityFactory*) body->GetUserData();
	entityManager->removeEntityFromSimulation(entity);
	sceneManager->removeSpriteFromScreen(entity->spriteObj);
	return true;

}

bool PhysicsManager::removeEntityFromMap(b2Body* bToRemove){
		std::map<int, std::list<b2Body*> >::iterator map_it;	
	//search through scopes
		for(map_it = bodies_map.begin(); map_it != bodies_map.end(); ++map_it) {
			if( !map_it->second.empty() ) {
				std::list<b2Body*>::iterator list_it;
				//search through resources of scope
				for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
					EntityFactory* entity = (EntityFactory*)(*list_it)->GetUserData();
					if( entity == bToRemove->GetUserData() ){
						map_it->second.erase(list_it);
						return true;
					}

				}
			}
		}
return false;
}


bool PhysicsManager::clear() {
	listeners.clear();
	sceneManager = NULL;
	entityManager = NULL;
	// Este tem q passar cada elemento do map e destruir?
	bodies_map.clear();
	delete(contactListener);
	return true;
}

//Contact listener Impl.
 
void MyContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
	std::cout << "antes" << std::endl;
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    contacts.push_back(myContact);
}
 
void MyContactListener::EndContact(b2Contact* contact) {
	std::cout << "depois" << std::endl;
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<MyContact>::iterator pos;
    pos = std::find(contacts.begin(), contacts.end(), myContact);
    if (pos != contacts.end()) {
        contacts.erase(pos);
    }

}

void MyContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold){
	//SetEnable <- false disables all contacts
	EntityFactory* entityA = (EntityFactory*) contact->GetFixtureA()->GetBody()->GetUserData();
	EntityFactory* entityB = (EntityFactory*) contact->GetFixtureB()->GetBody()->GetUserData();

	b2Fixture* mouseFixture = (entityA->spriteObj->resourceUID == 2)?contact->GetFixtureA(): (entityB->spriteObj->resourceUID == 2)?contact->GetFixtureB():NULL;

	if(entityA->spriteObj->resourceUID < WALL && entityB->spriteObj->resourceUID < WALL) {
		if(mouseFixture){
			//mouseFixture->GetBody()->SetLinearVelocity(b2Vec2(-1.5*mouseFixture->GetBody()->GetLinearVelocity().x,0));
		}
	} 
}

