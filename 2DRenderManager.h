#ifndef _2DRENDERMANAGER_
#define _2DRENDERMANAGER_

#include "ResourceManager.h"
#include "2DSceneManager.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_syswm.h>
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <windows.h>
#include <math.h>
#include "CCamera.h"
#include "TinyXML.h"

/**
	the 2D render manager
	create a window, initializes hardware, create, load, unload graphics resources and 
	render graphics
*****/

class c2DRenderManager {
protected:
	c2DRenderManager();
	static c2DRenderManager m2DRenderManager;
	SDL_SysWMinfo inf;
public:
	void renderScene();
	static c2DRenderManager* get2DRenderManager();
	SDL_Surface* renderWindow; //Standard window for render context
	std::stringstream videoInfo;
	bool init(unsigned int width=800, unsigned int height=600, bool fullscreen = false, char* windowTitle = 0);
	void clear();
	bool update();
	void toggleFullScreen();
	Resource* loadResourceFromXML(TiXmlElement *Element);
	void renderAllObjects();
	std::list<SDLRenderObject*> renderObjects;
	c2DSceneManager* sceneManager;
	HWND windowHandle;
};

#endif
