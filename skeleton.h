#ifndef SKELETON_H
#define SKELETON_H

#include <SDL.h>
#include "tile.h"
#include "constants.h"
#include "entity.h"
#include "utils.h"

class Skeleton : public Entity
{
private:
    static const int TOTAL_SKELETON_IDLE_SPRITES = 11;
    static const int TOTAL_SKELETON_WALK_SPRITES = 13;
    static const int TOTAL_SKELETON_ATTACK_SPRITES = 13;
    static const int TOTAL_SKELETON_TAKE_HIT_SPRITES = 8;
    double mVelX, mVelY;
    float initialX, initialY;
    SDL_Rect mBox;
    bool isWalking, isAttacking, isChasing, isTakeHit;
    int cntIdleFrames, cntWalkFrames, cntAttackFrames, cntTakeHitFrames;
    int direction;
    SDL_Rect gSkeletonIdleClips[TOTAL_SKELETON_IDLE_SPRITES];
    SDL_Rect gSkeletonWalkClips[TOTAL_SKELETON_WALK_SPRITES];
    SDL_Rect gSkeletonAttackClips[TOTAL_SKELETON_ATTACK_SPRITES];
    SDL_Rect gSkeletonTakeHitClips[TOTAL_SKELETON_TAKE_HIT_SPRITES];
    SDL_RendererFlip flip;
    int cntIdle;

public:
    static const int SKELETON_WIDTH = 96;
    static const int SKELETON_HEIGHT = 128;
    static const int SKELETON_TEXTURE_WIDTH = 240;
    static const int SKELETON_TEXTURE_HEIGHT = 330;
    static const int SKELETON_VEL = 150;

    static const int MAX_WALK_WIDTH = 240;
    static const int MAX_CHASE_WIDTH = 320;
    static const int MAX_IDLE_FRAMES = 4;
    static const int MAX_ATTACK_WIDTH = 32;

    Skeleton(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    void move(Tile *tiles, const SDL_Rect &playerBox, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera);
    void attack(const SDL_Rect &playerAttackRect);
    int getPosX();
    int getPosY();
};

class SkeletonFamily
{
    private:
    static const int TOTAL_SKELETON = 8;
    Skeleton skeleton[TOTAL_SKELETON];
    public:
    SkeletonFamily(SDL_Texture *mTexture = NULL);
    void move(Tile *tiles, const SDL_Rect &playerBox);
    void render(RenderWindow &window, SDL_Rect &camera);
    void attack(const SDL_Rect &playerAttackRect);
};

#endif