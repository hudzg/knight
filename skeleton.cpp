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
    isWalking = isAttacking = isTakeHit = isDeath = false;
    cntIdleFrames = cntWalkFrames = cntAttackFrames = cntTakeHitFrames = cntDeathFrames = 0;
    for (int i = 0, x = 0; i < TOTAL_SKELETON_IDLE_SPRITES; i++, x += 240)
        gSkeletonIdleClips[i] = {x, 0, SKELETON_TEXTURE_WIDTH, SKELETON_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_SKELETON_ATTACK_SPRITES; i++, x += 430)
        gSkeletonAttackClips[i] = {x, 320, 430, 370};
    for (int i = 0, x = 0; i < TOTAL_SKELETON_WALK_SPRITES; i++, x += 220)
        gSkeletonWalkClips[i] = {x, 1650, 220, 330};
    for (int i = 0, x = 0; i < TOTAL_SKELETON_TAKE_HIT_SPRITES; i++, x += 300)
        gSkeletonTakeHitClips[i] = {x, 1010, 300, 320};
    for (int i = 0, x = 0; i < TOTAL_SKELETON_DEATH_SPRITES; i++, x += 330)
        gSkeletonDeathClips[i] = {x, 690, 330, 320};
    flip = SDL_FLIP_NONE;
    cntIdle = 0;
    HP = SKELETON_INITIAL_HP;
    isDied = false;
}

void Skeleton::move(Tile *tiles, const SDL_Rect &playerBox, double timeStep)
{
    if(isDied || isDeath) return;
    // if (mPosX - MAX_ATTACK_WIDTH <= playerBox.x && playerBox.x <= mPosX + MAX_ATTACK_WIDTH)
    // {
    //     isAttacking = true;
    // }
    // else
    // {
    //     if (initialX - MAX_CHASE_WIDTH <= playerBox.x && playerBox.x <= initialX + MAX_CHASE_WIDTH)
    //     {
    //         isChasing = true;
    //         if (playerBox.x >= mPosX)
    //             mVelX = 2 * SKELETON_VEL;
    //         else
    //             mVelX = -2 * SKELETON_VEL;
    //     }
    //     else
    //     {
    //         // mVelX = SKELETON_VEL;
    //         isChasing = false;
    //     }
    // }
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

    if(isTakeHit) return;

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
    if (isDied || !checkCollision(mBox, camera))
        return;
    if(isDeath)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y, 1.0 * mBox.w * 330 / 240, 1.0 * mBox.h * 320 / 330};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gSkeletonDeathClips[cntDeathFrames / 8], 0.0, NULL, flip);
        cntDeathFrames++;
        if (cntDeathFrames >= TOTAL_SKELETON_DEATH_SPRITES * 8)
        {
            isDied = true;
        }
        return;
    }
    if (isTakeHit)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y, mBox.w * 1.25, mBox.h};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gSkeletonTakeHitClips[cntTakeHitFrames / 8], 0.0, NULL, flip);
        cntTakeHitFrames++;
        if (cntTakeHitFrames >= TOTAL_SKELETON_TAKE_HIT_SPRITES * 8)
        {
            isTakeHit = false;
            cntTakeHitFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntAttackFrames = 0;
        return;
    }
    if (isAttacking)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y - mBox.h * 0.12, mBox.w * 1.8, mBox.h * 1.12};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gSkeletonAttackClips[cntAttackFrames / 8], 0.0, NULL, flip);
        cntAttackFrames++;
        if (cntAttackFrames >= TOTAL_SKELETON_ATTACK_SPRITES * 8)
        {
            isAttacking = false;
            cntAttackFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntTakeHitFrames = 0;
        return;
    }
    if (isWalking)
    {
        window.renderPlayer(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &gSkeletonWalkClips[cntWalkFrames / 4], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_SKELETON_WALK_SPRITES * 4)
            cntWalkFrames = 0;
        cntIdleFrames = cntAttackFrames = cntTakeHitFrames = 0;
        return;
    }
    window.renderPlayer(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &gSkeletonIdleClips[cntIdleFrames / 4], 0.0, NULL, flip);
    cntIdleFrames++;
    if (cntIdleFrames >= TOTAL_SKELETON_IDLE_SPRITES * 4)
    {
        cntIdleFrames = 0;
        cntIdle++;
        if (cntIdle >= MAX_IDLE_FRAMES)
        {
            cntIdle = 0;
            mVelX = direction * SKELETON_VEL;
        }
    }
    cntWalkFrames = cntAttackFrames = cntTakeHitFrames = 0;
}

void Skeleton::attack(const SDL_Rect &playerAttackRect)
{
    if(isDeath || isDied) return;
    if(checkCollision(mBox, playerAttackRect)) 
    {
        HP--;
        if(HP == 0)
        {
            isDeath = true;
        }
        isTakeHit = true;
        if((playerAttackRect.x * 2 + playerAttackRect.w) / 2 >= (mBox.x * 2 + mBox.w) / 2)
            mVelX = SKELETON_VEL;
        else mVelX = -SKELETON_VEL;
    }
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

void SkeletonFamily::move(Tile *tiles, const SDL_Rect &playerBox)
{
    for (int i = 0; i < TOTAL_SKELETON; i++)
        skeleton[i].move(tiles, playerBox);
}

void SkeletonFamily::render(RenderWindow &window, SDL_Rect &camera)
{
    for (int i = 0; i < TOTAL_SKELETON; i++)
        skeleton[i].render(window, camera);
}

void SkeletonFamily::attack(const SDL_Rect &playerAttackRect)
{
    for (int i = 0; i < TOTAL_SKELETON; i++)
        skeleton[i].attack(playerAttackRect);
}