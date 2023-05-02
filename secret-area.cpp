#include "secret-area.h"

SecretArea::SecretArea(int x, int y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    // mPosX = x; mPosY = y;
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
    if (state == CLOSE && canOpen == true && checkCollision(mBox, playerBox))
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

Key::Key(int x, int y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    mPosX = x;
    mPosY = y;
    mBox = {x, y, KEY_WIDTH, KEY_HEIGHT};
    state = HIDDEN;
    maxTime = time = 0;
}

bool Key::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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

void Key::move()
{
    if (state != PICK || time > maxTime)
        return;

    int mVel = KEY_VEL * cos(1.0 * time / maxTime * PI / 2);

    if (mPosX < DSTPOS_X)
    {
        mPosX += mVel;
        if (mPosX > DSTPOS_X)
            mPosX = DSTPOS_X;
    }
    else if (mPosX > DSTPOS_X)
    {
        mPosX -= mVel;
        if (mPosX < DSTPOS_X)
            mPosX = DSTPOS_X;
    }

    if (mPosY < DSTPOS_Y)
    {
        mPosY += mVel;
        if (mPosY > DSTPOS_Y)
            mPosY = DSTPOS_Y;
    }
    else if (mPosY > DSTPOS_Y)
    {
        mPosY -= mVel;
        if (mPosY < DSTPOS_Y)
            mPosY = DSTPOS_Y;
    }
    time++;
}

void Key::render(RenderWindow &window, SDL_Rect &camera)
{
    if (state == NOT_PICK && checkCollision(camera, mBox))
    {
        window.renderTile(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox);
    }
    if (state == PICK)
    {
        window.renderTile(getTexture(), mPosX, mPosY, mBox);
    }
}

SDL_Rect Key::getBox()
{
    return mBox;
}
// bool isOpen();

void Key::setNotPick()
{
    if (state == HIDDEN)
        state = NOT_PICK;
}

void Key::setPick(SDL_Rect playerBox, SDL_Rect &camera)
{
    if (state == NOT_PICK && checkCollision(playerBox, mBox))
    {
        state = PICK;
        mPosX -= camera.x;
        mPosY -= camera.y;
        maxTime = 1.0 * PI / 2 * max(abs(DSTPOS_X - mPosX), abs(DSTPOS_Y - mPosY)) / KEY_VEL + 4;
    }
}

bool Key::isPick()
{
    return state == PICK;
}

void Key::setUsed()
{
    if (state == PICK)
        state = USED;
}

Chest::Chest(int x, int y, SDL_Texture *mTexture) : Entity(x, y, mTexture)
{
    mBox = {x, y, CHEST_WIDTH, CHEST_HEIGHT};
    state = HIDDEN;
    for (int i = 0, x = 0; i < 2; i++, x += CHEST_TEXTURE_WIDTH)
        clips[i] = {x, 0, CHEST_TEXTURE_WIDTH, CHEST_TEXTURE_HEIGHT};
}

bool Chest::checkCollision(SDL_Rect &a, const SDL_Rect &b)
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
void Chest::render(RenderWindow &window, SDL_Rect &camera)
{
    if (state != HIDDEN && checkCollision(camera, mBox))
    {
        window.renderTile(getTexture(), mBox.x - camera.x, mBox.y - camera.y, mBox, &clips[state - 1]);
    }
}

void Chest::setClose()
{
    if (state == HIDDEN)
        state = CLOSE;
}
bool Chest::setOpen(SDL_Rect playerBox)
{
    if (state == CLOSE && checkCollision(playerBox, mBox))
        state = OPEN;
}

SDL_Rect Chest::getBox()
{
    return mBox;
}