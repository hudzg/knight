#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include "entity.h"
#include "constants.h"
#include "utils.h"


class Tile : public Entity
{
private:
    SDL_Rect mBox;
    int mType;
    bool isWall;

public:
    Tile(int x = 0, int y = 0, SDL_Texture *mTexture = NULL, int tileType = 0, bool isWall = false);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void render(RenderWindow &window, SDL_Rect &camera, SDL_Rect *clip);
    SDL_Rect getBox();
    int getType();
    bool getIsWall();
};

#endif // TILE_H