#ifndef SECRET_AREA_H
#define SECRET_AREA_H

#include <SDL.h>
#include "entity.h"
#include "constants.h"
#include "utils.h"

class SecretArea : public Entity
{
private:
    enum SecretAreaState
    {
        CLOSE,
        OPEN,
        TOTAL_STATE
    };
    SDL_Rect mBox;
    // SDL_Rect clips[TOTAL_STATE];
    SecretAreaState state;
    bool canOpen;

public:
    static const int SECRET_AREA_WIDTH = 576;
    static const int SECRET_AREA_HEIGHT = 320;
    static const int SECRET_AREA_TEXTURE_WIDTH = 576;
    static const int SECRET_AREA_TEXTURE_HEIGHT = 320;
    SecretArea(int x = 0, int y = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void render(RenderWindow &window, SDL_Rect &camera);
    SDL_Rect getBox();
    bool isOpen();
    void setOpen(SDL_Rect &playerBox);
    void setCanOpen();
};

class Key : public Entity
{
private:
    enum KeyState
    {
        HIDDEN,
        NOT_PICK,
        PICK,
        USED,
        TOTAL_STATE
    };
    static const int KEY_VEL = 10;
    int mPosX, mPosY;
    int maxTime, time;
    SDL_Rect mBox;
    // SDL_Rect clips[TOTAL_STATE];
    KeyState state;

public:
    static const int KEY_WIDTH = 64;
    static const int KEY_HEIGHT = 64;
    static const int KEY_TEXTURE_WIDTH = 64;
    static const int KEY_TEXTURE_HEIGHT = 64;
    static const int DSTPOS_X = SCREEN_WIDTH - 2 * KEY_WIDTH;
    static const int DSTPOS_Y = SCREEN_HEIGHT - 2 * KEY_HEIGHT;
    Key(int x = 0, int y = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void move();
    void render(RenderWindow &window, SDL_Rect &camera);
    SDL_Rect getBox();
    // bool isOpen();
    void setNotPick();
    void setPick(SDL_Rect playerBox, SDL_Rect &camera);
    bool isPick();
    void setUsed();
};

class Chest : public Entity
{
private:
    enum ChestState
    {
        HIDDEN,
        CLOSE,
        OPEN,
        TOTAL_STATE
    };
    SDL_Rect mBox;
    SDL_Rect clips[2];
    ChestState state;

public:
    static const int CHEST_WIDTH = 64;
    static const int CHEST_HEIGHT = 64;
    static const int CHEST_TEXTURE_WIDTH = 64;
    static const int CHEST_TEXTURE_HEIGHT = 64;
    Chest(int x = 0, int y = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void render(RenderWindow &window, SDL_Rect &camera);
    SDL_Rect getBox();
    // bool isOpen();
    void setClose();
    bool setOpen(SDL_Rect playerBox);
    // void setUsed();
};

#endif // DOOR_H