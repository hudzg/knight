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
    GuideMenu guideMenu;
    HighScoresMenu highScoresMenu;
    SDL_Cursor* cursor;
    vector <SDL_Rect> traps;
    vector <Door> doors;
    SecretArea secretArea;
    Key key;
    Chest chest;
    Mix_Chunk *menuSound[TOTAL_MENU_SOUND_EFFECT];
    Mix_Chunk *playerSound[TOTAL_PLAYER_SOUND_EFFECT];
    Mix_Chunk *bossSound[TOTAL_BOSS_SOUND_EFFECT];
    Mix_Music *menuTheme, *gameTheme;
    TTF_Font *font[TOTAL_FONT];
    int score;
    vector <int> highScores;
    stringstream scoreText;

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
    bool setHighScores();
    bool loadTexture();
    bool loadSound();
    bool loadFont();
    bool loadMedia();
    bool isRunning();
    int getState();
    void renderScore();
    void updateHighScores();
    void handleMenuEvent(SDL_Event &event);
    void renderMenu();
    void handleGuideMenuEvent(SDL_Event &event);
    void renderGuideMenu();
    void handleHighScoresMenuEvent(SDL_Event &event);
    void renderHighScoresMenu();
    void handlePauseMenuEvent(SDL_Event &event);
    void renderPauseMenu();
    void handleGameOverMenuEvent(SDL_Event &event);
    void renderGameOverMenu();
    void handleWinMenuEvent(SDL_Event &event);
    void renderWinMenu();
    void handleGameEvent(SDL_Event &event);
    void renderGame();
    void close();
};

#endif