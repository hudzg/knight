#include "entity.h"

Entity::Entity(float mPosX, float mPosY, SDL_Texture* mTexture) : mPosX(mPosX), mPosY(mPosY), mTexture(mTexture)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    SDL_QueryTexture(mTexture, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

SDL_Texture* Entity::getTexture()
{
    return mTexture;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}