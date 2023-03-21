#include "game.h"

bool Game::init()
{
    camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    running = true;

    if(!window.initSDL()) return false;
    return true;
}

bool Game::setTiles(Tile *tiles)
{
    // printf("%d\n", (gRenderer != NULL));
    bool success = true;

    std::set <int> wallID;

    std::ifstream wall("images/map/wall.txt");

    if(wall.fail())
    {
        printf("Failed to load wall \n");
        success = false;
    }
    else
    {
        for(int i = 0; i < TOTAL_WALL_TILES; i++)
        {
            int x;
            wall >> x;
            if (wall.fail())
            {
                printf("Failed to load wall tile %d\n", i);
                success = false;
                break;
            }
            if (x >= 0 && x < TOTAL_TILE_SPRITES)
                wallID.insert(x);
            else
            {
                printf("Invalid map tile %d\n", i);
                success = false;
                break;
            }
        }
    }

    if(!success) return false;

    int x = 0, y = 0;
    std::ifstream map("images/map/map.map");
    if (map.fail())
    {
        printf("Failed to load map \n");
        success = false;
    }
    else
    {
        for (int i = 0; i < TOTAL_TILES; i++)
        {
            int tileType = -1;
            map >> tileType;
            if (map.fail())
            {
                printf("Failed to load map tile %d\n", i);
                success = false;
                break;
            }
            if (tileType >= 0 && tileType < TOTAL_TILE_SPRITES)
                tiles[i] = Tile(x, y, gTexture[TILE_TEXTURE], tileType, wallID.count(tileType));
            else
            {
                printf("Invalid map tile %d\n", i);
                success = false;
                break;
            }
            x += TILE_WIDTH;
            if (x >= LEVEL_WIDTH)
            {
                x = 0;
                y += TILE_HEIGHT;
            }
        }
    }
    if(success)
    {
        int x = 0, y = 0;
        for(int i = 0; i < TOTAL_TILE_SPRITES; i++)
        {
            gTileClips[i] = {x, y, TILE_TEXTURE_WIDTH, TILE_TEXTURE_HEIGHT};
            x += TILE_TEXTURE_WIDTH;
            if (x >= TILE_SET_WIDTH)
            {
                x = 0;
                y += TILE_TEXTURE_HEIGHT;
            }
        }
        // gTileClips[TILE_RED] = {0, 0, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_GREEN] = {0, 80, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_BLUE] = {0, 160, TILE_WIDTH, TILE_HEIGHT};

        // gTileClips[TILE_TOPLEFT] = {80, 0, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_LEFT] = {80, 80, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_BOTTOMLEFT] = {80, 160, TILE_WIDTH, TILE_HEIGHT};

        // gTileClips[TILE_TOP] = {160, 0, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_CENTER] = {160, 80, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_BOTTOM] = {160, 160, TILE_WIDTH, TILE_HEIGHT};

        // gTileClips[TILE_TOPRIGHT] = {240, 0, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_RIGHT] = {240, 80, TILE_WIDTH, TILE_HEIGHT};
        // gTileClips[TILE_BOTTOMRIGHT] = {240, 160, TILE_WIDTH, TILE_HEIGHT};
    }
    map.close();
    return success;
}

bool Game::setPlayer()
{
    player = Player(100.0, 0.0, gTexture[PLAYER_TEXTURE], gTexture[FIRE_ATTACK_TEXTURE]);
    return true;
}

bool Game::loadMedia()
{
    bool success = true;
    gTexture[PLAYER_TEXTURE] = window.loadFromFile("images/knight-sheet.png");
    if (gTexture[PLAYER_TEXTURE] == NULL)
    {
        printf("Failed to load dot texture\n");
        success = false;
    }
    // printf("%d\n", (gRenderer != NULL));
    gTexture[TILE_TEXTURE] = window.loadFromFile("images/map/tile.png");
    if (gTexture[TILE_TEXTURE] == NULL)
    {
        printf("Failed to load tile texture\n");
        success = false;
    }
    gTexture[FIRE_ATTACK_TEXTURE] = window.loadFromFile("images/attack/attack3.png");
    if (gTexture[FIRE_ATTACK_TEXTURE] == NULL)
    {
        printf("Failed to load fire attack texture\n");
        success = false;
    }
    if (!setTiles(tiles))
    {
        printf("Failed to load tile set\n");
        success = false;
    }
    if (!setPlayer())
    {
        printf("Failed to set dot\n");
        success = false;
    }
    return success;
}

bool Game::isRunning()
{
    return running;
}

void Game::handleEvent(SDL_Event &event)
{
    if (event.type == SDL_QUIT)
        running = false;
    player.handleEvent(event);
}

void Game::renderGame()
{
    window.clearRenderer();
    player.move(tiles);
    player.setCamera(camera);
    for (int i = 0; i < TOTAL_TILES; i++)
        tiles[i].render(window, camera, &gTileClips[tiles[i].getType()]);
    player.render(window, camera);
    

    window.renderPresent();
}

void Game::close()
{
    window.close();
    for (SDL_Texture *mTexture : gTexture)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
}