#ifndef BOSS_H
#define BOSS_H

#include <SDL.h>
#include "tile.h"
#include "constants.h"
#include "entity.h"
#include "utils.h"

class Boss : public Entity
{
private:
    static const int TOTAL_BOSS_IDLE_SPRITES = 6;
    static const int TOTAL_BOSS_WALK_SPRITES = 12;
    static const int TOTAL_BOSS_ATTACK_SPRITES = 15;
    static const int TOTAL_BOSS_TAKE_HIT_SPRITES = 5;
    static const int TOTAL_BOSS_DEATH_SPRITES = 22;
    double mVelX, mVelY;
    float initialX, initialY;
    SDL_Rect mBox;
    bool isWalking, isAttacking, isChasing, isTakeHit, isDeath;
    int cntIdleFrames, cntWalkFrames, cntAttackFrames, cntTakeHitFrames, cntDeathFrames;
    int direction;
    SDL_Rect gBossIdleClips[TOTAL_BOSS_IDLE_SPRITES];
    SDL_Rect gBossWalkClips[TOTAL_BOSS_WALK_SPRITES];
    SDL_Rect gBossAttackClips[TOTAL_BOSS_ATTACK_SPRITES];
    SDL_Rect gBossTakeHitClips[TOTAL_BOSS_TAKE_HIT_SPRITES];
    SDL_Rect gBossDeathClips[TOTAL_BOSS_DEATH_SPRITES];
    SDL_RendererFlip flip;
    int cntIdle;
    int HP;
    bool isDied;

    static const int MAX_BOSS_ATTACKED = 2;
    static const int MAX_BOSS_NOT_TAKE_HIT = 240;
    bool notTakeHit;
    int cntNotTakeHit, cntAttacked;

public:
    static const int BOSS_WIDTH = 192;
    static const int BOSS_HEIGHT = 224;
    static const int BOSS_TEXTURE_WIDTH = 576;
    static const int BOSS_TEXTURE_HEIGHT = 320;
    static const int BOSS_RENDER_WIDTH = 864;
    static const int BOSS_RENDER_HEIGHT = 480;
    static const int BOSS_VEL = 150;

    static const int MAX_WALK_WIDTH = 320;
    static const int MAX_CHASE_WIDTH = 320;
    static const int MAX_IDLE_FRAMES = 4;
    static const int MAX_ATTACK_WIDTH = 64;

    static const int BOSS_INITIAL_HP = 40;

    Boss(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    void move(Tile *tiles, const SDL_Rect &playerBox, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera);
    void attacked(const SDL_Rect &playerAttackRect);
    std::pair <int, int> getAttack(SDL_Rect playerBox);
    int getPosX();
    int getPosY();
};

#endif