#include "2DSceneManager.h"

#include "debug.h"
/**
	SceneManager Singleton 
**/
c2DSceneManager c2DSceneManager::m_c2DSceneManager;
c2DSceneManager* c2DSceneManager::get2DSceneManager()
{
    return &m_c2DSceneManager;
}


/**
	IMPL. do 2DSceneManager
**/

void c2DSceneManager::init(std::string xmlFile){
	loadFromXMLFile("SceneTree.xml");
}

SpriteObject* c2DSceneManager::getSceneObjPtr(int sceneUID, int objUID) {
	
	std::map<int, std::list<SpriteObject*> >::iterator map_it;	
	//search through scopes
	for(map_it = sprite_map.begin(); map_it != sprite_map.end(); ++map_it) {
		if( !map_it->second.empty() ) {
			std::list<SpriteObject*>::iterator list_it;
			//search through resources of scope
			for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
				//if found 
				if( (*list_it)->resourceUID == objUID && (*list_it)->binded == false ) {
					(*list_it)->binded = true;
					return *list_it;				
				}
			}
		}
	}
	return NULL;
}

/** swap scene */
bool c2DSceneManager::swap(int sceneNum) {
	Scene* buf = scenes.front();
	Scene* newFront;

	std::deque<Scene*>::iterator deq_it = scenes.begin();

	for (deq_it; deq_it !=scenes.end(); ++deq_it)
		if( (*deq_it)->uid == sceneNum ) {
			newFront = *deq_it;
			scenes.erase(deq_it);
			break;
		}

	scenes.push_front(newFront);

	std::cout << scenes.size() << std::endl;

	return true;
}

/** add timer to 2d scene */
void c2DSceneManager::addTimer(unsigned int ID, float interval) {
	Timer* timer = new Timer();
	timer->ID=ID;
	timer->interval=interval;
	timers.push_back(timer);
	//start the timer
	timer->start();
}

/** add a listener to the scene */
void c2DSceneManager::addListener(SceneListener *object) {
	listeners.push_back(object);
}

void c2DSceneManager::update()
{
    checkTimerExpired();
}


Scene* c2DSceneManager::getCurScene(){
	return scenes.front();
}

void c2DSceneManager::checkTimerExpired() {
	std::list<Timer*>::iterator list_it;
	//check all timers
	for(list_it = timers.begin(); list_it != timers.end(); list_it++) {
		(*list_it)->update();
		//if expired
		if( (*list_it)->expired) {
			std::list<SceneListener*>::iterator listener_it;
			for(listener_it=listeners.begin(); listener_it != listeners.end(); listener_it++) {
				//if event handled
				if( (*listener_it)->listenFor == SE_TIMER_EXPIRED) {
					//Raise Event
					(*listener_it)->event(this, NULL);
					//Restart timer
					(*list_it)->start();
				}
			}
		}
	}
}

bool c2DSceneManager::loadFromXMLFile(std::string filename) {
   TiXmlDocument doc(filename.c_str());

    if(doc.LoadFile())
    {
        //Find resources node
        TiXmlNode* ResourceTree = doc.FirstChild("scenes");

        if(ResourceTree)
        {
            //Enumerate resource objects
            for(TiXmlNode* child = ResourceTree->FirstChild(); child; child = child->NextSibling())
            {
                TiXmlElement *Element = child->ToElement();

                if(Element)
                {
	                Scene* scene = new Scene();
					scene->uid = 0;                    

                    for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib = ElementAttrib->Next())
					{
                        //Examine scenes
                        std::string AttribName = ElementAttrib->Name();
                        std::string AttribValue = ElementAttrib->Value();

                        //Detect resource type. Graphic? Audio? Text?
                        if(AttribName=="UID")
                        {
							scene->uid = atoi(AttribValue.c_str());
                            continue;
                        }
					}

                    scenes.push_back(scene);

					//Cycle through layers
					for(TiXmlNode* objs = child->FirstChild(); objs; objs = objs->NextSibling())
					{
					    if(std::string(objs->Value())=="layers")
					    {
					        for(TiXmlNode* obj = objs->FirstChild(); obj; obj = obj->NextSibling())
					        {
                                TiXmlElement *ObjElement = obj->ToElement();

                                addSceneLayers(scene, ObjElement);
					        }
                        }
					}
                }
            }

             return true;
        }
    }

    return false;
}


