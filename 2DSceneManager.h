#ifndef _2DSCENEMANAGER_
#define _2DSCENEMANAGER_

#include <string>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <math.h>
#include <SDL.h>
#include <windows.h>
#include "ResourceManager.h"
#include "RenderData.h"
#include "TinyXML.h"
#include <deque>
#include "Timer.h"
#include "CCamera.h" //Este pode sair

//---- EXTRAS ------
//typedef for timing events
typedef enum {SE_TIMER_EXPIRED=0} SCENE_EVENT_TYPE;

//forward declaration of scene manager class
class Layer;
class c2DSceneManager;

//------------------


/** 
Scene listener class: abstract base class whose event method is to be overriden
and redefined as a callback function, to be called at specific times and/or intervals
**/

class SceneListener {
public:
	SCENE_EVENT_TYPE listenFor;
	//method to be overridden
	virtual void event(c2DSceneManager* manager, void* customData) const = 0;
};

/**
	sceneObject: basic currency of a scene
	- all itens beloging to the scene should be derived from this class
	- all instances of this class should be assigned to a layer
**/

class SceneObject : public SDLRenderObject {
private:
public:
protected:
};

/** 
 Layer: each instance has a specified Z-order, and the scenen manager will sort the layers in order
**/

class Layer {
public:	
	Layer() : isVisible(true), ZOrder(0), posX(0), posY(0){}
	bool isVisible;
	unsigned int ZOrder;
	float posX;
	float posY;
	std::string name;
	std::list<SpriteObject*> sceneObjects; /**< SceneObjects are Sprites */
	void update();
	void playObjAnim();
	void pauseObjAnim();
	bool removeSpriteFromCurScene(SpriteObject* sprite);

};

/**
	A 2D Scene that holds Layers
**/

class Scene {
protected:
	
public:
/** Layer list */
	std::list<Layer*> layers;
/** Management of layers that this scene holds */
	int uid;
	Layer* addLayer(std::string name);
	Layer* findLayer(std::string name);
	void removeLayer(std::string name);
	void clearLayers();
	void sortLayers();
};


/**
	2DSceneManager: holds a list of layers, timers and listener objs.
**/

class c2DSceneManager {
protected:
	void addSceneLayers(Scene *scene, TiXmlElement *Element);	
	void addLayerObjects(int sceneUID, Layer* layer, TiXmlElement* element);
	void checkTimerExpired();
	void clearTimers();
	void clearScenes();
	void clearSprites();
	static c2DSceneManager m_c2DSceneManager;
	c2DSceneManager(){};
public:

	/** Map of scene objects
		- mapped by escope and SpriteObject pointers
		- we will be able to quickly find a resource pointer and its Scene escope
	*/
	std::map<int, std::list<SpriteObject*> > sprite_map;
	//Scene list
	std::deque<Scene*> scenes;
	//timer list
	std::list<Timer*> timers;

	//listener list
	std::list<SceneListener*> listeners;

	//get singleton
	static c2DSceneManager* get2DSceneManager();
		
	/** loads a scene from an xml file */
	bool loadFromXMLFile(std::string filename);
	void addTimer(unsigned int ID, float interval);
	void addListener(SceneListener* object);	

	/** Scene manipulation */
	bool swap(int sceneNum);
	Scene* getCurScene();
	void init(std::string xmlFile);
	void update();
	SpriteObject* getSceneObjPtr(int sceneUID, int objUID);
	bool removeSpriteFromScreen(SpriteObject* sprite);
	bool clear();
};


class cTestListener : public SceneListener
{

    public:

        cTestListener()
        {
            listenFor = SE_TIMER_EXPIRED;
        }

        void event(c2DSceneManager* manager, void* customData) const
        {
            Layer* layer = manager->getCurScene()->findLayer("layer2000");
            if(layer) layer->isVisible = !layer->isVisible;
        }
};

#endif