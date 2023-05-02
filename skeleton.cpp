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
        if ((b[i].getIsWall() || b[i].getType() == SKELETON_IS_WALL) && checkCollision(a, b[i].getBox()))
            return true;
    return false;
}

bool Skeleton::checkCollisionDoor(vector <Door> &doors)
{
    for(int i = 0; i < doors.size(); i++)
        if(!doors[i].isOpen() && checkCollision(mBox, doors[i].getBox()))
            return true;
    return false;
}

Skeleton::Skeleton(float x, float y, int ID, SDL_Texture *mTexture) : Entity(x, y, mTexture)
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
    this->ID = ID;
}

void Skeleton::move(Tile *tiles, const SDL_Rect &playerBox, vector <Door> &doors, SecretArea &secretArea, double timeStep)
{
    // printf("%f %f\n", mPosX, mPosY);
    if (isDied)
        return;
    attacked(mBox);

    if (isTakeHit)
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
        return;
    }

    if (isAttacking || isDeath)
        return;
    // if(isDeath) return;

    SDL_Rect attackBox = {mPosX - MAX_ATTACK_WIDTH, mPosY, MAX_ATTACK_WIDTH * 2 + SKELETON_WIDTH, SKELETON_HEIGHT};

    if (checkCollision(attackBox, playerBox))
    {
        // printf("%d %f\n", playerBox.x, mPosX);
        if (playerBox.x < mPosX)
        {
            direction = -1;
            flip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            // printf("h\n");
            direction = 1;
            flip = SDL_FLIP_NONE;
        }
        isAttacking = true;
    }
    else
    {
        SDL_Rect chasingBox = {initialX - MAX_CHASE_WIDTH, mPosY, MAX_CHASE_WIDTH * 2 + SKELETON_WIDTH, SKELETON_HEIGHT};
        // if (initialX - MAX_CHASE_WIDTH <= playerBox.x && playerBox.x <= initialX + MAX_CHASE_WIDTH)
        if (checkCollision(chasingBox, playerBox))
        {
            isChasing = true;
            if (playerBox.x >= mPosX)
                mVelX = 1.5 * SKELETON_VEL;
            else
                mVelX = -1.5 * SKELETON_VEL;
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

    if (isTakeHit)
        return;

    mPosX += mVelX * timeStep;
    mBox.x = mPosX;
    if (mPosX < 0 || mPosX + SKELETON_WIDTH > LEVEL_WIDTH || checkCollisionWall(mBox, tiles) || checkCollisionDoor(doors) || (!secretArea.isOpen() && checkCollision(mBox, secretArea.getBox())) || mPosX < initialX - MAX_WALK_WIDTH || mPosX > initialX + MAX_WALK_WIDTH)
    {
        mPosX -= mVelX * timeStep;
        mVelX = 0;
        direction *= -1;
    }

    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    if (mPosY < 0 || mPosY + SKELETON_HEIGHT > LEVEL_HEIGHT || checkCollisionWall(mBox, tiles) || checkCollisionDoor(doors) || (!secretArea.isOpen() && checkCollision(mBox, secretArea.getBox())))
    {
        mPosY -= mVelY * timeStep;
    }
    else
        mBox.x = mPosX;
    mBox.y = mPosY;
}

void Skeleton::render(RenderWindow &window, SDL_Rect &camera)
{
    if (isDied)
        return;
    if (isDeath)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y, 1.0 * mBox.w * 330 / 240, 1.0 * mBox.h * 320 / 330};
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gSkeletonDeathClips[cntDeathFrames / 8], 0.0, NULL, flip);
        cntDeathFrames++;
        if (cntDeathFrames >= TOTAL_SKELETON_DEATH_SPRITES * 8)
        {
            isDied = true;
        }
        return;
    }
    // render HP
    SDL_Rect HPBox = mBox;
    HPBox.y -= 16;
    HPBox.h = 4;
    HPBox.w = HPBox.w * HP / SKELETON_INITIAL_HP;
    HPBox.x -= camera.x;
    HPBox.y -= camera.y;
    window.renderFillBox(HPBox, 255, 0, 0);
    //

    if (isTakeHit)
    {
        SDL_Rect tmpBox = {mBox.x, mBox.y, mBox.w * 1.25, mBox.h};
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gSkeletonTakeHitClips[cntTakeHitFrames / 8], 0.0, NULL, flip);
        cntTakeHitFrames++;
        if (cntTakeHitFrames >= TOTAL_SKELETON_TAKE_HIT_SPRITES * 8)
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
        SDL_Rect tmpBox = {mBox.x, mBox.y - mBox.h * 0.12, mBox.w * 1.8, mBox.h * 1.12};
        if (flip != SDL_FLIP_NONE)
            tmpBox.x -= mBox.w * 0.8;
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gSkeletonAttackClips[cntAttackFrames / 8], 0.0, NULL, flip);

        // render attack hit box
        // if (50 <= cntAttackFrames && cntAttackFrames <= 70)
        // {
        //     SDL_Rect attackBox = {mBox.x + mBox.w * 0.8, mBox.y - mBox.h * 0.12, mBox.w, mBox.h * 1.12};
        //     if (flip != SDL_FLIP_NONE)
        //         attackBox.x = mBox.x - mBox.w * 0.8;
        //     attackBox.x -= camera.x;
        //     attackBox.y -= camera.y;
        //     window.renderBox(attackBox);
        // }
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
        if (checkCollision(mBox, camera))
            window.renderPlayer(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &gSkeletonWalkClips[cntWalkFrames / 4], 0.0, NULL, flip);
        cntWalkFrames++;
        if (cntWalkFrames >= TOTAL_SKELETON_WALK_SPRITES * 4)
            cntWalkFrames = 0;
        cntIdleFrames = cntAttackFrames = cntTakeHitFrames = 0;
        return;
    }
    if (checkCollision(mBox, camera))
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

