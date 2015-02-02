#ifndef _RENDERDATA_
#define _RENDERDATA_

#include <SDL.h>
#include <SDL_Image.h>
#include <windows.h>
#include "ResourceManager.h"
#include <vector>

/**
	definition of a graphic resource.
	inherits from the interface Resource "ResourceManager.h"
	pointers to instances to this class are maintened by resourceManager
	- deals with raw resources
*****/

class RenderResource : public Resource {
public:
	SDL_Surface* surface;
	bool alpha;
	void load();
	void unload();
	
};

/**
	a 2D Rendered Object
	each instance represents a unique renderable item in a 2D scene
	- 'in short, the SDLRenderObject is responsible for drawing a specified graphics resource on-screen according
	to a given range of settings
*****/

class SDLRenderObject {
public:
	RenderResource* renderResource;
	//Modificação Fábio - Lista de Stripes para animação
	std::vector<RenderResource*> listaObjetos;
	SDL_Rect renderRect;
	float posX;
	float posY;
	bool isVisible;
	int resourceUID;
	bool isColorKeyEnabled;
	bool isLoaded;
	bool setResourceObject(std::vector<Resource*> rrResource);
	void setColorKey(unsigned int r,unsigned  int g,unsigned  int b);
	SDLRenderObject() : isLoaded(false){};
	virtual void update(){};
	
};

/**
	a 2D Sprite Object
	Derived from render object, contains extra properties for creating animated tileset	
*****/

class SpriteObject : public SDLRenderObject {
public:	
	float timeLastFrame;
	unsigned int totalFrames;
	unsigned int framesPerRow;
	unsigned int framesPerColumn;
	unsigned int currentFrame;
	unsigned int startFrame;
	unsigned int rowNumber;
	unsigned int columnNumber;
	float speed;
	unsigned int frameWidth;
	unsigned int frameHeight;
	bool binded; /**< binded to/as a entity */
	//TODO: deixar tudo zero based
	SpriteObject() : totalFrames(0), framesPerRow(1), framesPerColumn(1), currentFrame(0), 
		startFrame(0), speed(0), frameWidth(32), frameHeight(32), binded(false), rowNumber(0), columnNumber(0){}
	void update();
	void play();
	void stop();
	void setFrameRect(unsigned int currentFrame);
};

#endif
