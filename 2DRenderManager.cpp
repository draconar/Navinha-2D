#include "2DRenderManager.h"

#include "debug.h"
/**
	Impl. of 2DRenderManager member f()'s	
*****/

//get singleton
c2DRenderManager c2DRenderManager::m2DRenderManager;

c2DRenderManager* c2DRenderManager::get2DRenderManager()
{
    return &m2DRenderManager;
}

c2DRenderManager::c2DRenderManager()
{
    renderWindow = NULL;
    sceneManager = NULL;	
	windowHandle = 0;
}


bool c2DRenderManager::init(unsigned int width, unsigned int height, bool fullScreen, char *windowTitle) {
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		return false;
	}

	if(fullScreen) 
		renderWindow = SDL_SetVideoMode(width, height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	else 
		renderWindow = SDL_SetVideoMode(width, height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if (!renderWindow)
		return false;

	SDL_WM_SetCaption(windowTitle, NULL); 

	//populate video info
	const SDL_VideoInfo *VidInfo = SDL_GetVideoInfo();

	videoInfo << 
		"Video Info\n can create hardware surfaces: " << VidInfo->hw_available <<
		"\nWindow Manager available: " << VidInfo->blit_hw <<
		"\nHardware to hardware colokey blits accelerated: " << VidInfo->blit_hw_CC << 
		"\nHardware to hardware alpha blits accelerated " << VidInfo->blit_hw_A <<
		"\nSoftware to hardware blits accelerated: " << VidInfo->blit_sw <<
		"\nSoftware to hardware colorkey blits: " << VidInfo->blit_sw_CC <<
		"\nSoftware to hardware alpha blits accelerated: " << VidInfo->blit_sw_A <<
		"\nColor fills accelerated: " << VidInfo->blit_fill <<
		"\nAmount of video memory in Kb " << VidInfo->video_mem;

		SDL_GetWMInfo(&inf);

		windowHandle = inf.window;
		std::cout << "w handle " << windowHandle << std::endl;

	if ( SDL_InitSubSystem(SDL_INIT_TIMER) < 0 ) {
		return false;
	}

	return true;
}

/** parse and loads into the memory a graphic metadata */
Resource* c2DRenderManager::loadResourceFromXML(TiXmlElement *Element) {
	if (Element) {
		//new instance of a render resource through base class resource (so resource mngr can work on it)
		RenderResource* resource = new RenderResource();
		for(TiXmlAttribute* elementAttrib = Element->FirstAttribute(); elementAttrib; elementAttrib = elementAttrib->Next()) {
			std::string attribName = elementAttrib->Name();
			std::string attribValue = elementAttrib->Value();
			if(attribName=="UID") {
				resource->resourceID = atoi(attribValue.c_str());
			}
			if(attribName=="filename") {
				resource->fileName = attribValue;
			}
			if(attribName=="scenescope") {
				resource->scope = atoi(attribValue.c_str());
			}
			if(attribName == "alpha") 
			{ 
				if (attribValue == "true") resource->alpha = true; 
				else resource->alpha = false; 
			}
		}
		
		resource->type = RESOURCE_GRAPHIC;
		//probe if file exists then load resource
		if( !ResourceManager::probeIfFileExists(resource->fileName) )
			return NULL;
		else {
				

			resource->load();
		}
								

		return static_cast<Resource*>( resource );
	}
	return NULL;
}



void c2DRenderManager::renderAllObjects() {
	std::list<SDLRenderObject*>::iterator list_it;
	//render all
	for(list_it=renderObjects.begin(); list_it != renderObjects.end(); list_it++) {
		if( (*list_it)->isVisible) {
			(*list_it)->update() ;
			SDL_Rect pos;			
			pos.x = int ( (*list_it)->posX);
			pos.y = int ( (*list_it)->posY);		
			SDL_BlitSurface( (*list_it)->renderResource->surface, &(*list_it)->renderRect, renderWindow, &pos);
		}
	}
}

bool c2DRenderManager::update() {
	SDL_Event event;
	while( SDL_PollEvent(&event)) {
		//check for msgs
		switch(event.type) {
			case SDL_QUIT:
				return false;
		//check for keypresses
			case SDL_KEYDOWN:
				{
					//If escape is pressed
					if(event.key.keysym.sym == SDLK_ESCAPE)
						return false;
				}
		}
	}

	//clear screen
	SDL_FillRect(renderWindow, 0, SDL_MapRGB(renderWindow->format, 100, 100, 100));

	renderScene();

	//Call frame listeners
	renderAllObjects();

	SDL_Flip(renderWindow);

	return true;
}

//render a scene
void c2DRenderManager::renderScene() {
	if(sceneManager) {
		Scene* scene = sceneManager->getCurScene();
		std::list<Layer*>::iterator list_it;
		//render all associated render objs
		for(list_it = scene->layers.begin(); list_it != scene->layers.end(); list_it++) {
			//dereference of list_it here is a ptr to a layer
			Layer* layer = *list_it;

			if(layer->isVisible) {
				std::list<SpriteObject*>::iterator object_it;
				for(object_it=layer->sceneObjects.begin(); object_it!=layer->sceneObjects.end();object_it++) {
					SpriteObject* object = *object_it;
					if(object->isVisible) {
							object->update();
						SDL_Rect pos;
						//render objs X Y as offset from layer X Y, change camera if info is available
						layer->posX = -CCamera::CameraControl.GetX();
						layer->posY = -CCamera::CameraControl.GetY();
						pos.x = int(layer->posX) + int(object->posX);
						pos.y = int(layer->posY) + int(object->posY);
						SDL_BlitSurface(object->renderResource->surface, &object->renderRect, renderWindow, &pos);
					}
				}
			}
		}
	}
}

void c2DRenderManager::clear()
{
	//Confirmar
    renderWindow = NULL;
    sceneManager = NULL;	
	windowHandle = 0;
    SDL_Quit();
}

