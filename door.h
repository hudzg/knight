#ifndef DOOR_H
#define DOOR_H

#include <SDL.h>
#include "entity.h"
#include "constants.h"
#include "utils.h"

class Door : public Entity
{
private:
    enum DoorState
    {
        CLOSE,
        OPEN,
        TOTAL_STATE
    };
    SDL_Rect mBox;
    SDL_Rect clips[TOTAL_STATE];
    DoorState state;
    bool canOpen;
    int cntSkeletonToOpenDoor;

public:
    static const int DOOR_WIDTH = 96;
    static const int DOOR_HEIGHT = 192;
    static const int DOOR_TEXTURE_WIDTH = 96 * 4;
    static const int DOOR_TEXTURE_HEIGHT = 192 * 4;
    Door(int x = 0, int y = 0, SDL_Texture *mTexture = NULL, int cntSkeletonToOpenDoor = 0);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    void render(RenderWindow &window, SDL_Rect &camera);
    SDL_Rect getBox();
    bool isOpen();
    void setOpen(SDL_Rect &playerBox);
    void setCanOpen(int cntSkeleton);
};

#endif // DOOR_H