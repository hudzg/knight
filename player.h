#if !defined(PLAYER_H)
#define PLAYER_H

#include <SDL.h>
#include "tile.h"
#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "skeleton.h"
#include "boss.h"

using namespace std;

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
    static const int FIRE_ATTACK_REAL_WIDTH = 300;
    static const int FIRE_ATTACK_REAL_HEIGHT = 112;
    FireAttack(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL);
    void attack(RenderWindow &window, int x, int y, SDL_Rect mBox, SDL_RendererFlip flip);
};

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
};

class Player : public Entity
{
private:
    static const int TOTAL_PLAYER_WALK_SPRITES = 6;
    static const int TOTAL_PLAYER_IDLE_SPRITES = 4;
    static const int TOTAL_PLAYER_JUMP_SPRITES = 5;
    static const int TOTAL_PLAYER_FALL_SPRITES = 5;
    static const int TOTAL_PLAYER_ATTACK_SPRITES = 8;
    static const int TOTAL_PLAYER_DASH_SPRITES = 5;
    static const int TOTAL_PLAYER_TAKE_HIT_SPRITES = 4;
    double mVelX, mVelY;
    SDL_Rect mBox;
    bool onGround, isWalking, isJumping, isAttacking, isDashing, isTakeHit;
    int cntJump;
    int cntWalkFrames, cntIdleFrames, cntJumpFrames, cntFallFrames, cntAttackFrames, cntDashFrames, cntTakeHitFrames;
    int direction;
    SDL_Rect gPlayerIdleClips[TOTAL_PLAYER_IDLE_SPRITES];
    SDL_Rect gPlayerWalkClips[TOTAL_PLAYER_WALK_SPRITES];
    SDL_Rect gPlayerJumpClips[TOTAL_PLAYER_JUMP_SPRITES];
    SDL_Rect gPlayerFallClips[TOTAL_PLAYER_FALL_SPRITES];
    SDL_Rect gPlayerAttackClips[TOTAL_PLAYER_ATTACK_SPRITES];
    SDL_Rect gPlayerDashClips[TOTAL_PLAYER_DASH_SPRITES];
    SDL_Rect gPlayerTakeHitClips[TOTAL_PLAYER_TAKE_HIT_SPRITES];
    SDL_RendererFlip flip;
    FireAttack fireAttackAnimation;
    HealthPoint HP;

public:
    static const int PLAYER_WIDTH = 128;
    static const int PLAYER_HEIGHT = 128;
    static const int PLAYER_TEXTURE_WIDTH = 320;
    static const int PLAYER_TEXTURE_HEIGHT = 320;
    static const int PLAYER_VEL = 600;
    static const int PLAYER_MAX_CNT_JUMP = 20;
    static const int PLAYER_DASH_VEL_LEVEL = 2;
    static const int PLAYER_JUMP_VEL_LEVEL = 3;
    static const int GRAVITY_SPEED = 120;
    static const int MAX_FALL_SPEED = 1200;

    Player(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL, SDL_Texture *mFireAttackTexture = NULL, SDL_Texture *mHPTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    void handleEvent(SDL_Event &e);
    void move(Tile *tiles = NULL, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera, SkeletonFamily &skeletonFamily, Boss &boss);
    void attacked(std::pair <int, int> value);
    void setCamera(SDL_Rect &camera);
    int getPosX();
    int getPosY();
    SDL_Rect getBox();
};

#endif // DOT_H