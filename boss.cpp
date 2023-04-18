#include "boss.h"

bool Boss::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

bool Boss::checkCollisionWall(SDL_Rect &a, Tile *b)
{
    for (int i = 0; i < TOTAL_TILES; i++)
        if (b[i].getIsWall() && checkCollision(a, b[i].getBox()))
            return true;
    return false;
}

Boss::Boss(float x, float y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    initialX = x;
    initialY = y;
    direction = 1;
    mVelX = BOSS_VEL;
    mVelY = BOSS_VEL * 4;
    mBox = {(int)x, (int)y, BOSS_WIDTH, BOSS_HEIGHT};
    isWalking = isAttacking = isTakeHit = isDeath = false;
    cntIdleFrames = cntWalkFrames = cntAttackFrames = cntTakeHitFrames = cntDeathFrames = 0;
    for (int i = 0, y = 0; i < TOTAL_BOSS_IDLE_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossIdleClips[i] = {0, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    for (int i = 0, y = 0; i < TOTAL_BOSS_WALK_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossWalkClips[i] = {BOSS_TEXTURE_WIDTH, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    for (int i = 0, y = 0; i < TOTAL_BOSS_ATTACK_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossAttackClips[i] = {2 * BOSS_TEXTURE_WIDTH, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    for (int i = 0, y = 0; i < TOTAL_BOSS_TAKE_HIT_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossTakeHitClips[i] = {3 * BOSS_TEXTURE_WIDTH, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    for (int i = 0, y = 0; i < TOTAL_BOSS_DEATH_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossDeathClips[i] = {4 * BOSS_TEXTURE_WIDTH, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    flip = SDL_FLIP_HORIZONTAL;
    cntIdle = 0;
    HP = BOSS_INITIAL_HP;
    isDied = false;

    notTakeHit = false;
    cntAttacked = cntNotTakeHit = 0;
}

void Boss::move(Tile *tiles, const SDL_Rect &playerBox, double timeStep)
{
    // printf("%f %f\n", mPosX, mPosY);
    if (isDied)
        return;
    
    if(notTakeHit)
    {
        cntNotTakeHit++;
        if(cntNotTakeHit == MAX_BOSS_NOT_TAKE_HIT)
        {
            cntNotTakeHit = 0;
            notTakeHit = false;
        }
    }

    if (isTakeHit)
    {
        if (mVelX != 0)
        {
            isWalking = true;
            if (mVelX < 0)
            {
                direction = -1;
                flip = SDL_FLIP_NONE;
            }
            else
            {
                direction = 1;
                flip = SDL_FLIP_HORIZONTAL;
            }
        }
        else
            isWalking = false;
        return;
    }

    if (isAttacking || isDeath)
        return;
    // if(isDeath) return;

    SDL_Rect attackBox = {mPosX - MAX_ATTACK_WIDTH, mPosY, MAX_ATTACK_WIDTH * 2 + BOSS_WIDTH, BOSS_HEIGHT};

    if (checkCollision(attackBox, playerBox))
    {
        // printf("%d %f\n", playerBox.x, mPosX);
        if (playerBox.x < mPosX)
        {
            direction = -1;
            flip = SDL_FLIP_NONE;
        }
        else
        {
            // printf("h\n");
            direction = 1;
            flip = SDL_FLIP_HORIZONTAL;
        }
        isAttacking = true;
    }
    else
    {
        SDL_Rect chasingBox = {initialX - MAX_CHASE_WIDTH, mPosY, MAX_CHASE_WIDTH * 2 + BOSS_WIDTH, BOSS_HEIGHT};
        // if (initialX - MAX_CHASE_WIDTH <= playerBox.x && playerBox.x <= initialX + MAX_CHASE_WIDTH)
        if (checkCollision(chasingBox, playerBox))
        {
            isChasing = true;
            if (playerBox.x >= mPosX)
                mVelX = 1.5 * BOSS_VEL;
            else
                mVelX = -1.5 * BOSS_VEL;
        }
        else
        {
            // mVelX = SKELETON_VEL;
            isChasing = false;
        }
    }

    if (isAttacking)
        return;

    if (mVelX != 0)
    {
        isWalking = true;
        if (mVelX < 0)
        {
            direction = -1;
            flip = SDL_FLIP_NONE;
        }
        else
        {
            direction = 1;
            flip = SDL_FLIP_HORIZONTAL;
        }
    }
    else
        isWalking = false;

    if (isTakeHit)
        return;

    mPosX += mVelX * timeStep;
    mBox.x = mPosX;
    if (mPosX < 0 || mPosX + BOSS_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles) || mPosX < initialX - MAX_WALK_WIDTH || mPosX > initialX + MAX_WALK_WIDTH)
    {
        mPosX -= mVelX * timeStep;
        mVelX = 0;
        direction *= -1;
    }

    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + BOSS_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles))
    {
        mPosY -= mVelY * timeStep;
    }
    else
        mBox.x = mPosX;
    mBox.y = mPosY;
}

void Boss::render(RenderWindow &window, SDL_Rect &camera)
{
    if (isDied || !checkCollision(mBox, camera))
        return;
    SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_RENDER_WIDTH / 2, mBox.y - BOSS_RENDER_HEIGHT + BOSS_HEIGHT, BOSS_RENDER_WIDTH, BOSS_RENDER_HEIGHT};
    SDL_Rect tmpBox2 = tmpBox;
    tmpBox2.x -= camera.x;
    tmpBox2.y -= camera.y;
    window.renderBox(tmpBox2);
    if (isDeath)
    {
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossDeathClips[cntDeathFrames / 8], 0.0, NULL, flip);
        cntDeathFrames++;
        if (cntDeathFrames >= TOTAL_BOSS_DEATH_SPRITES * 8)
        {
            isDied = true;
        }
        return;
    }
    if (isTakeHit)
    {
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossTakeHitClips[cntTakeHitFrames / 6], 0.0, NULL, flip);
        cntTakeHitFrames++;
        if (cntTakeHitFrames >= TOTAL_BOSS_TAKE_HIT_SPRITES * 6)
        {
            isTakeHit = false;
            cntTakeHitFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntAttackFrames = 0;
        isAttacking = false;
        return;
    }
    if (isAttacking)
    {
        // SDL_Rect tmpBox = {mBox.x, mBox.y - mBox.h * 0.12, mBox.w * 1.8, mBox.h * 1.12};
        // if (flip != SDL_FLIP_NONE)
        //     tmpBox.x -= mBox.w * 0.8;
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossAttackClips[cntAttackFrames / 6], 0.0, NULL, flip);
        cntAttackFrames++;
        if (cntAttackFrames >= TOTAL_BOSS_ATTACK_SPRITES * 6)
        {
            isAttacking = false;
            cntAttackFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntTakeHitFrames = 0;
        return;
    }
    if (isWalking)
    {
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossWalkClips[cntWalkFrames / 4], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_BOSS_WALK_SPRITES * 4)
            cntWalkFrames = 0;
        cntIdleFrames = cntAttackFrames = cntTakeHitFrames = 0;
        return;
    }
    // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
    window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossIdleClips[cntIdleFrames / 8], 0.0, NULL, flip);
    cntIdleFrames++;
    if (cntIdleFrames >= TOTAL_BOSS_IDLE_SPRITES * 8)
    {
        cntIdleFrames = 0;
        cntIdle++;
        if (cntIdle >= MAX_IDLE_FRAMES)
        {
            cntIdle = 0;
            mVelX = direction * BOSS_VEL;
        }
    }
    cntWalkFrames = cntAttackFrames = cntTakeHitFrames = 0;
}

std::pair <int, int> Boss::getAttack(SDL_Rect playerBox)
{
    // printf("%d\n", direction);
    if (isDied || !isAttacking || cntAttackFrames != 60)
        return make_pair(0, 0);
    SDL_Rect attackBox = {mBox.x + mBox.w, mBox.y - mBox.h * 0.12, mBox.w * 0.8, mBox.h * 1.12};
    if (flip != SDL_FLIP_NONE)
        attackBox.x = mBox.x - mBox.w * 0.8;
    if (checkCollision(attackBox, playerBox))
        return make_pair(1, direction);
    return make_pair(0, 0);
}

void Boss::attacked(const SDL_Rect &playerAttackRect)
{
    if (isDeath || isDied || notTakeHit)
        return;
    if (checkCollision(mBox, playerAttackRect))
    {
        HP--;
        cntAttacked++;
        if(cntAttacked >= MAX_BOSS_ATTACKED)
        {
            cntAttacked = 0;
            notTakeHit = true;
            cntNotTakeHit = 0;
        }
        if (HP == 0)
        {
            isDeath = true;
        }
        isTakeHit = true;
        if ((playerAttackRect.x * 2 + playerAttackRect.w) / 2 >= (mBox.x * 2 + mBox.w) / 2)
            mVelX = BOSS_VEL;
        else
            mVelX = -BOSS_VEL;
    }
}

int Boss::getPosX()
{
    return mPosX;
}

int Boss::getPosY()
{
    return mPosY;
}