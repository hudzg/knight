#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "constants.h"
#include "entity.h"
#include "utils.h"

class Menu
{
private:
    enum MenuTexture
    {
        BACKGROUND_TEXTURE,
        BUTTON_TEXTURE,
        TITLE_TEXTURE,
        TOTAL_TEXTURE
    };
    enum MenuButton
    {
        PLAY,
        GUIDE,
        HIGH,
        QUIT,
        TOTAL_BUTTON
    };
    static const int TOTAL_MENU_TITLE_SPRITES = 7;
    SDL_Texture *gTexture[TOTAL_TEXTURE];
    SDL_Rect backgroundBox, titleBox, buttonBox[TOTAL_BUTTON];
    SDL_Rect titleClips[TOTAL_MENU_TITLE_SPRITES];
    SDL_Rect buttonClips[TOTAL_BUTTON][2];
    int mouseover[TOTAL_BUTTON];
    int cntTitleFrames;

public:
    static const int MENU_TITLE_WIDTH = 1024;
    static const int MENU_TITLE_HEIGHT = 256;
    static const int MENU_TITLE_TEXTURE_WIDTH = 2048;
    static const int MENU_TITLE_TEXTURE_HEIGHT = 512;

    static const int MENU_BUTTON_WIDTH = 256;
    static const int MENU_BUTTON_HEIGHT = 64;
    static const int MENU_BUTTON_TEXTURE_WIDTH = 1024;
    static const int MENU_BUTTON_TEXTURE_HEIGHT = 256;


    Menu(SDL_Texture *backgroundTexture = NULL, SDL_Texture *buttonTexture = NULL, SDL_Texture *titleTexture = NULL);
    void handleEvent(SDL_Event &e, GameState &state, Mix_Chunk *gSound[]);
    void render(RenderWindow &window);
};

class SubMenu
{
private:
    enum MenuTexture
    {
        BACKGROUND_TEXTURE,
        BUTTON_TEXTURE,
        TITLE_TEXTURE,
        TOTAL_TEXTURE
    };
    enum MenuButton
    {
        AGAIN,
        RESUME,
        HOME,
        TOTAL_BUTTON
    };
    SDL_Texture *gTexture[TOTAL_TEXTURE];
    SDL_Rect backgroundBox, titleBox, buttonBox[TOTAL_BUTTON];
    SDL_Rect titleClips;
    SDL_Rect buttonClips[TOTAL_BUTTON][2];
    int haveResume;
    int mouseover[TOTAL_BUTTON];

public:
    static const int MENU_TITLE_WIDTH = 512;
    static const int MENU_TITLE_HEIGHT = 80;
    static const int MENU_TITLE_TEXTURE_WIDTH = 1024;
    static const int MENU_TITLE_TEXTURE_HEIGHT = 160;

    static const int MENU_BUTTON_WIDTH = 64;
    static const int MENU_BUTTON_HEIGHT = 64;
    static const int MENU_BUTTON_TEXTURE_WIDTH = 160;
    static const int MENU_BUTTON_TEXTURE_HEIGHT = 160;


    SubMenu(int y = 0, SDL_Texture *backgroundTexture = NULL, SDL_Texture *buttonTexture = NULL, SDL_Texture *titleTexture = NULL, int haveResume = 0);
    void handleEvent(SDL_Event &e, GameState &state, Mix_Chunk *gSound[]);
    void render(RenderWindow &window, int score, TTF_Font *font[]);
};

class GuideMenu
{
private:
    enum MenuTexture
    {
        BACKGROUND_TEXTURE,
        BUTTON_IMAGES_TEXTURE,
        BUTTON_TEXTURE,
        TOTAL_TEXTURE
    };

    enum MenuButtonImages
    {
        BUTTON_A,
        BUTTON_D,
        BUTTON_SHIFT,
        BUTTON_SPACE,
        BUTTON_LEFT_CLICK,
        BUTTON_RIGHT_CLICK,
        BUTTON_ESCAPE,
        TOTAL_BUTTON_IMAGES
    };

    enum MenuButton
    {
        BACK,
        TOTAL_BUTTON
    };

    SDL_Texture *gTexture[TOTAL_TEXTURE];
    SDL_Rect backgroundBox, buttonImagesBox[TOTAL_BUTTON_IMAGES], buttonBox[TOTAL_BUTTON];
    SDL_Rect buttonImagesClips[TOTAL_BUTTON_IMAGES], buttonClips[TOTAL_BUTTON][2];
    int mouseover[TOTAL_BUTTON];
    string buttonText[TOTAL_BUTTON_IMAGES];

public:

    static const int MENU_BUTTON_IMAGES_WIDTH = 128;
    static const int MENU_BUTTON_IMAGES_HEIGHT = 64;
    static const int MENU_BUTTON_IMAGES_TEXTURE_WIDTH = 128;
    static const int MENU_BUTTON_IMAGES_TEXTURE_HEIGHT = 64;

    static const int MENU_BUTTON_WIDTH = 256;
    static const int MENU_BUTTON_HEIGHT = 64;
    static const int MENU_BUTTON_TEXTURE_WIDTH = 1024;
    static const int MENU_BUTTON_TEXTURE_HEIGHT = 256;


    GuideMenu(SDL_Texture *backgroundTexture = NULL, SDL_Texture *buttonImagesTexture = NULL, SDL_Texture *buttonTexture = NULL);
    void handleEvent(SDL_Event &e, GameState &state, Mix_Chunk *gSound[]);
    void render(RenderWindow &window, TTF_Font *font[]);
};

class HighScoresMenu
{
private:
    enum MenuTexture
    {
        BACKGROUND_TEXTURE,
        BUTTON_TEXTURE,
        TOTAL_TEXTURE
    };

    enum MenuButton
    {
        BACK,
        TOTAL_BUTTON
    };

    SDL_Texture *gTexture[TOTAL_TEXTURE];
    SDL_Rect backgroundBox, buttonBox[TOTAL_BUTTON];
    SDL_Rect buttonClips[TOTAL_BUTTON][2];
    int mouseover[TOTAL_BUTTON];

public:

    static const int MENU_BUTTON_WIDTH = 256;
    static const int MENU_BUTTON_HEIGHT = 64;
    static const int MENU_BUTTON_TEXTURE_WIDTH = 1024;
    static const int MENU_BUTTON_TEXTURE_HEIGHT = 256;


    HighScoresMenu(SDL_Texture *backgroundTexture = NULL, SDL_Texture *buttonTexture = NULL);
    void handleEvent(SDL_Event &e, GameState &state, Mix_Chunk *gSound[]);
    void render(RenderWindow &window, TTF_Font *font[], vector <int> &highScores);
};

#endif