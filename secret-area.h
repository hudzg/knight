#ifndef SECRET_AREA_H
#define SECRET_AREA_H

#include <SDL.h>
#include "entity.h"
#include "constants.h"
#include "utils.h"

class SecretArea : public Entity
{
private:
    enum SecretAreaState
    {
        CLOSE,
        OPEN,
        TOTAL_STATE
    };
    SDL_Rect mBox;
    // SDL_Rect clips[TOTAL_STATE];
    SecretAreaState state;
    bool canOpen;

public:
    static const int SECRET_AREA_WIDTH = 576;
    static const int SECRET_AREA_HEIGHT = 320;
    static const int SECRET_AREA_TEXTURE_WIDTH = 576;
    static const int SECRET_AREA_TEXTURE_HEIGHT = 320;
    SecretArea(int x = 0, int y = 0, SDL_Texture *mTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void render(RenderWindow &window, SDL_Rect &camera);
    SDL_Rect getBox();
    bool isOpen();
    void setOpen(SDL_Rect &playerBox);
    void setCanOpen();
};

#endif // DOOR_H