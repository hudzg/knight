#include "player.h"

Player::Player(float x, float y, SDL_Texture *mTexture, SDL_Texture *mFireAttackTexture, SDL_Texture *mHPTexture, SDL_Texture *mSkillTexture, SDL_Texture *mMPTexture, SDL_Texture *mATKPTexture, SDL_Texture *mHPBuffTexture, SDL_Texture *mATKBuffTexture, SDL_Texture *mSkillUnlockTexture) : Entity(x, y, mTexture)
{
    fireAttackAnimation = FireAttack(mFireAttackTexture);
    skill = HammerGodSkill(mSkillTexture);
    HP = PlayerPoint(4, 40, 20, 2, mHPTexture);
    MP = PlayerPoint(4, 40, 20 + 80, 2, mMPTexture);
    ATKP = PlayerPoint(2, 40, SCREEN_HEIGHT - 20 - 80, 1, mATKPTexture);
    effectHPBuff = HPBuff(mHPBuffTexture);
    effectATKBuff = ATKBuff(mATKBuffTexture);
    effectSkillUnlock = SkillUnlock(mSkillUnlockTexture);
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
        gPlayerAttackClips[i] = {x, 1280, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_DASH_SPRITES; i++, x += 480)
        gPlayerDashClips[i] = {x, 1600, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_PLAYER_TAKE_HIT_SPRITES; i++, x += 480)
        gPlayerTakeHitClips[i] = {x, 1920, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT};
    flip = SDL_FLIP_NONE;
    cntTimeTakeTrap = 0;
    isTakeTrap = false;
    useSkill = false;
    doorToAddHP.clear();
    doorToAddHP.push_back(0); doorToAddHP.push_back(2);
    doorToAddATK.clear();
    doorToAddATK.push_back(1); doorToAddATK.push_back(3);
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

bool Player::checkCollisionDoor(vector<Door> &doors)
{
    for (int i = 0; i < doors.size(); i++)
        if (!doors[i].isOpen() && checkCollision(mBox, doors[i].getBox()))
            return true;
    return false;
}

bool Player::checkCollisionWall(SDL_Rect &a, Tile *b)
{
    for (int i = 0; i < TOTAL_TILES; i++)
    {
        // if (!isTakeTrap && b[i].getType() == TILE_TRAP)
        // {
        //     SDL_Rect tmp = b[i].getBox();
        //     tmp.x += tmp.w / 3;
        //     tmp.w /= 3;
        //     tmp.h /= 3;
        //     if (checkCollision(a, tmp))
        //     {
        //         attacked(make_pair(1, 1));
        //         isTakeTrap = true;
        //         cntTimeTakeTrap = 0;
        //         return false;
        //     }
        // }
        if (b[i].getIsWall() && checkCollision(a, b[i].getBox()))
            return true;
    }
    return false;
}

void Player::checkCollisionTrap(const vector<SDL_Rect> &b)
{
    if (isTakeTrap)
        return;
    for (SDL_Rect x : b)
        if (checkCollision(mBox, x))
        {
            isTakeTrap = true;
            cntTimeTakeTrap = 0;
            attacked(make_pair(1, 0));
            return;
        }
    return;
}

void Player::handleEvent(SDL_Event &e, GameState &state)
{
    // if (isTakeHit)
    //     return;
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LSHIFT:
            if (!isTakeHit)
            {
                isDashing = true;
                // isAttacking = false;
                // useSkill = false;
            }
            break;
        case SDLK_SPACE:
            if (onGround && !isTakeHit)
            {
                mVelY = -PLAYER_VEL * 2.25;
                onGround = false;
            }
            break;
        case SDLK_a:
            mVelX -= PLAYER_VEL / 1.5;
            break;
        case SDLK_d:
            mVelX += PLAYER_VEL / 1.5;
            break;
        case SDLK_ESCAPE:
            state = STATE_PAUSE_MENU;
            break;
        }
    }
    else if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
            mVelX += PLAYER_VEL / 1.5;
            break;
        case SDLK_d:
            mVelX -= PLAYER_VEL / 1.5;
            break;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0)
    {
        if (!isTakeHit && !isAttacking)
        {
            switch (e.button.button)
            {
            case SDL_BUTTON_LEFT:
                isAttacking = true;
                break;
            case SDL_BUTTON_RIGHT:
                if (skill.getIsUnlock() && MP.checkFullPoint())
                {
                    isAttacking = true;
                    useSkill = true;
                    skill.Reset();
                    MP.addPoint(-MP.getPoint());
                }
                break;
            default:
                break;
            }
        }
        // if (e.button.button == SDL_BUTTON_LEFT && !isTakeHit)
        // {
        //     isAttacking = true;
        // }
        // if (e.button.button == SDL_BUTTON_RIGHT && !isTakeHit)
        // {
        //     isAttacking = true;
        //     useSkill = true;
        // }
    }
}

