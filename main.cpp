#include "game.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    Game game;
    if(!game.init())
    {
        printf("Failed to init\n");
    }
    else 
    {
        if(!game.loadMedia())
            printf("Failed to load media\n");
        else
        {
            SDL_Event event;
            while(game.isRunning())
            {
                while(SDL_PollEvent(&event)) game.handleEvent(event);
                game.renderGame();
            }
        }
    }
    game.close();
    return 0;
}