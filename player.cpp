#include "player.h"

FireAttack::FireAttack(float x, float y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    cntFrames = 0;
    for (int i = 0, x = 0; i < TOTAL_ATTACK_SPRITES; i++, x += FIRE_ATTACK_TEXTURE_WIDTH)
        gClips[i] = {x, 0, FIRE_ATTACK_TEXTURE_WIDTH, FIRE_ATTACK_TEXTURE_HEIGHT};
}

void FireAttack::attack(RenderWindow &window, int x, int y, SDL_Rect mBox, SDL_RendererFlip flip)
{
    mBox.w = FIRE_ATTACK_WIDTH;
    mBox.h = FIRE_ATTACK_HEIGHT;
    window.renderPlayer(getTexture(), x, y, mBox, &gClips[cntFrames / 2], 0.0, NULL, flip);
    cntFrames++;
    if (cntFrames >= TOTAL_ATTACK_SPRITES * 2)
        cntFrames = 0;
}

HealthPoint::HealthPoint(SDL_Texture *mTexture) : Entity(0, 0, mTexture)
{
    HP = TOTAL_HP;
    for (int i = 0, x = 0; i < TOTAL_HP_SPRITES; i++, x += HP_TEXTURE_WIDTH)
        gClips[i] = {x, 0, HP_TEXTURE_WIDTH, HP_TEXTURE_HEIGHT};
    mBox = {0, 0, HP_WIDTH, HP_HEIGHT};
}

void HealthPoint::render(RenderWindow &window)
{
    for(int i = 0, x = HP_POS_X; i < TOTAL_HP; i++, x += HP_WIDTH)
        window.renderPlayer(getTexture(), x, HP_POS_Y, mBox, &gClips[i >= HP]);
}

void HealthPoint::addHP(int value)
{
    HP = max(0, HP + value);
}

Player::Player(float x, float y, SDL_Texture *mTexture, SDL_Texture *mFireAttackTexture, SDL_Texture *mHPTexture) : Entity(x, y, mTexture)
{
    fireAttackAnimation = FireAttack(x, y, mFireAttackTexture);
    HP = HealthPoint(mHPTexture);
    direction = 1;
    cntJump = 0;
    mVelX = 0;
    mVelY = PLAYER_VEL * PLAYER_JUMP_VEL_LEVEL;
    mBox = {(int)x, (int)y, PLAYER_WIDTH, PLAYER_HEIGHT};
    onGround = false;
    isWalking = false;
    isJumping = false;
    isAttacking = false;
    isDashing = false;
    isTakeHit = false;
    cntWalkFrames = cntIdleFrames = cntJumpFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
    for (int i = 0, x = 0; i < TOTAL_PLAYER_WALK_SPRITES; i++, x += 480)
        gPlayerIdleClips[i] = {x, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_WALK_SPRITES; i++, x += 480)
        gPlayerWalkClips[i] = {x, 320, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_JUMP_SPRITES; i++, x += 480)
        gPlayerJumpClips[i] = {x, 640, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_FALL_SPRITES; i++, x += 480)
        gPlayerFallClips[i] = {x, 960, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_ATTACK_SPRITES; i++, x += 480)
        gPlayerAttackClips[i] = {x, 1280, (int)(PLAYER_TEXTURE_WIDTH * 1.5), PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_DASH_SPRITES; i++, x += 480)
        gPlayerDashClips[i] = {x, 1600, (int)(PLAYER_TEXTURE_WIDTH * 1.5), PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_TAKE_HIT_SPRITES; i++, x += 480)
        gPlayerTakeHitClips[i] = {x, 1920, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    flip = SDL_FLIP_NONE;
}

bool Player::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

bool Player::checkCollisionWall(SDL_Rect &a, Tile *b)
{
    for (int i = 0; i < TOTAL_TILES; i++)
        if (b[i].getIsWall() && checkCollision(a, b[i].getBox()))
            return true;
    return false;
}

void Player::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LSHIFT:
            isDashing = true;
            break;
        case SDLK_SPACE:
            if (onGround)
            {
                mVelY = -PLAYER_VEL * PLAYER_JUMP_VEL_LEVEL;
                onGround = false;
            }
            break;
        case SDLK_a:
            mVelX -= PLAYER_VEL;
            break;
        case SDLK_d:
            mVelX += PLAYER_VEL;
            break;
        }
    }
    else if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
            mVelX += PLAYER_VEL;
            break;
        case SDLK_d:
            mVelX -= PLAYER_VEL;
            break;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0)
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            isAttacking = true;
        }
    }
}

void Player::move(Tile *tiles, double timeStep)
{
    if (mVelY < 0)
    {
        isJumping = true;
        cntJump++;
        if (cntJump >= PLAYER_MAX_CNT_JUMP)
        {
            cntJump = 0;
            mVelY *= -1;
        }
    }
    else
        isJumping = false;

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

    if (isDashing)
    {
        int mDashVelX = PLAYER_DASH_VEL_LEVEL * direction * PLAYER_VEL;
        mPosX += mDashVelX * timeStep;
        mBox.x = mPosX;
        if (mPosX < 0 || mPosX + PLAYER_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles))
        {
            mPosX -= mDashVelX * timeStep;
            mBox.x = mPosX;
        }
    }
    else
    {
        mPosX += mVelX * timeStep;
        mBox.x = mPosX;
        if (mPosX < 0 || mPosX + PLAYER_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles))
        {
            // printf("h\n");
            mPosX -= mVelX * timeStep;
            mBox.x = mPosX;
        }
    }

    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + PLAYER_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles))
    {
        onGround = true;
        mPosY -= mVelY * timeStep;
    }
    else
        onGround = false;
    mBox.x = mPosX;
    mBox.y = mPosY;
}

