#ifndef HUD_H
#define HUD_H

#include "utils.h"
#include "entity.h"

class HealthPoint : public Entity
{
private:
    static const int TOTAL_HP_SPRITES = 2;
    SDL_Rect gClips[TOTAL_HP_SPRITES];
    int HP;
    SDL_Rect mBox;

public:
    static const int HP_WIDTH = 80;
    static const int HP_HEIGHT = 80;
    static const int HP_TEXTURE_WIDTH = 160;
    static const int HP_TEXTURE_HEIGHT = 160;

    static const int TOTAL_HP = 4;
    static const int HP_POS_X = 80;
    static const int HP_POS_Y = 40;
    HealthPoint(SDL_Texture *mTexture = NULL);
    void render(RenderWindow &window);
    void addHP(int value);
    int getHP();
};

class PlayerPoint
{
protected:
    vector<SDL_Rect> gClips;
    int point;
    SDL_Rect mBox;
    int totalPoint, mPosX, mPosY;
    SDL_Texture *mTexture;
    int TOTAL_POINT_SPRITES;

public:
    static const int PLAYER_POINT_WIDTH = 80;
    static const int PLAYER_POINT_HEIGHT = 80;
    static const int PLAYER_POINT_TEXTURE_WIDTH = 160;
    static const int PLAYER_POINT_TEXTURE_HEIGHT = 160;
    PlayerPoint(int totalPoint = 0, int mPosX = 0, int mPoxY = 0, int TOTAL_POINT_SPRITES = 0, SDL_Texture *mTexture = NULL);
    void render(RenderWindow &window);
    void addPoint(int value);
    int getPoint();
    bool checkFullPoint();
};

#endif