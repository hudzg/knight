#include "menu.h"

Menu::Menu(SDL_Texture *backgroundTexture, SDL_Texture *buttonTexture, SDL_Texture *titleTexture)
{
    gTexture[BACKGROUND_TEXTURE] = backgroundTexture;
    gTexture[BUTTON_TEXTURE] = buttonTexture;
    gTexture[TITLE_TEXTURE] = titleTexture;
    backgroundBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    titleBox = {SCREEN_WIDTH / 2 - MENU_TITLE_WIDTH / 2, 100, MENU_TITLE_WIDTH, MENU_TITLE_HEIGHT};
    for (int i = 0, y = 300; i < TOTAL_BUTTON; i++, y += MENU_BUTTON_HEIGHT + MENU_BUTTON_HEIGHT / 4)
    {
        mouseover[i] = 0;
        buttonBox[i] = {SCREEN_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
    }

    for (int i = 0, y = 0; i < TOTAL_MENU_TITLE_SPRITES; i++, y += MENU_TITLE_TEXTURE_HEIGHT)
        titleClips[i] = {0, y, MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_TEXTURE_HEIGHT};
    for (int i = 0, y = 0; i < TOTAL_BUTTON; i++, y += MENU_BUTTON_TEXTURE_HEIGHT)
    {
        buttonClips[i][0] = {0, y, MENU_BUTTON_TEXTURE_WIDTH, MENU_BUTTON_TEXTURE_HEIGHT};
        buttonClips[i][1] = {MENU_BUTTON_TEXTURE_WIDTH, y, MENU_BUTTON_TEXTURE_WIDTH, MENU_BUTTON_TEXTURE_HEIGHT};
    }

    cntTitleFrames = 0;
}

void Menu::handleEvent(SDL_Event &e, GameState &state)
{
    // for (int i = 0; i < TOTAL_BUTTON; i++)
    //     mouseover[i] = 0;
    if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
    {
        for (int i = 0; i < TOTAL_BUTTON; i++)
            mouseover[i] = 0;
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < TOTAL_BUTTON; i++)
        {
            bool inside = 0;
            if (buttonBox[i].x <= x && x <= buttonBox[i].x + buttonBox[i].w && buttonBox[i].y <= y && y <= buttonBox[i].y + buttonBox[i].h)
                inside = 1;
            if (inside)
            {
                // printf("h\n");
                // mouseover[i] = 1;
                switch (e.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT)
                    {
                        if (i == PLAY)
                            state = STATE_PLAY;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    mouseover[i] = 1;
                    break;
                default:
                    // mouseover[i] = 0;
                    break;
                }
            }
        }
    }
}

void Menu::render(RenderWindow &window)
{
    window.renderPlayer(gTexture[BACKGROUND_TEXTURE], 0, 0, backgroundBox);
    for (int i = 0; i < TOTAL_BUTTON; i++)
        window.renderPlayer(gTexture[BUTTON_TEXTURE], buttonBox[i].x, buttonBox[i].y, buttonBox[i], &buttonClips[i][mouseover[i]]);
    window.renderPlayer(gTexture[TITLE_TEXTURE], titleBox.x, titleBox.y, titleBox, &titleClips[cntTitleFrames / 8]);
    cntTitleFrames++;
    if (cntTitleFrames >= TOTAL_MENU_TITLE_SPRITES * 8)
        cntTitleFrames = 0;
}

SubMenu::SubMenu(int y, SDL_Texture *backgroundTexture, SDL_Texture *buttonTexture, SDL_Texture *titleTexture, int haveResume)
{
    gTexture[BACKGROUND_TEXTURE] = backgroundTexture;
    gTexture[BUTTON_TEXTURE] = buttonTexture;
    gTexture[TITLE_TEXTURE] = titleTexture;
    backgroundBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    titleBox = {SCREEN_WIDTH / 2 - MENU_TITLE_WIDTH / 2, 250, MENU_TITLE_WIDTH, MENU_TITLE_HEIGHT};
    titleClips = {0, y, MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_TEXTURE_HEIGHT};
    // for (int i = 0, y = 300; i < TOTAL_BUTTON; i++, y += MENU_BUTTON_HEIGHT + MENU_BUTTON_HEIGHT / 4)
    // {
    //     mouseover[i] = 0;
    //     buttonBox[i] = {SCREEN_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
    // }

    for (int i = 0, x = SCREEN_WIDTH / 2 - ((2 + haveResume) * MENU_BUTTON_WIDTH + (1 + haveResume) * MENU_BUTTON_WIDTH / 4) / 2; i < TOTAL_BUTTON; i++)
    {
        mouseover[i] = 0;
        if (i != RESUME || (haveResume && i == RESUME))
        {
            buttonBox[i] = {x, 450, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
            x += MENU_BUTTON_WIDTH + MENU_BUTTON_WIDTH / 4;
        }
    }

    for (int i = 0, y = 0; i < TOTAL_BUTTON; i++, y += MENU_BUTTON_TEXTURE_HEIGHT)
    {
        buttonClips[i][0] = {0, y, MENU_BUTTON_TEXTURE_WIDTH, MENU_BUTTON_TEXTURE_HEIGHT};
        buttonClips[i][1] = {MENU_BUTTON_TEXTURE_WIDTH, y, MENU_BUTTON_TEXTURE_WIDTH, MENU_BUTTON_TEXTURE_HEIGHT};
    }
    this->haveResume = haveResume;
}

void SubMenu::handleEvent(SDL_Event &e, GameState &state)
{
    // for (int i = 0; i < TOTAL_BUTTON; i++)
    //     mouseover[i] = 0;
    if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
    {
        for (int i = 0; i < TOTAL_BUTTON; i++)
            mouseover[i] = 0;
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < TOTAL_BUTTON; i++)
        {
            if (!(i != RESUME || (haveResume && i == RESUME)))
                continue;
            bool inside = 0;
            if (buttonBox[i].x <= x && x <= buttonBox[i].x + buttonBox[i].w && buttonBox[i].y <= y && y <= buttonBox[i].y + buttonBox[i].h)
                inside = 1;
            if (inside)
            {
                // printf("h\n");
                // mouseover[i] = 1;
                switch (e.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT)
                    {
                        if (i == RESUME)
                            state = STATE_PLAY;
                        if (i == AGAIN)
                            state = STATE_AGAIN;
                        if (i == HOME)
                            state = STATE_MENU;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    mouseover[i] = 1;
                    break;
                default:
                    // mouseover[i] = 0;
                    break;
                }
            }
        }
    }
}

void SubMenu::render(RenderWindow &window)
{
    window.renderPlayer(gTexture[BACKGROUND_TEXTURE], 0, 0, backgroundBox);
    for (int i = 0; i < TOTAL_BUTTON; i++)
        if (i != RESUME || (haveResume && i == RESUME))
            window.renderPlayer(gTexture[BUTTON_TEXTURE], buttonBox[i].x, buttonBox[i].y, buttonBox[i], &buttonClips[i][mouseover[i]]);
    window.renderPlayer(gTexture[TITLE_TEXTURE], titleBox.x, titleBox.y, titleBox, &titleClips);
}