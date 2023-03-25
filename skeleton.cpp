#include "skeleton.h"

bool Skeleton::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

bool Skeleton::checkCollisionWall(SDL_Rect &a, Tile *b)
{
    for (int i = 0; i < TOTAL_TILES; i++)
        if (b[i].getIsWall() && checkCollision(a, b[i].getBox()))
            return true;
    return false;
}

Skeleton::Skeleton(float x, float y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    initialX = x;
    initialY = y;
    direction = 1;
    mVelX = SKELETON_VEL;
    mVelY = SKELETON_VEL * 4;
    mBox = {(int)x, (int)y, SKELETON_WIDTH, SKELETON_HEIGHT};
    isWalking = false;
    cntIdleFrames = cntWalkFrames = 0;
    for (int i = 0, x = 0; i < TOTAL_SKELETON_IDLE_SPRITES; i++, x += 240)
        gSkeletonIdleClips[i] = {x, 0, SKELETON_TEXTURE_WIDTH, SKELETON_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_SKELETON_WALK_SPRITES; i++, x += 220)
        gSkeletonWalkClips[i] = {x, 1650, 220, 330};
    flip = SDL_FLIP_NONE;
    cntIdle = 0;
}

void Skeleton::move(Tile *tiles, double timeStep)
{
    if (mVelX != 0)
    {
        isWalking = true;
        if (mVelX < 0)
        {
            direction = -1;
            flip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            direction = 1;
            flip = SDL_FLIP_NONE;
        }
    }
    else
        isWalking = false;
    mPosX += mVelX * timeStep;
    mBox.x = mPosX;
    if (mPosX < 0 || mPosX + SKELETON_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles) || mPosX < initialX - MAX_WALK_WIDTH || mPosX > initialX + MAX_WALK_WIDTH)
    {
        mPosX -= mVelX * timeStep;
        mVelX = 0;
        direction *= -1;
    }

    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + SKELETON_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles))
    {
        mPosY -= mVelY * timeStep;
    }
    else
        mBox.x = mPosX;
    mBox.y = mPosY;
}

void Skeleton::render(RenderWindow &window, SDL_Rect &camera)
{
    if (!checkCollision(mBox, camera))
        return;
    if (isWalking)
    {
        window.renderPlayer(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &gSkeletonWalkClips[cntWalkFrames / 4], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_SKELETON_WALK_SPRITES * 4)
            cntWalkFrames = 0;
        cntIdleFrames = 0;
        return;
    }
    window.renderPlayer(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &gSkeletonIdleClips[cntIdleFrames / 4], 0.0, NULL, flip);
    cntIdleFrames++;
    if (cntIdleFrames >= TOTAL_SKELETON_IDLE_SPRITES * 4)
    {
        cntIdleFrames = 0;
        cntIdle++;
        if(cntIdle >= MAX_IDLE_FRAMES)
        {
            cntIdle = 0;
            mVelX = direction * SKELETON_VEL;
        }
    }
    cntWalkFrames = 0;
}

int Skeleton::getPosX()
{
    return mPosX;
}

int Skeleton::getPosY()
{
    return mPosY;
}

SkeletonFamily::SkeletonFamily(SDL_Texture *mTexture)
{
    for (int i = 0; i < TOTAL_SKELETON; i++)
        skeleton[i] = Skeleton(rand() % LEVEL_WIDTH, 0, mTexture);
}

void SkeletonFamily::move(Tile *tiles)
{
    for (int i = 0; i < TOTAL_SKELETON; i++)
        skeleton[i].move(tiles);
}

void SkeletonFamily::render(RenderWindow &window, SDL_Rect &camera)
{
    for (int i = 0; i < TOTAL_SKELETON; i++)
        skeleton[i].render(window, camera);
}