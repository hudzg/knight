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

Boss::Boss(float x, float y, SDL_Texture *mTexture, SDL_Texture *fireBallTexture) : Entity(x, y, mTexture)
{
    initialX = x;
    initialY = y;
    direction = 1;
    mVelX = BOSS_VEL;
    mVelY = BOSS_VEL * 4;
    mBox = {(int)x, (int)y, BOSS_WIDTH, BOSS_HEIGHT};
    isWalking = isAttacking = isTakeHit = isDeath = isSmashing = isCasting = false;
    cntIdleFrames = cntWalkFrames = cntAttackFrames = cntTakeHitFrames = cntDeathFrames = cntSmashFrames = cntCastFrames = 0;
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
    for (int i = 0, y = 0; i < TOTAL_BOSS_SMASH_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossSmashClips[i] = {5 * BOSS_TEXTURE_WIDTH, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    for (int i = 0, y = 0; i < TOTAL_BOSS_CAST_SPRITES; i++, y += BOSS_TEXTURE_HEIGHT)
        gBossCastClips[i] = {6 * BOSS_TEXTURE_WIDTH, y, BOSS_TEXTURE_WIDTH, BOSS_TEXTURE_HEIGHT};
    flip = SDL_FLIP_HORIZONTAL;
    cntIdle = 0;
    HP = BOSS_INITIAL_HP;
    isDied = false;

    notTakeHit = false;
    cntAttacked = cntNotTakeHit = 0;
    cntCycle = 0;
    // for (int i = 0; i < MAX_BOSS_CYCLE; i++)
    //     stateAttack[i] = CAST;

    for (int i = 0; i < MAX_BOSS_CYCLE; i++)
        stateAttack[i] = ATTACK;
    stateAttack[3] = SMASH;
    stateAttack[7] = CAST;

    fireRain = FireRain(fireBallTexture);
}

void Boss::move(Tile *tiles, const SDL_Rect &playerBox, double timeStep)
{
    // printf("%f %f\n", mPosX, mPosY);
    fireRain.move();
    fireRain.checkOnGround();

    if (isDied)
        return;

    if (notTakeHit)
    {
        cntNotTakeHit++;
        if (cntNotTakeHit == MAX_BOSS_NOT_TAKE_HIT)
        {
            cntNotTakeHit = 0;
            notTakeHit = false;
        }
    }

    mVelY += GRAVITY_SPEED;
    if (mVelY >= MAX_FALL_SPEED)
        mVelY = MAX_FALL_SPEED;

    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + BOSS_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles))
    {
        mPosY -= mVelY * timeStep;
    }
    mBox.y = mPosY;

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

    if (isCasting || isSmashing || isAttacking || isDeath)
        return;
    // if(isDeath) return;

    SDL_Rect attackBox = {mPosX - MAX_ATTACK_WIDTH, mPosY, MAX_ATTACK_WIDTH * 2 + BOSS_WIDTH, BOSS_HEIGHT};

    if (checkCollision(attackBox, playerBox))
    {
        // printf("%d %f\n", playerBox.x, mPosX);
        if (playerBox.x + playerBox.w / 2 < mPosX + mBox.w / 2)
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
        // isAttacking = true;
        switch (stateAttack[cntCycle])
        {
        case ATTACK:
            isAttacking = true;
            break;
        case SMASH:
            mVelY = -BOSS_VEL * 6;
            isSmashing = true;
            break;
        case CAST:
            isCasting = true;
            break;

        default:
            break;
        }
        cntCycle++;
        if (cntCycle >= MAX_BOSS_CYCLE)
        {
            cntCycle = 0;
            // mVelY = -BOSS_VEL * 6;
            // isSmashing = true;
            // isAttacking = false;
        }
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

    if (isCasting || isSmashing || isAttacking)
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

    // mPosY += mVelY * timeStep;
    // mBox.y = mPosY;
    // if (mPosY < 0 || mPosY + BOSS_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles))
    // {
    //     mPosY -= mVelY * timeStep;
    // }
    mBox.x = mPosX;
    // mBox.y = mPosY;
}

void Boss::render(RenderWindow &window, SDL_Rect &camera, Mix_Chunk *sound[])
{

    fireRain.render(window, camera);

    if (isDied)
        return;
    SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_RENDER_WIDTH / 2, mBox.y - BOSS_RENDER_HEIGHT + BOSS_HEIGHT, BOSS_RENDER_WIDTH, BOSS_RENDER_HEIGHT};
    // SDL_Rect tmpBox2 = tmpBox;
    // tmpBox2.x -= camera.x;
    // tmpBox2.y -= camera.y;
    // window.renderBox(tmpBox2);
    if (isDeath)
    {
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossDeathClips[cntDeathFrames / 8], 0.0, NULL, flip);
        cntDeathFrames++;
        if (cntDeathFrames >= TOTAL_BOSS_DEATH_SPRITES * 8)
        {
            isDied = true;
        }
        return;
    }
    // render HP
    SDL_Rect HPBox = mBox;
    HPBox.y -= 16;
    HPBox.h = 8;
    HPBox.w = HPBox.w * HP / BOSS_INITIAL_HP;
    HPBox.x -= camera.x;
    HPBox.y -= camera.y;
    window.renderFillBox(HPBox, 255, 0, 0);
    //
    if (isTakeHit)
    {
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossTakeHitClips[cntTakeHitFrames / 6], 0.0, NULL, flip);
        cntTakeHitFrames++;
        if (cntTakeHitFrames >= TOTAL_BOSS_TAKE_HIT_SPRITES * 6)
        {
            isTakeHit = false;
            cntTakeHitFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntAttackFrames = cntSmashFrames = cntCastFrames = 0;
        isAttacking = false;
        return;
    }
    if (isCasting)
    {
        fireRain.insert(sound[BOSS_FIRE_SOUND]);
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossCastClips[cntCastFrames / 5], 0.0, NULL, flip);
        cntCastFrames++;
        if (cntCastFrames >= TOTAL_BOSS_CAST_SPRITES * 5)
        {
            isCasting = false;
            cntCastFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntTakeHitFrames = cntAttackFrames = cntSmashFrames = 0;
        return;
    }
    if (isSmashing)
    {
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossSmashClips[cntSmashFrames / 5], 0.0, NULL, flip);

        if(cntSmashFrames == 56)
            Mix_PlayChannel(-1, sound[BOSS_BOOM_SOUND], 0);

        cntSmashFrames++;

        /// render hitbox
        // if (55 <= cntSmashFrames && cntSmashFrames <= 60)
        // {
        //     SDL_Rect attackBox = {mBox.x, mBox.y + mBox.h * 1 / 2, mBox.w * 2, mBox.h / 2};
        //     attackBox.x += mBox.w / 2 - attackBox.w / 2;
        //     attackBox.x -= camera.x;
        //     attackBox.y -= camera.y;
        //     window.renderBox(attackBox);
        // }

        if (cntSmashFrames >= TOTAL_BOSS_SMASH_SPRITES * 5)
        {
            isSmashing = false;
            cntSmashFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntTakeHitFrames = cntAttackFrames = cntCastFrames = 0;
        return;
    }
    if (isAttacking)
    {
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossAttackClips[cntAttackFrames / 6], 0.0, NULL, flip);

        if(cntAttackFrames == 52)
            Mix_PlayChannel(-1, sound[BOSS_ATTACK_SOUND], 0);

        cntAttackFrames++;

        /// render hitbox
        // if (55 <= cntAttackFrames && cntAttackFrames <= 60)
        // {
        //     SDL_Rect attackBox = {mBox.x + 0.5 * mBox.w, mBox.y, mBox.w * 1.16, mBox.h};
        //     if (flip == SDL_FLIP_NONE)
        //         attackBox.x -= mBox.w * 1.16;
        //     attackBox.x -= camera.x;
        //     attackBox.y -= camera.y;
        //     window.renderBox(attackBox);
        // }

        if (cntAttackFrames >= TOTAL_BOSS_ATTACK_SPRITES * 6)
        {
            isAttacking = false;
            cntAttackFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntTakeHitFrames = cntSmashFrames = cntCastFrames = 0;
        return;
    }
    if (isWalking)
    {
        // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gBossWalkClips[cntWalkFrames / 4], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_BOSS_WALK_SPRITES * 4)
            cntWalkFrames = 0;
        cntIdleFrames = cntAttackFrames = cntTakeHitFrames = cntSmashFrames = cntCastFrames = 0;
        return;
    }
    // SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - BOSS_TEXTURE_WIDTH, mBox.y, BOSS_TEXTURE_WIDTH * 2, BOSS_TEXTURE_HEIGHT * 2};
    if (checkCollision(mBox, camera))
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
    cntWalkFrames = cntAttackFrames = cntTakeHitFrames = cntSmashFrames = cntCastFrames = 0;
}

