#ifndef SKELETON_H
#define SKELETON_H

#include <SDL.h>
#include "tile.h"
#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "door.h"
#include <vector>
#include "secret-area.h"

class Skeleton : public Entity
{
private:
    static const int TOTAL_SKELETON_IDLE_SPRITES = 11;
    static const int TOTAL_SKELETON_WALK_SPRITES = 13;
    static const int TOTAL_SKELETON_ATTACK_SPRITES = 13;
    static const int TOTAL_SKELETON_TAKE_HIT_SPRITES = 8;
    static const int TOTAL_SKELETON_DEATH_SPRITES = 15;
    double mVelX, mVelY;
    float initialX, initialY;
    SDL_Rect mBox;
    bool isWalking, isAttacking, isChasing, isTakeHit, isDeath;
    int cntIdleFrames, cntWalkFrames, cntAttackFrames, cntTakeHitFrames, cntDeathFrames;
    int direction;
    SDL_Rect gSkeletonIdleClips[TOTAL_SKELETON_IDLE_SPRITES];
    SDL_Rect gSkeletonWalkClips[TOTAL_SKELETON_WALK_SPRITES];
    SDL_Rect gSkeletonAttackClips[TOTAL_SKELETON_ATTACK_SPRITES];
    SDL_Rect gSkeletonTakeHitClips[TOTAL_SKELETON_TAKE_HIT_SPRITES];
    SDL_Rect gSkeletonDeathClips[TOTAL_SKELETON_DEATH_SPRITES];
    SDL_RendererFlip flip;
    int cntIdle;
    int HP;
    int ID;
    bool isDied;

public:
    static const int SKELETON_WIDTH = 96;
    static const int SKELETON_HEIGHT = 128;
    static const int SKELETON_TEXTURE_WIDTH = 240;
    static const int SKELETON_TEXTURE_HEIGHT = 330;
    static const int SKELETON_VEL = 150;

    static const int MAX_WALK_WIDTH = 320;
    static const int MAX_CHASE_WIDTH = 320;
    static const int MAX_IDLE_FRAMES = 4;
    static const int MAX_ATTACK_WIDTH = 4;

    static const int SKELETON_INITIAL_HP = 8;
    // static const int SKELETON_INITIAL_HP = 1;

    static const int SKELETON_IS_WALL = 240;

    Skeleton(float mPosX = 0, float mPosY = 0, int ID = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    bool checkCollisionDoor(vector <Door> &doors);
    void move(Tile *tiles, const SDL_Rect &playerBox, vector <Door> &doors, SecretArea &secretArea, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera);
    int attacked(const SDL_Rect &playerAttackRect, int damage, int &score);
    std::pair <int, int> getAttack(SDL_Rect playerBox);
    int getPosX();
    int getPosY();
    bool getDied();
};

class SkeletonFamily
{
    private:
    // static const int TOTAL_SKELETON = 2;
    std::vector <Skeleton> skeleton;
    int cntSkeletonDied;
    public:
    SkeletonFamily(SDL_Texture *mTexture = NULL);
    void move(Tile *tiles, const SDL_Rect &playerBox, vector <Door> &doors, SecretArea &secretArea);
    void render(RenderWindow &window, SDL_Rect &camera);
    int attacked(const SDL_Rect &playerAttackRect, int damage, int &score);
    void checkDied();
    std::pair <int, int> getCountAttack(SDL_Rect playerBox);
    int getCntSkeletonDied();
};

#endif