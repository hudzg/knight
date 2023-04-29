#ifndef FIREBALL_H
#define FIREBALL_H

#include <SDL.h>
#include "utils.h"
#include "constants.h"
#include <vector>
// #include <random>
// #include <chrono>

class FireBall
{
private:
    static const int TOTAL_FIRE_BALL_SPRITES = 4;
    SDL_Rect gClips[TOTAL_FIRE_BALL_SPRITES];
    int cntFrames;
    SDL_Texture *mTexture;
    float mPosX, mPosY, mVelY;
    SDL_Rect mBox;

public:
    static const int FIRE_BALL_WIDTH = 50;
    static const int FIRE_BALL_HEIGHT = 64;
    static const int FIRE_BALL_TEXTURE_WIDTH = 64;
    static const int FIRE_BALL_TEXTURE_HEIGHT = 64;
    static const int FIRE_BALL_RENDER_WIDTH = 64;
    static const int FIRE_BALL_RENDER_HEIGHT = 64;
    static const int GROUND_HEIGHT = 64 * 2;
    static const int GRAVITY_SPEED = 30;
    static const int MAX_FALL_SPEED = 600;

    FireBall(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void move(double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera);
    std::pair<int, int> getAttack(SDL_Rect &playerBox);
    bool onGround();
};

class FireRain
{
private:
    // static const int TOTAL_SKELETON = 2;
    std::vector<FireBall> fireBalls;
    SDL_Texture *mTexture;
    int timeInsert;

public:
    // std::mt19937 rd(std::chrono::_V2::steady_clock::now().time_since_epoch().count());
    long long Rand(long long l, long long r)
    {
        std::mt19937 rd(std::chrono::_V2::steady_clock::now().time_since_epoch().count());
        return l + (1ll * (rd() % MOD) * (rd() % MOD)) % (r - l + 1);
    }
    static const int MAX_TIME_INSERT = 3;
    FireRain(SDL_Texture *mTexture = NULL);
    void insert();
    void move();
    void render(RenderWindow &window, SDL_Rect &camera);
    void checkOnGround();
    std::pair<int, int> getCountAttack(SDL_Rect &playerBox);
};

#endif