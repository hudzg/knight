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
    double mVelX, mVelY;
    SDL_Rect mBox;
    bool isWalking;
    int cntIdleFrames;
    int direction;
    SDL_Rect gSkeletonIdleClips[TOTAL_SKELETON_IDLE_SPRITES];
    SDL_RendererFlip flip;

public:
    static const int SKELETON_WIDTH = 96;
    static const int SKELETON_HEIGHT = 128;
    static const int SKELETON_TEXTURE_WIDTH = 240;
    static const int SKELETON_TEXTURE_HEIGHT = 320;
    static const int SKELETON_VEL = 300;

    Skeleton(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    void move(Tile *tiles = NULL, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera);
    int getPosX();
    int getPosY();
};

class SkeletonFamily
{
    private:
    const static int TOTAL_SKELETON = 8;
    Skeleton skeleton[TOTAL_SKELETON];
    public:
    SkeletonFamily(SDL_Texture *mTexture = NULL);
    void move(Tile *tiles = NULL);
    void render(RenderWindow &window, SDL_Rect &camera);
};

#endif