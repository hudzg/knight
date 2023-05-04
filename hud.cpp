#include "hud.h"

// HealthPoint::HealthPoint(SDL_Texture *mTexture) : Entity(0, 0, mTexture)
// {
//     HP = TOTAL_HP;
//     for (int i = 0, x = 0; i < TOTAL_HP_SPRITES; i++, x += HP_TEXTURE_WIDTH)
//         gClips[i] = {x, 0, HP_TEXTURE_WIDTH, HP_TEXTURE_HEIGHT};
//     mBox = {0, 0, HP_WIDTH, HP_HEIGHT};
// }

// void HealthPoint::render(RenderWindow &window)
// {
//     for (int i = 0, x = HP_POS_X; i < TOTAL_HP; i++, x += HP_WIDTH)
//         window.renderPlayer(getTexture(), x, HP_POS_Y, mBox, &gClips[i >= HP]);
// }

// void HealthPoint::addHP(int value)
// {
//     HP = max(0, HP + value);
// }

// int HealthPoint::getHP()
// {
//     return HP;
// }

PlayerPoint::PlayerPoint(int totalPoint, int mPosX, int mPosY, int TOTAL_POINT_SPRITES, SDL_Texture *mTexture)
{
    this->totalPoint = totalPoint;
    this->mPosX = mPosX;
    this->mPosY = mPosY;
    // printf("%d %d\n", mPosX, mPosY);
    this->mTexture = mTexture;
    this->TOTAL_POINT_SPRITES = TOTAL_POINT_SPRITES;
    point = totalPoint;
    for (int i = 0, x = 0; i < TOTAL_POINT_SPRITES; i++, x += PLAYER_POINT_TEXTURE_WIDTH)
        gClips.push_back({x, 0, PLAYER_POINT_TEXTURE_WIDTH, PLAYER_POINT_TEXTURE_HEIGHT});
    mBox = {0, 0, PLAYER_POINT_WIDTH, PLAYER_POINT_HEIGHT};
}

void PlayerPoint::render(RenderWindow &window)
{
    // if(s != "") std::cout << (mTexture != NULL) << endl;
    for (int i = 0, x = mPosX; i < totalPoint; i++, x += PLAYER_POINT_WIDTH)
    {
        window.renderPlayer(mTexture, x, mPosY, mBox, &gClips[i >= point]);
        // printf("%d %d\n", x, mPosY);
    }
}

void PlayerPoint::addPoint(int value)
{
    point += value;
    if(point < 0) point = 0;
    if(point > totalPoint) point = totalPoint;
}

void PlayerPoint::addTotalPoint(int value)
{
    totalPoint += value;
    point = totalPoint;
}

int PlayerPoint::getPoint()
{
    return point;
}

bool PlayerPoint::checkFullPoint()
{
    return point == totalPoint;
}