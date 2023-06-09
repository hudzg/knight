#include "utils.h"

bool RenderWindow::initSDL()
{
    bool success = 1;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init SDL, error: %s\n", SDL_GetError());
        success = 0;
    }
    else
    {
        gWindow = SDL_CreateWindow("hudzg", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Failed to create window, error: %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
            // set icon
            SDL_Surface *icon = IMG_Load("images/icon/icon.png");
            SDL_SetWindowIcon(gWindow, icon);
            SDL_FreeSurface(icon);
            // SDL_FreeSurface(resizedIcon);

            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Failed to render, error: %s\n", SDL_GetError());
                success = 0;
            }
            else
            {

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("Failed to init SDL_image, error: %sn", IMG_GetError());
                    success = 0;
                }
                if (TTF_Init() == -1)
                {
                    printf("Failed to init SDL_ttf, error: %s\n", TTF_GetError());
                    success = 0;
                }
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("Failed to init SDL_mixer, error: %s\n", Mix_GetError());
                    success = 0;
                }
            }
        }
    }
    return success;
}

// SDL_Renderer* getRenderer()
// {
//     return gRenderer;
// }

void RenderWindow::clearRenderer()
{
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
}

void RenderWindow::renderPresent()
{
    SDL_RenderPresent(gRenderer);
}

void RenderWindow::close()
{
    // TTF_CloseFont(gFont);
    // gFont = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

SDL_Texture *RenderWindow::loadFromFile(std::string path)
{
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
        printf("Failed to load %s img, error: %s", path.c_str(), IMG_GetError());
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
            printf("Failed to create texture %s, error: %s\n", path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void RenderWindow::renderTile(SDL_Texture *mTexture, int x, int y, SDL_Rect rect, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, rect.w, rect.h};
    // if (clip != NULL)
    // {
    //     renderQuad.w = clip->w;
    //     renderQuad.h = clip->h;
    // }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void RenderWindow::renderPlayer(SDL_Texture *mTexture, int x, int y, SDL_Rect rect, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, rect.w, rect.h};
    // if (clip != NULL)
    // {
    //     renderQuad.w = clip->w;
    //     renderQuad.h = clip->h;
    // }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void RenderWindow::renderHitbox(SDL_Texture *mTexture, int x, int y, SDL_Rect rect, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, rect.w, rect.h};
    // if (clip != NULL)
    // {
    //     renderQuad.w = clip->w;
    //     renderQuad.h = clip->h;
    // }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &renderQuad);
}

void RenderWindow::renderBox(SDL_Rect rect)
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &rect);
}

void RenderWindow::renderFillBox(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    SDL_RenderFillRect(gRenderer, &rect);
}

void RenderWindow::renderTextNoFont(const char *text, const char *fontPath, int fontSize, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
    if (font == NULL)
    {
        printf("Failed to load font, error: %s\n", TTF_GetError());
        return;
    }
    SDL_Color textColor = {r, g, b, a};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("Failed to create text surface, error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (textTexture == NULL)
    {
        printf("Failed to create texture, error: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect textRect = {x, y, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h); // lấy kích thước của texture
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    SDL_DestroyTexture(textTexture);
}

void RenderWindow::renderText(const char *text, TTF_Font *font, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color textColor = {r, g, b, a};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("Failed to create text surface, error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (textTexture == NULL)
    {
        printf("Failed to create texture, error: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect textRect = {x, y, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h); // lấy kích thước của texture
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void RenderWindow::renderTextRight(const char *text, TTF_Font *font, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color textColor = {r, g, b, a};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("Failed to create text surface, error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (textTexture == NULL)
    {
        printf("Failed to create texture, error: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect textRect = {x, y, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h); // lấy kích thước của texture
    textRect.x -= textRect.w;
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void RenderWindow::renderTextCenterScreen(const char *text, TTF_Font *font, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color textColor = {r, g, b, a};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("Failed to create text surface, error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (textTexture == NULL)
    {
        printf("Failed to create texture, error: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect textRect = {0, 0, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h); // lấy kích thước của texture
    textRect.x = SCREEN_WIDTH / 2 - textRect.w / 2;
    textRect.y = SCREEN_HEIGHT / 2 - textRect.h / 2;
    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}