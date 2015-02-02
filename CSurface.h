//Marcado

#ifndef _CSURFACE_H_
    #define _CSURFACE_H_

#include <SDL.h>
#include <SDL_image.h>

class CSurface {
    public:
        CSurface();

    public:
		//OnLoad de uma nova superfície qualquer
        static SDL_Surface* OnLoad(char* File);
		//depois de carregada uma sup nova, desenha na sup principal.
		static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
		//mesma da anterior, mas desenha um teco de um BMP maior (e.g. tileset)
		static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
		//aplica transp a uma sup
		static bool Transparent(SDL_Surface* Surf_Dest, int R, int G, int B);
};

#endif