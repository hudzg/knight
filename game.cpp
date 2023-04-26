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
    std::ifstream map("images/map/map2/map.map");
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
                printf("Failed to load map tile %d\n", i);
                success = false;
                break;
            }
            if (tileType >= 0 && tileType < TOTAL_TILE_SPRITES)
            {
                tiles[i] = Tile(x, y, gTexture[TILE_TEXTURE], tileType, wallID.count(tileType));
                if(tileType == TILE_TRAP)
                    traps.push_back(tiles[i].getBox());
            }
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
    player = Player(0.0, 520.0, gTexture[PLAYER_TEXTURE], gTexture[FIRE_ATTACK_TEXTURE], gTexture[HP_TEXTURE]);
    return true;
}

bool Game::setSkeleton()
{
    skeletonFamily = SkeletonFamily(gTexture[SKELETON_TEXTURE]);
    return true;
}

bool Game::setBoss()
{
    boss = Boss(9300.0, 500.0, gTexture[BOSS_TEXTURE]);
    return true;
}

bool Game::setMenu()
{
    menu = Menu(gTexture[MENU_BACKGROUND_TEXTURE], gTexture[MENU_BUTTON_TEXTURE], gTexture[MENU_TITLE_TEXTURE]);
    pauseMenu = SubMenu(0, gTexture[SUBMENU_BACKGROUND_TEXTURE], gTexture[SUBMENU_BUTTON_TEXTURE], gTexture[SUBMENU_TITLE_TEXTURE], 1);
    winMenu = SubMenu(160, gTexture[SUBMENU_BACKGROUND_TEXTURE], gTexture[SUBMENU_BUTTON_TEXTURE], gTexture[SUBMENU_TITLE_TEXTURE]);
    gameOverMenu = SubMenu(320, gTexture[SUBMENU_BACKGROUND_TEXTURE], gTexture[SUBMENU_BUTTON_TEXTURE], gTexture[SUBMENU_TITLE_TEXTURE]);
    return true;
}

bool Game::setDoor()
{
    doors.clear();
    doors.push_back(Door(31 * 64, 7 * 64, gTexture[DOOR_TEXTURE], 7));
    doors.push_back(Door(60 * 64, 13 * 64, gTexture[DOOR_TEXTURE], 7 + 13));
    doors.push_back(Door(107 * 64, 0 * 64, gTexture[DOOR_TEXTURE], 7 + 13 + 11));
    doors.push_back(Door(1146 * 64, 0 * 64, gTexture[DOOR_TEXTURE], 7 + 13 + 11 + 8));
    return true;
}

bool Game::setDynamicObject()
{
    if (!setPlayer())
    {
        printf("Failed to set player\n");
        return false;
    }
    if(!setDoor())
    {
        printf("Failed to set door\n");
        return false;
    }
    if(!setSkeleton())
    {
        printf("Failed to set skeleton\n");
        return false;
    }
    if(!setBoss())
    {
        printf("Failed to set boss\n");
        return false;
    }
    return true;
}

void Game::setCursor()
{
    SDL_Surface* cursorSurface = IMG_Load("images/gui/cursor/cursor.png");

    // Tạo con trỏ chuột mới từ biểu tượng chuột tải lên
    if(cursorSurface == NULL)
    {
        printf("h\n");
    }
    // Thiết lập biểu tượng chuột mới
    cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    SDL_SetCursor(cursor);
    if(cursor == NULL)
    {
        printf("h\n");
    }

    // Giải phóng bộ nhớ và đóng SDL
    // SDL_FreeCursor(cursor);
    SDL_FreeSurface(cursorSurface);
}

