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
#include "skeleton.h"
#include "boss.h"
#include "menu.h"
#include "door.h"
#include "secret-area.h"

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
    SkeletonFamily skeletonFamily;
    Boss boss;
    GameState state;
    Menu menu;
    SubMenu pauseMenu, gameOverMenu, winMenu;
    SDL_Cursor* cursor;
    vector <SDL_Rect> traps;
    vector <Door> doors;
    SecretArea secretArea;
    Key key;
    Chest chest;

public:
    bool init();
    bool setTiles(Tile *tiles);
    bool setPlayer();
    bool setSkeleton();
    bool setBoss();
    bool setMenu();
    bool setDoor();
    bool setSecretArea();
    bool setDynamicObject();
    void setCursor();
    bool loadMedia();
    bool isRunning();
    int getState();
    void handleMenuEvent(SDL_Event &event);
    void renderMenu();
    void handlePauseMenuEvent(SDL_Event &event);
    void renderPauseMenu();
    void handleGameOverMenuEvent(SDL_Event &event);
    void renderGameOverMenu();
    void handleGameEvent(SDL_Event &event);
    void renderGame();
    void close();
};

#endif