void Player::move(Tile *tiles, vector<Door> &doors, SecretArea &secretArea, double timeStep)
{
    // if (mVelY < 0)
    // {
    //     isJumping = true;
    //     cntJump++;
    //     if (cntJump >= PLAYER_MAX_CNT_JUMP)
    //     {
    //         cntJump = 0;
    //         mVelY *= -1;
    //     }
    // }
    // else
    //     isJumping = false;
    mVelY += GRAVITY_SPEED;
    if (mVelY >= MAX_FALL_SPEED)
        mVelY = MAX_FALL_SPEED;
    if (mVelY < 0)
        isJumping = true;
    else
        isJumping = false;

    cntTimeTakeTrap++;
    if (cntTimeTakeTrap >= MAX_TIME_TAKE_TRAP)
    {
        isTakeTrap = false;
        cntTimeTakeTrap = 0;
    }

    if (isTakeHit)
    {
        // printf()
        mPosX += 0.5 * direction * PLAYER_VEL * timeStep;
        mBox.x = mPosX;
        if (mPosX < 0 || mPosX + PLAYER_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles) || checkCollisionDoor(doors) || (!secretArea.isOpen() && checkCollision(mBox, secretArea.getBox())))
        {
            mPosX -= 0.5 * direction * PLAYER_VEL * timeStep;
            mBox.x = mPosX;
        }
    }
    else
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

        if (isDashing)
        {
            // printf("h\n");
            int mDashVelX = PLAYER_DASH_VEL_LEVEL * direction * PLAYER_VEL / 1.25 * cos(1.0 * cntDashFrames / (TOTAL_PLAYER_DASH_SPRITES * 6) * PI / 2);
            // printf("%d %f\n", mDashVelX, 1.0 * cntDashFrames / (TOTAL_PLAYER_DASH_SPRITES * 4) * PI / 2);
            mPosX += mDashVelX * timeStep;
            mBox.x = mPosX;
            if (mPosX < 0 || mPosX + PLAYER_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles) || checkCollisionDoor(doors) || (!secretArea.isOpen() && checkCollision(mBox, secretArea.getBox())))
            {
                mPosX -= mDashVelX * timeStep;
                mBox.x = mPosX;
            }
        }
        else
        {
            mPosX += mVelX * timeStep;
            mBox.x = mPosX;
            if (mPosX < 0 || mPosX + PLAYER_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles) || checkCollisionDoor(doors) || (!secretArea.isOpen() && checkCollision(mBox, secretArea.getBox())))
            {
                // printf("h\n");
                mPosX -= mVelX * timeStep;
                mBox.x = mPosX;
            }
        }
    }

    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + PLAYER_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles) || checkCollisionDoor(doors) || (!secretArea.isOpen() && checkCollision(mBox, secretArea.getBox())))
    {
        if (mVelY > 0)
            onGround = true;
        else
            onGround = false;
        mPosY -= mVelY * timeStep;
    }
    else
        onGround = false;
    mBox.x = mPosX;
    mBox.y = mPosY;
}

void Player::renderEffect(RenderWindow &window, SDL_Rect &camera)
{
    effectHPBuff.render(window, camera, mBox, flip);
    effectATKBuff.render(window, camera, mBox, flip);
    effectSkillUnlock.render(window, camera, mBox, flip);
}

