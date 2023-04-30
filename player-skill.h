#ifndef PLAYER_SKILL_H
#define PLAYER_SKILL_H

#include "utils.h"
#include "entity.h"

class FireAttack : public Entity
{
private:
    static const int TOTAL_ATTACK_SPRITES = 19;
    SDL_Rect gClips[TOTAL_ATTACK_SPRITES];
    int cntFrames;

public:
    static const int FIRE_ATTACK_WIDTH = 453;
    static const int FIRE_ATTACK_HEIGHT = 168;
    static const int FIRE_ATTACK_TEXTURE_WIDTH = 302;
    static const int FIRE_ATTACK_TEXTURE_HEIGHT = 112;
    static const int FIRE_ATTACK_REAL_WIDTH = 280;
    static const int FIRE_ATTACK_REAL_HEIGHT = 112;
    FireAttack(SDL_Texture *mTexture = NULL);
    void render(RenderWindow &window, int x, int y, SDL_Rect mBox, SDL_RendererFlip flip);
};

class HammerGodSkill : public Entity
{
private:
    static const int TOTAL_ATTACK_SPRITES = 11;
    SDL_Rect gClips[TOTAL_ATTACK_SPRITES];
    int cntFrames;
    int x, y;
    SDL_RendererFlip flip;
    SDL_Rect mBox;

public:
    static const int HAMMER_GOD_SKILL_WIDTH = 453;
    static const int HAMMER_GOD_SKILL_HEIGHT = 168;
    static const int HAMMER_GOD_SKILL_TEXTURE_WIDTH = 576;
    static const int HAMMER_GOD_SKILL_TEXTURE_HEIGHT = 320;
    static const int HAMMER_GOD_SKILL_RENDER_WIDTH = 576 * 3 / 3;
    static const int HAMMER_GOD_SKILL_RENDER_HEIGHT = 320 * 3 / 3;
    HammerGodSkill(SDL_Texture *mTexture = NULL);
    void setPos(int x, int y, SDL_RendererFlip flip);
    // void render(RenderWindow &window, int x, int y, SDL_Rect mBox, SDL_RendererFlip flip);
    void render(RenderWindow &window, SDL_Rect &camera);
    SDL_Rect getAttackBox();
    void Reset();
};

#endif