bool Game::loadMedia()
{
    bool success = true;
    // gTexture[PLAYER_TEXTURE] = window.loadFromFile("images/knight2.png");
    gTexture[PLAYER_TEXTURE] = window.loadFromFile("images/knight/knight.png");
    if (gTexture[PLAYER_TEXTURE] == NULL)
    {
        printf("Failed to load dot texture\n");
        success = false;
    }
    // printf("%d\n", (gRenderer != NULL));
    gTexture[TILE_TEXTURE] = window.loadFromFile("images/map/map2/tile.png");
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
    gTexture[SUBMENU_BACKGROUND_TEXTURE] = window.loadFromFile("images/gui/sub-menu/background.png");
    if (gTexture[SUBMENU_BACKGROUND_TEXTURE] == NULL)
    {
        printf("Failed to load sub menu background texture\n");
        success = false;
    }
    gTexture[SUBMENU_BUTTON_TEXTURE] = window.loadFromFile("images/gui/sub-menu/button.png");
    if (gTexture[SUBMENU_BUTTON_TEXTURE] == NULL)
    {
        printf("Failed to load sub menu button texture\n");
        success = false;
    }
    gTexture[SUBMENU_TITLE_TEXTURE] = window.loadFromFile("images/gui/sub-menu/title.png");
    if (gTexture[SUBMENU_TITLE_TEXTURE] == NULL)
    {
        printf("Failed to load sub menu title texture\n");
        success = false;
    }
    gTexture[DOOR_TEXTURE] = window.loadFromFile("images/object/door.png");
    if (gTexture[DOOR_TEXTURE] == NULL)
    {
        printf("Failed to load door texture\n");
        success = false;
    }
    if (!setTiles(tiles))
    {
        printf("Failed to load tile set\n");
        success = false;
    }
    if(!setMenu())
    {
        printf("Failed to set menu\n");
        success = false;
    }
    // if(!setDoor())
    // {
    //     printf("Failed to set door\n");
    //     success = false;
    // }
    if(!setDynamicObject())
    {
        printf("Failed to set dynamic object\n");
        success = false;
    }
    setCursor();
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
    if(state == STATE_PLAY)
    {
        setDynamicObject();
    }
}
void Game::renderMenu()
{
    window.clearRenderer();
    menu.render(window);
    window.renderPresent();
}

void Game::handlePauseMenuEvent(SDL_Event &event)
{
    if (event.type == SDL_QUIT)
        running = false;
    pauseMenu.handleEvent(event, state);
    if(state == STATE_AGAIN)
    {
        state = STATE_PLAY;
        setDynamicObject();
    }
}

void Game::renderPauseMenu()
{
    window.clearRenderer();
    pauseMenu.render(window);
    window.renderPresent();
}

void Game::handleGameOverMenuEvent(SDL_Event &event)
{
    if (event.type == SDL_QUIT)
        running = false;
    gameOverMenu.handleEvent(event, state);
    if(state == STATE_AGAIN)
    {
        state = STATE_PLAY;
        setDynamicObject();
    }
}

void Game::renderGameOverMenu()
{
    window.clearRenderer();
    gameOverMenu.render(window);
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
    player.move(tiles, doors);
    player.setCamera(camera);


    for (int i = 0; i < TOTAL_TILES; i++)
        tiles[i].render(window, camera, &gTileClips[tiles[i].getType()]);

    for (int i = 0; i < doors.size(); i++)
    {
        doors[i].setCanOpen(skeletonFamily.getCntSkeletonDied());
        doors[i].render(window, camera);
    }

    skeletonFamily.move(tiles, player.getBox(), doors);
    skeletonFamily.render(window, camera);
    skeletonFamily.checkDied();

    boss.move(tiles, player.getBox());
    boss.render(window, camera);

    player.attacked(skeletonFamily.getCountAttack(player.getBox()));
    player.attacked(boss.getAttack(player.getBox()));
    player.checkCollisionTrap(traps);
    player.render(window, camera, skeletonFamily, boss, doors);

    if(player.getHP() == 0) state = STATE_GAME_OVER_MENU;
    
    window.renderPresent();
}

int Game::getState()
{
    return state;
}

void Game::close()
{
    window.close();
    SDL_FreeCursor(cursor);
    for (SDL_Texture *mTexture : gTexture)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
}