std::pair<int, int> Skeleton::getAttack(SDL_Rect playerBox)
{
    // printf("%d\n", direction);
    if (isDied || !isAttacking || cntAttackFrames != 60)
        return make_pair(0, 0);
    SDL_Rect attackBox = {mBox.x + mBox.w * 0.8, mBox.y - mBox.h * 0.12, mBox.w, mBox.h * 1.12};
    if (flip != SDL_FLIP_NONE)
        attackBox.x = mBox.x - mBox.w * 0.8;
    // SDL_Rect tmpBox2 = {attackBox.x - camera.x, attackBox.y - camera.y, attackBox.w, attackBox.h};
    //     window.renderBox(tmpBox2);
    if (checkCollision(attackBox, playerBox))
        return make_pair(1, direction);
    return make_pair(0, 0);
}

void Skeleton::attacked(const SDL_Rect &playerAttackRect)
{
    if (isDeath || isDied)
        return;
    if (checkCollision(mBox, playerAttackRect))
    {
        HP--;
        if (HP == 0)
        {
            isDeath = true;
        }
        isTakeHit = true;
        if ((playerAttackRect.x * 2 + playerAttackRect.w) / 2 >= (mBox.x * 2 + mBox.w) / 2)
            mVelX = SKELETON_VEL;
        else
            mVelX = -SKELETON_VEL;
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

bool Skeleton::getDied()
{
    return isDied;
}

SkeletonFamily::SkeletonFamily(SDL_Texture *mTexture)
{
    // for (int i = 0; i < TOTAL_SKELETON; i++)
    //     skeleton[i] = Skeleton(rand() % LEVEL_WIDTH, 0, mTexture);
    skeleton.push_back(Skeleton(64, 128, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(512, 128, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(768, 960, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(15 * 64, 0 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(16 * 64, 5 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(17 * 64, 13 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(21 * 64, 10 * 64, skeleton.size(), mTexture));

    skeleton.push_back(Skeleton(33 * 64, 7 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(41 * 64, 7 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(46 * 64, 10 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(53 * 64, 7 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(58 * 64, 7 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(61 * 64, 4 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(67 * 64, 4 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(49 * 64, 1 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(55 * 64, 1 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(70 * 64, 1 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(77 * 64, 1 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(50 * 64, 13 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(58 * 64, 13 * 64, skeleton.size(), mTexture));

    skeleton.push_back(Skeleton(62 * 64, 13 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(68 * 64, 13 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(78 * 64, 13 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(86 * 64, 13 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(90 * 64, 15 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(95 * 64, 15 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(100 * 64, 15 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(92 * 64, 9 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(97 * 64, 9 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(92 * 64, 3 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(97 * 64, 3 * 64, skeleton.size(), mTexture));

    skeleton.push_back(Skeleton(109 * 64, 0 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(115 * 64, 0 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(120 * 64, 3 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(126 * 64, 6 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(132 * 64, 6 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(130 * 64, 0 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(139 * 64, 0 * 64, skeleton.size(), mTexture));
    skeleton.push_back(Skeleton(144 * 64, 0 * 64, skeleton.size(), mTexture));
}

void SkeletonFamily::move(Tile *tiles, const SDL_Rect &playerBox, vector <Door> &doors, SecretArea &secretArea)
{
    for (int i = 0; i < skeleton.size(); i++)
        skeleton[i].move(tiles, playerBox, doors, secretArea);
}

void SkeletonFamily::render(RenderWindow &window, SDL_Rect &camera)
{
    for (int i = 0; i < skeleton.size(); i++)
        skeleton[i].render(window, camera);
}

void SkeletonFamily::attacked(const SDL_Rect &playerAttackRect)
{
    for (int i = 0; i < skeleton.size(); i++)
        skeleton[i].attacked(playerAttackRect);
}

void SkeletonFamily::checkDied()
{
    for(int i = 0; i < skeleton.size(); i++)
        while (i < skeleton.size() && skeleton[i].getDied())
        {
            skeleton.erase(skeleton.begin() + i);
            cntSkeletonDied++;
        }
        
}

std::pair<int, int> SkeletonFamily::getCountAttack(SDL_Rect playerBox)
{
    std::pair<int, int> result = make_pair(0, 0);
    for (int i = 0; i < skeleton.size(); i++)
    {
        std::pair<int, int> tmp = skeleton[i].getAttack(playerBox);
        result.first += tmp.first;
        if (tmp.first > 0)
            result.second = tmp.second;
    }
    // printf("%d\n", result.second);
    return result;
}

int SkeletonFamily::getCntSkeletonDied()
{
    return cntSkeletonDied;
}