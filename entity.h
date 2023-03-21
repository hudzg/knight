#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

class Entity
{
protected:
    float mPosX, mPosY;
    SDL_Rect currentFrame;
    SDL_Texture *mTexture;

public:
    Entity(float mPosX = 0, float mPosY = 0, SDL_Texture* mTexture = NULL);
    SDL_Rect getCurrentFrame();
    SDL_Texture* getTexture();
};

#endif