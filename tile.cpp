#include "tile.h"

Tile::Tile(int x, int y, SDL_Texture *mTexture, int tileType, bool isWall) : Entity(x, y, mTexture)
{
    mBox = {x, y, TILE_WIDTH, TILE_HEIGHT};
    mType = tileType;
    this->isWall = isWall;
}

bool Tile::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

void Tile::render(RenderWindow &window, SDL_Rect &camera, SDL_Rect *clip)
{
    if (Tile::checkCollision(camera, mBox))
        window.renderTile(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, clip);
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

int Tile::getType()
{
    return mType;
}

bool Tile::getIsWall()
{
    return isWall;
}