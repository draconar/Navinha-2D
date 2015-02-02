#include "RenderData.h"

#include "debug.h"

/**
	Impl. of RenderResource member f()'s	
	- deals with each instance of a raw resource. here is: graphics.
*****/

void RenderResource::load() {
	//unload();
	//load gfx into temp buffer
	//uses IMG_Load SDL function
	//supports: BMP, GIF, JPEG, LBM, PCX, PNG, PNM, TGA, TIFF, XCF, XPM, XV
	SDL_Surface *tmpSurface = IMG_Load(fileName.c_str());

	if(tmpSurface && !alpha) {
	//creates optimized img buffer
		surface = SDL_DisplayFormat(tmpSurface);
	//free old buffer
		SDL_FreeSurface(tmpSurface);
		if(surface)
			isLoaded = true;
	}

	else if (tmpSurface && alpha) {

		//surface = tmpSurface;
		//free old buffer
		surface = SDL_DisplayFormatAlpha(tmpSurface);
		SDL_FreeSurface(tmpSurface);
		if(surface)
			isLoaded = true;

	}
	
}

void RenderResource::unload() {
	if(surface) {
		SDL_FreeSurface(surface);
		surface=NULL;
	}
	isLoaded = false;
}

/**
	Impl. of SDLRenderObject
*****/

bool SDLRenderObject::setResourceObject(std::vector<Resource*> rrResource) {
	std::vector<Resource*>::iterator itr;
	
	if(!rrResource.empty()) {
		for (itr = rrResource.begin(); itr != rrResource.end(); ++itr)
			listaObjetos.push_back((RenderResource*)*itr);
		renderResource = listaObjetos.front();
		return true;
	}

	return false;
}

void SDLRenderObject::setColorKey(unsigned int r, unsigned int g, unsigned int b)
{
	if(isColorKeyEnabled) {	    
		Uint32 colorkey = SDL_MapRGB(renderResource->surface->format, r, g, b);
		SDL_SetColorKey(renderResource->surface, SDL_SRCCOLORKEY, colorkey);
	}
}



/**
	Impl. of SpriteObject
*****/

void SpriteObject::setFrameRect(unsigned int currentFrame) {
	
	/*unsigned int rowNumber = currentFrame/framesPerRow;
	unsigned int columnNumber = currentFrame;

	if(rowNumber > 0)
		columnNumber = currentFrame - (rowNumber * framesPerRow);

	renderRect.x = frameWidth*(currentFrame%framesPerRow) ;
	renderRect.y = frameHeight*(rowNumber%framesPerColumn);
	renderRect.w = frameWidth;
	renderRect.h = frameHeight;*/

	//Modificações Fábio
	columnNumber = currentFrame%framesPerRow;

	/*if(rowNumber > 0)
		columnNumber = currentFrame - (rowNumber * framesPerRow);*/

	renderRect.x = frameWidth*(columnNumber);
	//renderRect.y = frameHeight*(rowNumber);
	renderRect.y = 0;
	renderRect.w = frameWidth;
	renderRect.h = frameHeight;
	
}

//Called to start animation playback
//TODO: acho que esse método deve ser ligado á TIMERs
void SpriteObject::play() {
	//calculate frame dimensions
	SDL_Surface *tmpSurface = renderResource->surface;
	setFrameRect(currentFrame);
	
	timeLastFrame = SDL_GetTicks();
	
}


void SpriteObject::update() {

	if (speed == 0)
		return;

	float timeSinceLastFrame = SDL_GetTicks() - timeLastFrame;

	/*if(timeSinceLastFrame >= speed) {
		currentFrame++;
	
		if(currentFrame >= (totalFrames) )
			currentFrame = startFrame;

		setFrameRect(currentFrame);

		timeLastFrame = SDL_GetTicks();

	}*/

	//Modificações Fábio

	renderResource = listaObjetos.at(rowNumber);

	if(timeSinceLastFrame >= speed) {
		currentFrame++;
	
		if(currentFrame >= framesPerRow)
			currentFrame = startFrame;

		setFrameRect(currentFrame);

		timeLastFrame = SDL_GetTicks();

	}

}
