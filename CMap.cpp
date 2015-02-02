//Marcado

#include "CMap.h"

#include "debug.h"

CMap::CMap() {
    Surf_Tileset = NULL;
}

bool CMap::OnLoad(char* File) {
    TileList.clear();

    FILE* FileHandle = fopen(File, "r");

    if(FileHandle == NULL) {
        return false;
    }

    for(int Y = 0;Y < MAP_HEIGHT;Y++) {
        for(int X = 0;X < MAP_WIDTH;X++) {
            CTile tempTile;

            fscanf(FileHandle, "%d:%d ", &tempTile.TileID, &tempTile.TypeID);

            TileList.push_back(tempTile);
        }
        fscanf(FileHandle, "\n");
    }

    fclose(FileHandle);

    return true;
}

void CMap::OnRender(SDL_Surface* Surf_Display, int MapX, int MapY) {
    if(Surf_Tileset == NULL) return;
	//TILE_SIZE é o valor em pixels, TilesetWidth & height é o valor da 
	//área da IMG em tiles. onde Surf_Tileset->w é inicializado?

    int TilesetWidth  = Surf_Tileset->w / TILE_SIZE;
    int TilesetHeight = Surf_Tileset->h / TILE_SIZE;

    int ID = 0;

    for(int Y = 0;Y < MAP_HEIGHT;Y++) {
        for(int X = 0;X < MAP_WIDTH;X++) {
            if(TileList[ID].TypeID == TILE_TYPE_NONE) {
                ID++;
                continue;
            }
			//vejo o x e y do map, a partir dele, defino os x e y de cada tile
			//aqui, é o "where to on the screen"
            int tX = MapX + (X * TILE_SIZE);
            int tY = MapY + (Y * TILE_SIZE);
			//
            int TilesetX = (TileList[ID].TileID % TilesetWidth) * TILE_SIZE;
			//lixão: divisão é truncada. quando resultado = 0,5, o final fica 0
            int TilesetY = (TileList[ID].TileID / TilesetWidth) * TILE_SIZE;

            CSurface::OnDraw(Surf_Display, Surf_Tileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);

            ID++;
        }
    }
}
 

CTile* CMap::GetTile(int X, int Y) {
    int ID = 0;

    ID = X / TILE_SIZE;
    ID = ID + (MAP_WIDTH * (Y / TILE_SIZE));

    if(ID < 0 || ID >= TileList.size()) {
        return NULL;
    }

    return &TileList[ID];
}