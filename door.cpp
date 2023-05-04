#include "door.h"

Door::Door(int x, int y, SDL_Texture *mTexture, int cntSkeletonToOpenDoor) : Entity(x, y, mTexture)
{
    mBox = {x, y, DOOR_WIDTH, DOOR_HEIGHT};
    state = CLOSE;
    canOpen = false;
    for(int i = 0, x = 0; i < TOTAL_STATE; i++, x += DOOR_TEXTURE_WIDTH)
        clips[i] = {x, 0, DOOR_TEXTURE_WIDTH, DOOR_TEXTURE_HEIGHT};
    this->cntSkeletonToOpenDoor = cntSkeletonToOpenDoor;
    // state = OPEN;
}

bool Door::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

void Door::render(RenderWindow &window, SDL_Rect &camera)
{
    if (checkCollision(camera, mBox))
    {
        window.renderTile(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &clips[state]);
    }
}

SDL_Rect Door::getBox()
{
    return mBox;
}

void Door::setOpen(SDL_Rect &playerBox)
{
    if(state == CLOSE && canOpen == true && checkCollision(mBox, playerBox))
        state = OPEN;
}

bool Door::isOpen()
{
    return state == OPEN;
}

void Door::setCanOpen(int cntSkeleton)
{
    // bool ok = !canOpen;
    if(cntSkeleton >= cntSkeletonToOpenDoor) canOpen = true;
    // if(ok && canOpen) printf("h\n");
}