std::pair<int, int> Boss::getAttack(SDL_Rect playerBox)
{
    // printf("%d\n", direction);
    std::pair<int, int> res = fireRain.getCountAttack(playerBox);
    // std::pair<int, int> res = make_pair(0, 0);
    if (isDied || isDeath)
        return res;
    if (isAttacking && cntAttackFrames == 60)
    {
        SDL_Rect attackBox = {mBox.x + 0.5 * mBox.w, mBox.y, mBox.w * 1.16, mBox.h};
        if (flip == SDL_FLIP_NONE)
            attackBox.x -= mBox.w * 1.16;
        if (checkCollision(attackBox, playerBox))
        {
            res.first++;
            res.second = direction;
        }
        return res;
    }
    if (isSmashing && cntSmashFrames == 60)
    {
        SDL_Rect attackBox = {mBox.x, mBox.y + mBox.h * 1 / 2, mBox.w * 2, mBox.h / 2};
        attackBox.x += mBox.w / 2 - attackBox.w / 2;
        int direction = (mBox.x + mBox.w / 2 > playerBox.x + playerBox.w / 2 ? -1 : 1);
        if (checkCollision(attackBox, playerBox))
        {
            res.first++;
            res.second = direction;
        }
        return res;
    }
    return res;
}

int Boss::attacked(const SDL_Rect &playerAttackRect, int damage, int &score)
{
    if (isDeath || isDied || notTakeHit || isSmashing)
        return 0;
    if (checkCollision(mBox, playerAttackRect))
    {
        HP -= damage;
        cntAttacked++;
        if (cntAttacked >= MAX_BOSS_ATTACKED)
        {
            cntAttacked = 0;
            notTakeHit = true;
            cntNotTakeHit = 0;
        }
        if (HP <= 0)
        {
            isDeath = true;
            score += SCORE_BOSS;
        }
        isTakeHit = true;
        if ((playerAttackRect.x * 2 + playerAttackRect.w) / 2 >= (mBox.x * 2 + mBox.w) / 2)
            mVelX = BOSS_VEL;
        else
            mVelX = -BOSS_VEL;
        return 1;
    }
    return 0;
}

int Boss::getPosX()
{
    return mPosX;
}

int Boss::getPosY()
{
    return mPosY;
}

bool Boss::getDied()
{
    return isDied;
}