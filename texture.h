#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL_image.h>
#include <string>
#include "utils.h"

class LTexture
{
private:
    SDL_Texture *mTexture;
    int mWidth, mHeight;

public:
    void free();
    LTexture();
    ~LTexture();
    bool loadFromFile(std::string path);
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int getWidth();
    int getHeight();
};

#endif // TEXTURE_H