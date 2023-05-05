#include "fire-ball.h"

FireBall::FireBall(float mPosX, float mPosY, SDL_Texture *mTexture)
{
    mVelY = 0;
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    this->mTexture = mTexture;
    mBox = {(int)mPosX, (int)mPosY, FIRE_BALL_WIDTH, FIRE_BALL_HEIGHT};
    for (int i = 0, x = 0; i < TOTAL_FIRE_BALL_SPRITES; i++, x += FIRE_BALL_TEXTURE_WIDTH)
        gClips[i] = {x, 0, FIRE_BALL_TEXTURE_WIDTH, FIRE_BALL_TEXTURE_HEIGHT};
    cntFrames = 0;
}

bool FireBall::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

void FireBall::move(double timeStep)
{
    mVelY += GRAVITY_SPEED;
    if (mVelY >= MAX_FALL_SPEED)
        mVelY = MAX_FALL_SPEED;
    mPosY += mVelY * timeStep;
    mBox.y = mPosY;
    // if (mPosY < 0 || mPosY + FIRE_BALL_HEIGHT > LEVEL_HEIGHT)
    // {
    //     mPosY -= mVelY * timeStep;
    // }
    mBox.y = mPosY;
}
void FireBall::render(RenderWindow &window, SDL_Rect &camera)
{
    // cerr << cntFrames << " ";
    SDL_Rect tmpBox = {mBox.x + mBox.w / 2 - FIRE_BALL_RENDER_WIDTH / 2, mBox.y, FIRE_BALL_RENDER_WIDTH, FIRE_BALL_RENDER_HEIGHT};
    window.renderPlayer(mTexture, tmpBox.x - camera.x, tmpBox.y - camera.y, tmpBox, &gClips[cntFrames / 4]);

    // render hit box
    // tmpBox = mBox;
    // tmpBox.x -= camera.x;
    // tmpBox.y -= camera.y;
    // window.renderBox(tmpBox);

    cntFrames++;
    if (cntFrames >= TOTAL_FIRE_BALL_SPRITES * 4)
        cntFrames = 0;
}

std::pair<int, int> FireBall::getAttack(SDL_Rect &playerBox)
{
    if (checkCollision(mBox, playerBox))
        return make_pair(1, 0);
    return make_pair(0, 0);
}

bool FireBall::onGround()
{
    return (mPosY + FIRE_BALL_HEIGHT + GROUND_HEIGHT > LEVEL_HEIGHT);
}

FireRain::FireRain(SDL_Texture *mTexture)
{
    timeInsert = 0;
    this->mTexture = mTexture;
    fireBalls.clear();
}

void FireRain::insert(Mix_Chunk *sound)
{
    // std::cerr << "insert\n";
    if (timeInsert == 0)
    {
        Mix_PlayChannel(-1, sound, 0);
        fireBalls.push_back(FireBall(Rand(136 * 64, 156 * 64), 6 * 64, mTexture));
    }
    timeInsert++;
    if (timeInsert >= MAX_TIME_INSERT)
        timeInsert = 0;
}

void FireRain::move()
{
    // std::cerr << "move\n";
    for (int i = 0; i < fireBalls.size(); i++)
        fireBalls[i].move();
}
void FireRain::render(RenderWindow &window, SDL_Rect &camera)
{
    // std::cerr << "render " << fireBalls.size() << " ";
    for (int i = 0; i < fireBalls.size(); i++)
    {
        // cout << i << " ";
        fireBalls[i].render(window, camera);
    }
    // std::cerr << "h\n";
}

void FireRain::checkOnGround()
{
    // std::cerr << "checkOnGround\n";
    for (int i = 0; i < fireBalls.size(); i++)
        while (i < fireBalls.size() && fireBalls[i].onGround())
        {
            fireBalls.erase(fireBalls.begin() + i);
        }
}

std::pair<int, int> FireRain::getCountAttack(SDL_Rect &playerBox)
{
    // std::cerr << "getCountAttack\n";
    std::pair<int, int> result = make_pair(0, 0);
    for (int i = 0; i < fireBalls.size(); i++)
    {
        std::pair<int, int> tmp = make_pair(0, 0);
        do
        {
            if(i >= fireBalls.size()) 
            {
                // printf("h\n");
                std::cerr << "h\n";
            }
            tmp = fireBalls[i].getAttack(playerBox);
            result.first += tmp.first;
            if (tmp.first > 0)
            {
                result.second = tmp.second;
                fireBalls.erase(fireBalls.begin() + i);
            }
        } while (i < fireBalls.size() && tmp.first > 0);

        // result.first += tmp.first;
        // if (tmp.first > 0)
        // {
        //     result.second = tmp.second;
        // }
    }
    // printf("%d\n", result.second);
    return result;
}