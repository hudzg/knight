#if !defined(DOT_H)
#define DOT_H

#include <SDL.h>
#include "tile.h"
#include "constants.h"
#include "entity.h"
#include "utils.h"

using namespace std;

class Dot : public Entity
{
private:
    double mVelX, mVelY;
    SDL_Rect mBox;
    bool onGround;
    int cntJump;

public:
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_VEL = 600;
    static const int DOT_MAX_CNT_JUMP = 20;

    // void shiftColliders();
    Dot(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    void handleEvent(SDL_Event &e);
    void move(Tile *tiles = NULL, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera);
    void setCamera(SDL_Rect &camera);
    int getPosX();
    int getPosY();
};

#endif // DOT_H
