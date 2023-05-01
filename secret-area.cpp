#include "secret-area.h"

SecretArea::SecretArea(int x, int y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    mBox = {x, y, SECRET_AREA_WIDTH, SECRET_AREA_HEIGHT};
    state = CLOSE;
    canOpen = false;
    // for(int i = 0, x = 0; i < TOTAL_STATE; i++, x += SECRET_AREA_TEXTURE_WIDTH)
    //     clips[i] = {x, 0, SECRET_AREA_TEXTURE_WIDTH, SECRET_AREA_TEXTURE_HEIGHT};
}

bool SecretArea::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

void SecretArea::render(RenderWindow &window, SDL_Rect &camera)
{
    if (state == CLOSE && checkCollision(camera, mBox))
    {
        window.renderTile(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox);
    }
}

SDL_Rect SecretArea::getBox()
{
    return mBox;
}

void SecretArea::setOpen(SDL_Rect &playerBox)
{
    if(state == CLOSE && canOpen == true && checkCollision(mBox, playerBox))
        state = OPEN;
}

bool SecretArea::isOpen()
{
    return state == OPEN;
}

void SecretArea::setCanOpen()
{
    // bool ok = !canOpen;
    canOpen = true;
    // if(ok && canOpen) printf("h\n");
}