void Player::render(RenderWindow &window, SDL_Rect &camera, SkeletonFamily &skeletonFamily)
{
    // render HP
    HP.render(window);

    if(isTakeHit)
    {
        window.renderPlayer(getTexture(), mPosX - camera.x, mPosY - camera.y, mBox, &gPlayerTakeHitClips[cntTakeHitFrames / 4], 0.0, NULL, flip);
        cntTakeHitFrames++;
        if (cntTakeHitFrames >= TOTAL_PLAYER_TAKE_HIT_SPRITES * 4)
        {
            isTakeHit = false;
            cntTakeHitFrames = 0;
        }
        cntIdleFrames = cntJumpFrames = cntWalkFrames = cntFallFrames = cntAttackFrames = cntDashFrames = 0;
        return;
    }
    if (isDashing)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y, mBox.w * 1.5, mBox.h};
        int value = 0;
        if (flip == SDL_FLIP_HORIZONTAL)
            value = -PLAYER_WIDTH / 2;
        window.renderPlayer(getTexture(), mPosX - camera.x + value, mPosY - camera.y, tmpBox, &gPlayerDashClips[cntDashFrames / 4], 0.0, NULL, flip);
        cntDashFrames++;
        if (cntDashFrames >= TOTAL_PLAYER_DASH_SPRITES * 4)
        {
            isDashing = false;
            cntDashFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntJumpFrames = cntFallFrames = cntAttackFrames = cntTakeHitFrames = 0;
        return;
    }
    if (isAttacking)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y, mBox.w * 1.5, mBox.h};
        int value = 0;
        if (flip == SDL_FLIP_HORIZONTAL)
            value = -PLAYER_WIDTH / 2;
        window.renderPlayer(getTexture(), mPosX - camera.x + value, mPosY - camera.y, tmpBox, &gPlayerAttackClips[cntAttackFrames / 6], 0.0, NULL, flip);
        fireAttackAnimation.attack(window, mPosX - camera.x - PLAYER_WIDTH + (direction == -1 ? value : 0), mPosY - camera.y, mBox, flip);
        tmpBox.x += - PLAYER_WIDTH;
        tmpBox.w = FireAttack::FIRE_ATTACK_TEXTURE_WIDTH;
        tmpBox.h = FireAttack::FIRE_ATTACK_TEXTURE_HEIGHT;
        if(cntAttackFrames == 0) 
            skeletonFamily.attacked(tmpBox);
        cntAttackFrames++;
        if (cntAttackFrames >= TOTAL_PLAYER_ATTACK_SPRITES * 6)
        {
            isAttacking = false;
            cntAttackFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntJumpFrames = cntFallFrames = cntDashFrames = cntTakeHitFrames = 0;
        return;
    }
    if (isJumping)
    {
        window.renderPlayer(getTexture(), mPosX - camera.x, mPosY - camera.y, mBox, &gPlayerJumpClips[cntJumpFrames / 4], 0.0, NULL, flip);
        cntJumpFrames++;
        if (cntJumpFrames >= TOTAL_PLAYER_JUMP_SPRITES * 4)
            cntJumpFrames = 0;
        cntIdleFrames = cntWalkFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
        return;
    }
    if (!onGround)
    {
        window.renderPlayer(getTexture(), mPosX - camera.x, mPosY - camera.y, mBox, &gPlayerFallClips[cntFallFrames / 4], 0.0, NULL, flip);
        cntFallFrames++;
        if (cntFallFrames >= TOTAL_PLAYER_FALL_SPRITES * 4)
            cntFallFrames = 0;
        cntIdleFrames = cntWalkFrames = cntJumpFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
        return;
    }
    if (!isWalking)
    {
        window.renderPlayer(getTexture(), mPosX - camera.x, mPosY - camera.y, mBox, &gPlayerIdleClips[cntIdleFrames / 8], 0.0, NULL, flip);
        cntIdleFrames++;
        if (cntIdleFrames >= TOTAL_PLAYER_IDLE_SPRITES * 8)
            cntIdleFrames = 0;
        cntJumpFrames = cntWalkFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
    }
    else
    {
        window.renderPlayer(getTexture(), mPosX - camera.x, mPosY - camera.y, mBox, &gPlayerWalkClips[cntWalkFrames / 6], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_PLAYER_WALK_SPRITES * 6)
            cntWalkFrames = 0;
        cntJumpFrames = cntIdleFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
    }
}

void Player::attacked(int value)
{
    HP.addHP(-value);
    if(value > 0) 
    {
        cntTakeHitFrames = 0;
        isTakeHit = true;
    }
}

void Player::setCamera(SDL_Rect &camera)
{
    camera.x = (mPosX + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mPosY + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    camera.x = max(camera.x, 0);
    camera.x = min(camera.x, LEVEL_WIDTH - SCREEN_WIDTH);
    camera.y = max(camera.y, 0);
    camera.y = min(camera.y, LEVEL_HEIGHT - SCREEN_HEIGHT);
}

int Player::getPosX()
{
    return mPosX;
}

int Player::getPosY()
{
    return mPosY;
}

SDL_Rect Player::getBox()
{
    return mBox;
}