void Player::render(RenderWindow &window, SDL_Rect &camera, SkeletonFamily &skeletonFamily, Boss &boss, vector<Door> &doors, SecretArea &secretArea, Key &key, Chest &chest)
{
    // render hud
    HP.render(window);
    if (skill.getIsUnlock())
        MP.render(window);
    ATKP.render(window);
    // effectHPBuff.render(window, camera, mBox, flip);

    SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - PLAYER_RENDER_WIDTH / 2, mBox.y, PLAYER_RENDER_WIDTH, PLAYER_RENDER_HEIGHT};
    // SDL_Rect tmpBox2 = tmpBox;
    // tmpBox2.x -= camera.x;
    // tmpBox2.y -= camera.y;
    // window.renderBox(tmpBox2);
    if (isTakeHit)
    {
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerTakeHitClips[cntTakeHitFrames / 8], 0.0, NULL, flip);

        // render take hit screen
        if (cntTakeHitFrames < 20)
        {
            SDL_Rect takeHitBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            // window.renderFillBox(takeHitBox, 255, 64, 64, 64);
            window.renderFillBox(takeHitBox, 139, 0, 0, 64);
        }
        //

        cntTakeHitFrames++;
        if (cntTakeHitFrames >= TOTAL_PLAYER_TAKE_HIT_SPRITES * 8)
        {
            isTakeHit = false;
            cntTakeHitFrames = 0;
            direction *= -1;
        }
        isDashing = isAttacking = false;
        cntIdleFrames = cntJumpFrames = cntWalkFrames = cntFallFrames = cntAttackFrames = cntDashFrames = 0;
        return;
    }
    if (isAttacking)
    {
        // attack when dash
        if (isDashing)
        {
            cntDashFrames++;
            if (cntDashFrames >= TOTAL_PLAYER_DASH_SPRITES * 6)
            {
                isDashing = false;
                cntDashFrames = 0;
            }
        }

        // normal attack
        if (!useSkill)
        {
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerAttackClips[cntAttackFrames / 6], 0.0, NULL, flip);
            fireAttackAnimation.render(window, mBox.x + mBox.w / 2 - FireAttack::FIRE_ATTACK_WIDTH / 2 - camera.x, mBox.y + mBox.h / 2 - FireAttack::FIRE_ATTACK_HEIGHT / 2 - camera.y, mBox, flip);
            tmpBox.x = mBox.x + mBox.w / 2 - FireAttack::FIRE_ATTACK_REAL_WIDTH / 2;
            tmpBox.y = mBox.y + mBox.h / 2 - FireAttack::FIRE_ATTACK_REAL_HEIGHT / 2;
            tmpBox.w = FireAttack::FIRE_ATTACK_REAL_WIDTH;
            tmpBox.h = FireAttack::FIRE_ATTACK_REAL_HEIGHT;
            SDL_Rect tmpBox2 = {tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox.w, tmpBox.h};
            window.renderBox(tmpBox2);
            if (cntAttackFrames == 0)
            {
                int cntEnemyAttack = 0;
                cntEnemyAttack += skeletonFamily.attacked(tmpBox, ATKP.getPoint());
                cntEnemyAttack += boss.attacked(tmpBox, ATKP.getPoint());
                MP.addPoint(cntEnemyAttack);
                for (int i = 0; i < doors.size(); i++)
                    doors[i].setOpen(tmpBox);
                for(int i = 0; i < doorToAddHP.size(); i++)
                    while(i < doorToAddHP.size() && doors[doorToAddHP[i]].isOpen())
                    {
                        HP.addTotalPoint(1);
                        doorToAddHP.erase(doorToAddHP.begin() + i);
                        effectHPBuff.setIsBuff();
                    }
                for(int i = 0; i < doorToAddATK.size(); i++)
                    while(i < doorToAddATK.size() && doors[doorToAddATK[i]].isOpen())
                    {
                        ATKP.addTotalPoint(1);
                        doorToAddATK.erase(doorToAddATK.begin() + i);
                        effectATKBuff.setIsBuff();
                    }
                secretArea.setOpen(tmpBox);
                if (key.isPick())
                {
                    if (chest.setOpen(tmpBox))
                    {
                        key.setUsed();
                        skill.setUnlock();
                        effectSkillUnlock.setIsBuff();
                    }
                }
            }
            cntAttackFrames++;
            if (cntAttackFrames >= TOTAL_PLAYER_ATTACK_SPRITES * 6)
            {
                isAttacking = false;
                cntAttackFrames = 0;
            }
            cntIdleFrames = cntWalkFrames = cntJumpFrames = cntFallFrames = cntTakeHitFrames = 0;
            return;
        }

        // skill attack
        // printf("h\n");
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerAttackClips[cntAttackFrames / 8], 0.0, NULL, flip);
        if (cntAttackFrames == 0)
            skill.setPos(mBox.x + mBox.w / 2 - HammerGodSkill::HAMMER_GOD_SKILL_RENDER_WIDTH / 2, mBox.y + HammerGodSkill::HAMMER_GOD_SKILL_RENDER_HEIGHT / 20 + mBox.h - HammerGodSkill::HAMMER_GOD_SKILL_RENDER_HEIGHT, flip);
        skill.render(window, camera);
        tmpBox = skill.getAttackBox();
        SDL_Rect tmpBox2 = {tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox.w, tmpBox.h};
        window.renderBox(tmpBox2);
        if (cntAttackFrames == 36)
        {
            int cntEnemyAttack = 0;
            cntEnemyAttack += skeletonFamily.attacked(tmpBox, ATKP.getPoint());
            cntEnemyAttack += boss.attacked(tmpBox, ATKP.getPoint());
            MP.addPoint(cntEnemyAttack);
            for (int i = 0; i < doors.size(); i++)
                doors[i].setOpen(tmpBox);
            secretArea.setOpen(tmpBox);
        }
        cntAttackFrames++;
        if (cntAttackFrames >= TOTAL_PLAYER_ATTACK_SPRITES * 8)
        {
            isAttacking = false;
            useSkill = false;
            cntAttackFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntJumpFrames = cntFallFrames = cntTakeHitFrames = 0;
        return;
    }
    if (isDashing)
    {
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerDashClips[cntDashFrames / 6], 0.0, NULL, flip);
        cntDashFrames++;
        if (cntDashFrames >= TOTAL_PLAYER_DASH_SPRITES * 6)
        {
            isDashing = false;
            cntDashFrames = 0;
        }
        cntIdleFrames = cntWalkFrames = cntJumpFrames = cntFallFrames = cntAttackFrames = cntTakeHitFrames = 0;
        return;
    }
    if (isJumping)
    {
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerJumpClips[cntJumpFrames / 4], 0.0, NULL, flip);
        cntJumpFrames++;
        if (cntJumpFrames >= TOTAL_PLAYER_JUMP_SPRITES * 4)
            cntJumpFrames = 0;
        cntIdleFrames = cntWalkFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
        return;
    }
    if (!onGround)
    {
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerFallClips[cntFallFrames / 4], 0.0, NULL, flip);
        cntFallFrames++;
        if (cntFallFrames >= TOTAL_PLAYER_FALL_SPRITES * 4)
            cntFallFrames = 0;
        cntIdleFrames = cntWalkFrames = cntJumpFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
        return;
    }
    if (!isWalking)
    {
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerIdleClips[cntIdleFrames / 8], 0.0, NULL, flip);
        cntIdleFrames++;
        if (cntIdleFrames >= TOTAL_PLAYER_IDLE_SPRITES * 8)
            cntIdleFrames = 0;
        cntJumpFrames = cntWalkFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
    }
    else
    {
        window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gPlayerWalkClips[cntWalkFrames / 6], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_PLAYER_WALK_SPRITES * 6)
            cntWalkFrames = 0;
        cntJumpFrames = cntIdleFrames = cntFallFrames = cntAttackFrames = cntDashFrames = cntTakeHitFrames = 0;
    }
}

void Player::attacked(std::pair<int, int> value)
{
    HP.addPoint(-value.first);
    if (value.first > 0)
    {
        // printf("h\n");
        cntTakeHitFrames = 0;
        isTakeHit = true;
        mVelY = -750;
        if (value.second == 1)
        {
            direction = 1;
            flip = SDL_FLIP_HORIZONTAL;
        }
        else if (value.second == -1)
        {
            direction = -1;
            flip = SDL_FLIP_NONE;
        }
        else
        {
            direction *= -1;
        }
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

int Player::getHP()
{
    return HP.getPoint();
}

SDL_Rect Player::getBox()
{
    return mBox;
}