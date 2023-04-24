#include "game.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    Game game;
    if (!game.init())
    {
        printf("Failed to init\n");
    }
    else
    {
        if (!game.loadMedia())
            printf("Failed to load media\n");
        else
        {
            SDL_Event event;
            while (game.isRunning())
            {
                // printf("%d %d %d\n", game.getState(), STATE_MENU, STATE_PLAY);
                switch (game.getState())
                {
                case STATE_MENU:
                    while (SDL_PollEvent(&event))
                        game.handleMenuEvent(event);
                    game.renderMenu();
                    break;

                case STATE_PAUSE_MENU:
                    while (SDL_PollEvent(&event))
                        game.handlePauseMenuEvent(event);
                    game.renderPauseMenu();
                    break;

                case STATE_GAME_OVER_MENU:
                    while (SDL_PollEvent(&event))
                        game.handleGameOverMenuEvent(event);
                    game.renderGameOverMenu();
                    break;

                case STATE_PLAY:
                    while (SDL_PollEvent(&event))
                        game.handleGameEvent(event);
                    game.renderGame();
                    break;
                default:
                    break;
                }
                // if (game.getState() == STATE_MENU)
                // {
                //     // printf("h\n");
                //     while (SDL_PollEvent(&event))
                //         game.handleMenuEvent(event);
                //     game.renderMenu();
                // }
                // else
                // {
                //     while (SDL_PollEvent(&event))
                //         game.handleGameEvent(event);
                //     game.renderGame();
                // }
            }
        }
    }
    game.close();
    return 0;
}