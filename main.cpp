#include "game.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    // if (!init())
    // {
    //     printf("Failed to init\n");
    // }
    // else
    // {
    //     if (!loadMedia())
    //         printf("Failed to load media");
    //     else
    //     {
    //         // for(int i = 0; i < TOTAL_TILES; i++)
    //         //     printf("%d %d %d\n", tiles[i].getBox().x, tiles[i].getBox().y, tiles[i].getType());
    //         // for(int i = 0; i < TOTAL_TILE_SPRITES; i++)
    //         //     printf("%d %d %d %d\n", gTileClips[i].x, gTileClips[i].y, gTileClips[i].w, gTileClips[i].h);
    //         bool quit = 0;
    //         SDL_Event e;
    //         Dot dot;
    //         SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    //         LTimer stepTimer;
    //         while (!quit)
    //         {
    //             while (SDL_PollEvent(&e) != 0)
    //             {
    //                 if (e.type == SDL_QUIT)
    //                 {
    //                     quit = 1;
    //                 }
    //                 dot.handleEvent(e);
    //             }
    //             // gDotTexture.setAsRenderTarget();
    //             SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    //             SDL_RenderClear(gRenderer);

    //             double timeStep = stepTimer.getTicks() / 1000.0;

    //             dot.move(tiles, timeStep);
    //             dot.setCamera(camera);

    //             stepTimer.start();

    //             for(int i = 0; i < TOTAL_TILES; i++)
    //                 tiles[i].render(camera);

    //             SDL_SetRenderTarget(gRenderer, NULL);
    //             dot.render(camera);

    //             SDL_RenderPresent(gRenderer);
    //         }
    //     }
    // }
    // close();

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