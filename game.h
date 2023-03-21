#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>
#include <set>

#include "texture.h"
#include "tile.h"
#include "constants.h"
#include "player.h"
#include "utils.h"

class Game
{
private:
    RenderWindow window;
    SDL_Texture *gTexture[TOTAL_TEXTURE];
    Tile tiles[TOTAL_TILES];
    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
    SDL_Rect camera;
    bool running;
    Player player;

public:
    bool init();
    bool setTiles(Tile *tiles);
    bool setPlayer();
    bool loadMedia();
    bool isRunning();
    void handleEvent(SDL_Event &event);
    void renderGame();
    void close();
};

#endif