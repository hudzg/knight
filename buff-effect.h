#ifndef BUFF_EFFECT_H
#define BUFF_EFFECT_H
#include "utils.h"

class BuffEffect
{
protected:
    SDL_Rect mBox;
    SDL_Texture *mTexture;
    int cntFrames;
    vector<SDL_Rect> gClips;
    bool isBuff;
    bool isEnd;

public:
    BuffEffect();
    void render(RenderWindow &window, SDL_Rect &camera, SDL_Rect &playerBox, SDL_RendererFlip flip);
    void setIsBuff();
    bool checkIsEnd();
};

class HPBuff : public BuffEffect
{
public:
    static const int TOTAL_HP_BUFF_SPRITES = 17;
    static const int HP_BUFF_WIDTH = 112 * 2;
    static const int HP_BUFF_HEIGHT = 112 * 2;
    static const int HP_BUFF_TEXTURE_WIDTH = 112 * 2;
    static const int HP_BUFF_TEXTURE_HEIGHT = 112 * 2;

    HPBuff(SDL_Texture *mTexture = NULL);
};

class ATKBuff : public BuffEffect
{
public:
    static const int TOTAL_ATK_BUFF_SPRITES = 13;
    static const int ATK_BUFF_WIDTH = 112 * 2;
    static const int ATK_BUFF_HEIGHT = 112 * 2;
    static const int ATK_BUFF_TEXTURE_WIDTH = 112 * 2;
    static const int ATK_BUFF_TEXTURE_HEIGHT = 112 * 2;

    ATKBuff(SDL_Texture *mTexture = NULL);
};

class SkillUnlock : public BuffEffect
{
public:
    static const int TOTAL_SKILL_UNLOCK_SPRITES = 12;
    static const int SKILL_UNLOCK_WIDTH = 144 * 1.5;
    static const int SKILL_UNLOCK_HEIGHT = 144 * 1.5;
    static const int SKILL_UNLOCK_TEXTURE_WIDTH = 144 * 2;
    static const int SKILL_UNLOCK_TEXTURE_HEIGHT = 144 * 2;

    SkillUnlock(SDL_Texture *mTexture = NULL);
};

#endif