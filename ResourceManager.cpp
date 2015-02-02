#include "ResourceManager.h"
#include "2DRenderManager.h"
#include "AudioManager.h"
#include "boost/filesystem.hpp"
#include "ErrorLogManager.h"

#include "debug.h"

#define SAFE_DELETE(a) {delete a; (a)=NULL;}
/**
	Impls. of the ResourceManager class' member f()s
****/

ResourceManager ResourceManager::resourceManager;

// GET SINGLETON-----------------------------------------------------

ResourceManager* ResourceManager::getResourceManager()
{
    return &resourceManager;
}


bool ResourceManager::loadFromXMLFile(std::string fileName) {

	TiXmlDocument doc(fileName.c_str());
	
	if(doc.LoadFile()) {
		//find topmost resource node
		TiXmlNode* resourceTree = doc.FirstChild("resources");
		if(resourceTree) {
			//enum resource objs
			for(TiXmlNode* child = resourceTree->FirstChild(); child; child=child->NextSibling()) {
				TiXmlElement *element = child->ToElement();
				if(element) {
					Resource *resource = NULL;
					for(TiXmlAttribute* elementAttrib = element->FirstAttribute(); elementAttrib; elementAttrib=elementAttrib->Next()) {
						//examine resource obj
						std::string attribName = elementAttrib->Name();
						std::string attribValue = elementAttrib->Value();
						//resource type?
						if(attribName == "type") {
							//let managers implement their own derived versions of Resource.
							//add a Resource to the Map< , >
							if(attribValue == "graphic") {								
								c2DRenderManager* g_RenderManager = c2DRenderManager::get2DRenderManager();
                                resource = g_RenderManager->loadResourceFromXML(element);									
							}
							if(attribValue == "audio") {
								AudioManager* audioManager = AudioManager::getAudioManager();
								resource = audioManager->loadResourceFromXML(element);
							}
							if(attribValue == "text") {
								//resource = configManager->loadResourceFromXML(element);
							}
						}
						if(resource){
							if(attribName == "UID") resource->resourceID = atoi(attribValue.c_str());						
							if(attribName == "filename") {
								resource->fileName = attribValue;								
							}
							if(attribName == "scenescope") resource->scope = atoi(attribValue.c_str()); 
						}					
					}
					//pode haver prob. aqui. checar aninhamento
					if(resource) {						
						resources_map[resource->scope].push_back(resource);
						resourceCount++;
					}
				}
			}
			return true;
		}
	}
	return false;
}

std::vector<Resource*> ResourceManager::findResourceByID(unsigned int UID) {
	std::map<unsigned int, std::list<Resource*> >::iterator it;
	std::vector<Resource*> listaObjetos;

	//search through scopes
	for(it = resources_map.begin(); it != resources_map.end(); ++it) {
		if( !it->second.empty() ) {
			std::list<Resource*>::iterator list_it;
			//search through resources of scope
			for(list_it = it->second.begin(); list_it != it->second.end(); ++list_it ) {
				//if found by ID
				if ( (*list_it)->resourceID == UID ) listaObjetos.push_back(*list_it);				
			}
		}
	}
	return listaObjetos;
}


void ResourceManager::clear() {
	std::map<unsigned int, std::list<Resource*> >::iterator it;
	//search through scopes
	for(it = resources_map.begin(); it != resources_map.end(); ++it) {
		if( !it->second.empty() ) {
			std::list<Resource*>::iterator list_it;
			//search through resources of scope
			for(list_it = it->second.begin(); list_it != it->second.end(); ++list_it ) {
				//if found by ID, SAFE DELETE
				(*list_it)->unload();
				SAFE_DELETE(*list_it);
			}
			it->second.clear();
		}
	}

}

void ResourceManager::setCurrentScope(unsigned int scope) {
	//Unload old scope, if not global
	if(currentScope != 0) {
		std::list<Resource*>::iterator list_it;
		
		for(list_it = resources_map[currentScope].begin(); list_it != resources_map[currentScope].end(); ++list_it ) {
				//unload
				(*list_it)->unload();				
		}
	}

		currentScope = scope;

		//Load new scope
		std::list<Resource*>::iterator list_it;

		for(list_it = resources_map[currentScope].begin(); list_it != resources_map[currentScope].end(); ++list_it ) {
				//if found by ID then load it!
				(*list_it)->load();						

		}
		
}

void ResourceManager::printResources() {
	std::map<unsigned int, std::list<Resource*> >::iterator it;	
	//search through scopes
	for(it = resources_map.begin(); it != resources_map.end(); ++it) {
		if( !it->second.empty() ) {
			std::list<Resource*>::iterator list_it;
			//search through resources of scope
			for(list_it = it->second.begin(); list_it != it->second.end(); ++list_it ) {
				//if found 
				std::cout << "filename: " << (*list_it)->fileName <<
					"ID " << (*list_it)->resourceID << "Type: " << (*list_it)->type << std::endl;				
			}
		}
	}		
}


bool ResourceManager::probeIfFileExists(std::string filename) {	
namespace bf = boost::filesystem; 


bf::path p(filename);
	if( !bf::exists(p) ) {
		THROW_EXCEPTION(2, filename + " FILE NOT FOUND");	
		std::cout<< filename << " does not exist.\n";
			return false;
		}


return true;
}