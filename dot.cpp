#include "dot.h"

// void Dot::shiftColliders()
// {
//     int row = 0;
//     for (int i = 0; i < mColliders.size(); i++)
//     {
//         mColliders[i].x = mPosX + (DOT_WIDTH - mColliders[i].w) / 2;
//         mColliders[i].y = mPosY + row;
//         row += mColliders[i].h;
//     }
// }

// vector<SDL_Rect> &getColliders()
// {
//     return mColliders;
// }

Dot::Dot(float x, float y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    cntJump = 0;
    mVelX = 0;
    mVelY = DOT_VEL;
    mBox = {0, 0, DOT_WIDTH, DOT_HEIGHT};
    onGround = false;
}

bool Dot::checkCollision(SDL_Rect &a, const SDL_Rect &b)
{
    int x, y, u, v;
    x = std::max(a.x, b.x);
    y = std::max(a.y, b.y);
    u = std::min(a.x + a.w, b.x + b.w);
    v = std::min(a.y + a.h, b.y + b.h);
    if (x < u && y < v)
        return true;
    return false;
}

bool Dot::checkCollisionWall(SDL_Rect &a, Tile *b)
{
    for (int i = 0; i < TOTAL_TILES; i++)
        if (b[i].getType() >= TILE_CENTER && b[i].getType() <= TILE_TOPLEFT && checkCollision(a, b[i].getBox()))
            return true;
    return false;
}

void Dot::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_SPACE:
            if (onGround)
            {
                // cnt++;
                mVelY = -DOT_VEL;
                onGround = false;
            }
            break;
        case SDLK_a:
            mVelX -= DOT_VEL;
            break;
        case SDLK_d:
            mVelX += DOT_VEL;
            break;
        }
    }
    else if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
            mVelX += DOT_VEL;
            break;
        case SDLK_d:
            mVelX -= DOT_VEL;
            break;
        }
    }
}

void Dot::move(Tile *tiles, double timeStep)
{
    if (mVelY < 0)
    {
        cntJump++;
        if (cntJump >= DOT_MAX_CNT_JUMP)
        {
            cntJump = 0;
            mVelY *= -1;
        }
    }
    // printf("%d\n", cnt);
    // onGround = false;
    mPosX += mVelX * timeStep;
    mBox.x = mPosX;
    // shiftColliders();
    if (mPosX < 0 || mPosX + DOT_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles))
    {
        mPosX -= mVelX * timeStep;
        // changeDirectionX();
    }
    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + DOT_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles))
    {
        onGround = true;
        mPosY -= mVelY * timeStep;
        // changeDirectionY();
    }
    mBox.x = mPosX;
    mBox.y = mPosY;
    // shiftColliders();
}

void Dot::render(RenderWindow &window, SDL_Rect &camera)
{
    window.render(getTexture(), mPosX - camera.x, mPosY - camera.y, mBox);
}

void Dot::setCamera(SDL_Rect &camera)
{
    camera.x = (mPosX + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mPosY + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    camera.x = max(camera.x, 0);
    camera.x = min(camera.x, LEVEL_WIDTH - SCREEN_WIDTH);
    camera.y = max(camera.y, 0);
    camera.y = min(camera.y, LEVEL_HEIGHT - SCREEN_HEIGHT);
}

int Dot::getPosX()
{
    return mPosX;
}

int Dot::getPosY()
{
    return mPosY;
}