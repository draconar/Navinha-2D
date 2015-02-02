#ifndef _RESOURCEMANAGER_
#define _RESOURCEMANAGER_

//#include "EngineObject.h"
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include "TinyXML.h"

//Enum to indicate resource type
typedef enum {
	RESOURCE_NULL = 0,
	RESOURCE_GRAPHIC,
	RESOURCE_MOVIE,
	RESOURCE_AUDIO,
	RESOURCE_TEXT,
}RESOURCE_TYPE;

/** 
	RESOURCE CLASS:
	- Represents a single resource to be managed by resource manager.
	- will be derived, so other managers can use their specifics resources, e.g. 
	RenderManager has a custom graphics resource class; audio manager an audio one, etc
*****/

class Resource {

protected:
	bool isLoaded;

public:
	unsigned int resourceID;
	unsigned int scope;
	std::string fileName;
	RESOURCE_TYPE type;

	virtual ~Resource(){};
	virtual void load(){};
	virtual void unload(){};

	inline Resource(){
		resourceID = scope = 0;
		isLoaded = false;
		type = RESOURCE_NULL;
	}

};

/** 
	RESOURCE MANAGER:
	manages collections of resources. duh.
*****/

class ResourceManager {
private:
	unsigned int currentScope;
	// # of loaded/non-loaded resources
	unsigned int resourceCount;
	static ResourceManager resourceManager;

public:
	std::map<unsigned int, std::list<Resource*> > resources_map;
	//find resource by ID, return null if resource not found
	std::vector<Resource*> findResourceByID(unsigned int UID);
	void clear();
	bool loadFromXMLFile(std::string fileName);
	void setCurrentScope(unsigned int scope);
	static ResourceManager* getResourceManager();
	const unsigned int getResourceCount(){
		return resourceCount;
	}
	//inlined
	ResourceManager() {
		currentScope = resourceCount = 0;
	}
	void printResources();
	static bool probeIfFileExists(std::string filename);
};


#endif