void c2DSceneManager::addSceneLayers(Scene *scene, TiXmlElement *Element){
Layer* layer = new Layer();

if(!layer)
		return;

if(Element)
                {                    
					layer->ZOrder = scene->layers.size();

                    for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib = ElementAttrib->Next())
					{
                        //Examine layers
                        std::string AttribName = ElementAttrib->Name();
                        std::string AttribValue = ElementAttrib->Value();

                        //Detect resource type. Graphic? Audio? Text?
                        if(AttribName=="name")
                        {
                            layer->name=AttribValue;
                            continue;
                        }

                        if(AttribName=="posx")
                        {
                            layer->posX = atof(AttribValue.c_str());
                        }

                        if(AttribName=="posy")
                        {
                            layer->posY = atof(AttribValue.c_str());
                        }

                        if(AttribName=="visible")
                        {
                            if(AttribValue=="true")
                                layer->isVisible=true;
                            else
                                layer->isVisible=false;
                        }
					}

					//Cycle through layer objects
					for(TiXmlNode* objs = Element->FirstChild(); objs; objs = objs->NextSibling())
					{
					    if(std::string(objs->Value())=="objects")
					    {
					        for(TiXmlNode* obj = objs->FirstChild(); obj; obj = obj->NextSibling())
					        {
                                TiXmlElement *ObjElement = obj->ToElement();

								addLayerObjects(scene->uid, layer, ObjElement);
					        }
                        }
					}

				scene->layers.push_back(layer);	

                }//endif

		scene->sortLayers();

}

             

void c2DSceneManager::addLayerObjects(int sceneUID, Layer *layer, TiXmlElement *Element) {
	SpriteObject* object = new SpriteObject();
	unsigned int r=0;
	unsigned int g=0;
	unsigned int b=0;

	if(!object)
		return;

    for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib = ElementAttrib->Next())
    {
        std::string AttribName = ElementAttrib->Name();
        std::string AttribValue = ElementAttrib->Value();
        if(AttribName=="resourceID")
        {
			//get singleton
            ResourceManager* resourceManager = ResourceManager::getResourceManager();
			//if resource not found, delete intermediate object and then return
			int resourceUID = atoi(AttribValue.c_str());
			if( object->setResourceObject(
				resourceManager->findResourceByID(resourceUID))
				) {
					object->isLoaded = true;
					object->resourceUID = resourceUID;
			} else {
				delete object;
				return;
			}
        }
		

        if(AttribName=="posx")
        {
			object->posX = layer->posX + atof(AttribValue.c_str());
			/**todo: retirar esse zero para quando for sprite**/
			//object->renderRect.x = 0;
        }

        if(AttribName=="posy")
        {
            object->posY = layer->posY + atof(AttribValue.c_str());
			/**todo: retirar esse zero para quando for sprite**/
			//object->renderRect.y = 0;
        }

		if(AttribName=="w")
		{
			object->frameWidth = atoi(AttribValue.c_str());
		}

		if(AttribName=="h")
		{
			object->frameHeight = atoi(AttribValue.c_str());
		}

		if(AttribName=="animspeed")
		{
			object->speed = atof(AttribValue.c_str());
		}

        if(AttribName=="colorkey")
        {
            if(AttribValue=="true")
               object->isColorKeyEnabled=true;
            else
               object->isColorKeyEnabled=false;
        }
		if(AttribName=="visible"){
			
			if(AttribValue=="true")
				object->isVisible=true;
             else
				object->isVisible=false;
			
		}

		if(AttribName=="r")
			{
				r = atoi(AttribValue.c_str());
			}

		if(AttribName=="g")
			{
				g = atoi(AttribValue.c_str());
			}

		if(AttribName=="b")
			{
				b = atoi(AttribValue.c_str());
			}			
		if (AttribName == "framesPerRow")
		{
			object->framesPerRow = atoi(AttribValue.c_str());
		}
				
    }

	//basic configuration of a layer object
	object->setFrameRect(object->currentFrame);
	object->setColorKey(r, g, b);
	
    layer->sceneObjects.push_back(object);
	sprite_map[sceneUID].push_back(object);
	return;
}

