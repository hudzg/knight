#include "game.h"

bool Game::init()
{
    camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    running = true;

    if(!window.initSDL()) return false;
    state = STATE_MENU;
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
    std::ifstream map("images/map/map2.map");
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
            // printf("%d\n", tileType);
            if (map.fail())
            {
                printf("Failed to load map atile %d\n", i);
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
    }
    map.close();
    return success;
}

bool Game::setPlayer()
{
    player = Player(200.0, 0.0, gTexture[PLAYER_TEXTURE], gTexture[FIRE_ATTACK_TEXTURE], gTexture[HP_TEXTURE]);
    return true;
}

bool Game::setSkeleton()
{
    skeletonFamily = SkeletonFamily(gTexture[SKELETON_TEXTURE]);
    return true;
}

bool Game::setBoss()
{
    boss = Boss(500.0, 0.0, gTexture[BOSS_TEXTURE]);
    return true;
}

bool Game::setMenu()
{
    menu = Menu(gTexture[MENU_BACKGROUND_TEXTURE], gTexture[MENU_BUTTON_TEXTURE], gTexture[MENU_TITLE_TEXTURE]);
    return true;
}

bool Game::loadMedia()
{
    bool success = true;
    gTexture[PLAYER_TEXTURE] = window.loadFromFile("images/knight2.png");
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
    gTexture[SKELETON_TEXTURE] = window.loadFromFile("images/skeleton/skeleton.png");
    if (gTexture[SKELETON_TEXTURE] == NULL)
    {
        printf("Failed to load fire skeleton texture\n");
        success = false;
    }
    gTexture[HP_TEXTURE] = window.loadFromFile("images/hp/hp.png");
    if (gTexture[HP_TEXTURE] == NULL)
    {
        printf("Failed to load fire hp texture\n");
        success = false;
    }
    gTexture[BOSS_TEXTURE] = window.loadFromFile("images/boss/boss.png");
    if (gTexture[BOSS_TEXTURE] == NULL)
    {
        printf("Failed to load boss texture\n");
        success = false;
    }
    gTexture[MENU_BACKGROUND_TEXTURE] = window.loadFromFile("images/gui/menu-background.png");
    if (gTexture[MENU_BACKGROUND_TEXTURE] == NULL)
    {
        printf("Failed to load menu background texture\n");
        success = false;
    }
    gTexture[MENU_BUTTON_TEXTURE] = window.loadFromFile("images/gui/button.png");
    if (gTexture[MENU_BUTTON_TEXTURE] == NULL)
    {
        printf("Failed to load menu button texture\n");
        success = false;
    }
    gTexture[MENU_TITLE_TEXTURE] = window.loadFromFile("images/gui/title.png");
    if (gTexture[MENU_TITLE_TEXTURE] == NULL)
    {
        printf("Failed to load menu title texture\n");
        success = false;
    }
    if (!setTiles(tiles))
    {
        printf("Failed to load tile set\n");
        success = false;
    }
    if (!setPlayer())
    {
        printf("Failed to set player\n");
        success = false;
    }
    if(!setSkeleton())
    {
        printf("Failed to set skeleton\n");
        success = false;
    }
    if(!setBoss())
    {
        printf("Failed to set boss\n");
        success = false;
    }
    if(!setMenu())
    {
        printf("Failed to set menu\n");
        success = false;
    }
    return success;
}

bool Game::isRunning()
{
    return running;
}

void Game::handleMenuEvent(SDL_Event &event)
{
    if (event.type == SDL_QUIT)
        running = false;
    menu.handleEvent(event, state);
}
void Game::renderMenu()
{
    window.clearRenderer();
    menu.render(window);
    window.renderPresent();
}

void Game::handleGameEvent(SDL_Event &event)
{
    if (event.type == SDL_QUIT)
        running = false;
    player.handleEvent(event, state);
}

void Game::renderGame()
{
    window.clearRenderer();
    player.move(tiles);
    player.setCamera(camera);


    for (int i = 0; i < TOTAL_TILES; i++)
        tiles[i].render(window, camera, &gTileClips[tiles[i].getType()]);

    skeletonFamily.move(tiles, player.getBox());
    skeletonFamily.render(window, camera);

    boss.move(tiles, player.getBox());
    boss.render(window, camera);

    player.attacked(skeletonFamily.getCountAttack(player.getBox()));
    player.attacked(boss.getAttack(player.getBox()));
    player.render(window, camera, skeletonFamily, boss);
    
    window.renderPresent();
}

int Game::getState()
{
    return state;
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