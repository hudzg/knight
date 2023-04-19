#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <algorithm>
#include "constants.h"
#include <fstream>
#include <string>

using namespace std;

class RenderWindow
{
private:
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;

public:
    bool initSDL();
    SDL_Renderer *getRenderer();
    SDL_Texture* loadFromFile(std::string path);
    void renderTile(SDL_Texture *mTexture, int x, int y, SDL_Rect rect, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderPlayer(SDL_Texture *mTexture, int x, int y, SDL_Rect rect, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderHitbox(SDL_Texture *mTexture, int x, int y, SDL_Rect rect, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderBox(SDL_Rect rect);
    void renderFillBox(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void close();
    void clearRenderer();
    void renderPresent();
};

#endif