bool c2DSceneManager::removeSpriteFromScreen(SpriteObject* sprite){	
	Scene* scene = getCurScene();
		std::list<Layer*>::iterator list_it;
		//render all associated render objs
		for(list_it = scene->layers.begin(); list_it != scene->layers.end(); list_it++) {
			//dereference of list_it here is a ptr to a layer
			Layer* layer = *list_it;

			if(layer->isVisible) {
				std::list<SpriteObject*>::iterator object_it;
				for(object_it=layer->sceneObjects.begin(); object_it!=layer->sceneObjects.end();object_it++) {
					SpriteObject* object = *object_it;
					if(object == sprite) {
							layer->sceneObjects.erase(object_it);
							return true;
					}
				}
			}
		}
	/*
	std::map<int, std::list<SpriteObject*> >::iterator map_it;	
	//search through scopes
		for(map_it = sprite_map.begin(); map_it != sprite_map.end(); ++map_it) {
			if( !map_it->second.empty() ) {
				std::list<SpriteObject*>::iterator list_it;
				//search through resources of scope
				for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it ) {
					if( *list_it == sprite ){
						map_it->second.erase(list_it);
						return true;
					}

				}
			}
		}*/

return false;
}

bool c2DSceneManager::clear() {

	//Limpar os timers ir em toda lista e matar com delete
	clearTimers();
	listeners.clear();

	//Ir em cada Scene e matar os Layers
	clearScenes();
	//Ir em cada sprite e matar
	clearSprites();

	return true;
}

void c2DSceneManager::clearTimers() {
	std::list<Timer*>::iterator list_it;
	for(list_it = timers.begin(); list_it != timers.end(); ++list_it ) {
		delete(*list_it);
		(*list_it)=NULL;
	}
	timers.clear();
}

void c2DSceneManager::clearScenes() {
	std::deque<Scene*>::iterator list_it;
	for(list_it = scenes.begin(); list_it != scenes.end(); ++list_it ) {
		(*list_it)->clearLayers();
		delete(*list_it);
		(*list_it)=NULL;
	}
	scenes.clear();
}
void c2DSceneManager::clearSprites() {
	std::map<int, std::list<SpriteObject*> >::iterator it;
	for(it = sprite_map.begin(); it != sprite_map.end(); ++it) {
		if( !it->second.empty() ) {
			std::list<SpriteObject*>::iterator list_it;
			for(list_it = it->second.begin(); list_it != it->second.end(); ++list_it ) {
				delete(*list_it);
				(*list_it)=NULL;
			}
			it->second.clear();
		}
	}
	sprite_map.clear();
}

/**
	Impl. of Layer methods
**/

void Layer::playObjAnim() {
		std::list<SpriteObject*>::iterator object_it;
			for(object_it=sceneObjects.begin(); object_it!=sceneObjects.end();object_it++) {
				SpriteObject* object = *object_it;
				if(object->isVisible) {
					object->play();					
				}
			}
}

/** 
	Impl. of Scene Methods
**/

Layer* Scene::addLayer(std::string name) {
	Layer* layer = findLayer(name);

	if(!layer) {
		layer = new Layer();
		layer->name = name;
		layers.push_back(layer);
	}
	return layer;
}


Layer* Scene::findLayer(std::string name) {
	std::list<Layer*>::iterator list_it;
	//render all associated render objs
	for(list_it = layers.begin(); list_it != layers.end(); list_it++)
		if( (*list_it)->name == name )
			return (*list_it);

	return NULL;
}

/** removes name with a specified name from the scene */
void Scene::removeLayer(std::string name) {
	std::list<Layer*>::iterator list_it;
	//render all associated render objs
	for(list_it=layers.begin(); list_it != layers.end(); list_it++)
		if( (*list_it)->name == name )
			layers.remove(*list_it);
}

/** sort layers by Z-Order */
bool compareLayerOrder(const Layer* lhs, const Layer* rhs) {
	return lhs->ZOrder < rhs->ZOrder;
}

void Scene::clearLayers() {
	std::list<Layer*>::iterator list_it;
	for(list_it = layers.begin(); list_it != layers.end(); list_it++) {
		delete(*list_it);
		(*list_it)=NULL;
	}
	layers.clear();
}

/** sort the list of layers by Z-Order
should be called once for every Z-Order change */
void Scene::sortLayers() {
	layers.sort(compareLayerOrder);
}


