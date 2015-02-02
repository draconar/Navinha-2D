#ifndef _GAME_CONSTANTS_
#define	_GAME_CONSTANTS_

/** Define entities ID constants to be used throughout the game life */

enum entitiesID {
	PLAYER=1, /**< entities controlable by the player; Box2D dynamic entities */
	METEOR, /**< mouse controlable entity; Box2D mouse joint */
	ENEMY=1000, /**< entities not controlable by the player yet interactable in a way or other; Box2D dynamic entities */
	WALL=2000, /**<  wall, spikes, etc; ; Box2D static entities*/
	BG=3000, /**< Background images, static images, etc; not a Box2D entity */
	CAT=4000, /**< ??????? */
	PROFITS=5000 /**< Profits */
};

/** Helper function: get the range of the entities involved in an event */
inline int checkIDrange(int ID) {

	int ID_range;

	if( ID <= PLAYER )
			ID_range = PLAYER;
	else if ( ID > PLAYER && ID <= METEOR)
			ID_range = METEOR;
	else if(ID >= PLAYER && ID < ENEMY)
			ID_range = PLAYER;
	else if(ID >= ENEMY && ID < WALL)
			ID_range = ENEMY;
	else if(ID >= WALL && ID < BG)
			ID_range = WALL;
	else
			ID_range = CAT;

	return ID_range;
}

//typedef for Physics events
typedef enum PSX_EVENT_TYPE {
	PE_PLAYER_WALL=0,
	PE_PLAYER_ENEMY,
	PE_PLAYER_PLAYER,
	PE_ENEMY_WALL,
	PE_ENEMY_ENEMY,
	PE_ENEMY_PLAYER,
	PE_METEOR_PLAYER,
	PE_METEOR_WALL,
	PE_METEOR_ENEMY
};

const int screenW = 1024;
const int screenH = 768;

#endif