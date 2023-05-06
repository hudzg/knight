#include "game.h"
#include "utils.h"
#include "timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    Game game;
    LTimer capTimer;
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
                capTimer.start();
                switch (game.getState())
                {
                case STATE_MENU:
                    while (SDL_PollEvent(&event))
                        game.handleMenuEvent(event);
                    game.renderMenu();
                    break;

                case STATE_GUIDE_MENU:
                    while (SDL_PollEvent(&event))
                        game.handleGuideMenuEvent(event);
                    game.renderGuideMenu();
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
                int frameTicks = capTimer.getTicks();
                if( frameTicks < SCREEN_TICKS_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
                }
            }
        }
    }
    game.close();
    return 0;
}