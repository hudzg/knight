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
#include <random>
#include <chrono>
#include <sstream>


using namespace std;

// mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());

// long long Rand(long long l, long long r)
// {
//     return l + (1ll * (rd() % MOD) * (rd() % MOD)) % (r - l + 1);
// }

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
    void renderTextNoFont(const char *text, const char *fontPath, int fontSize, int x, int y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
    void renderText(const char *text, TTF_Font *font, int x, int y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
    void renderTextRight(const char *text, TTF_Font *font, int x, int y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
    void renderTextCenterScreen(const char *text, TTF_Font *font, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
    void close();
    void clearRenderer();
    void renderPresent();
};

#endif