#include "buff-effect.h"

BuffEffect::BuffEffect()
{
    cntFrames = 0;
    isBuff = false;
    isEnd = false;
}

void BuffEffect::render(RenderWindow &window, SDL_Rect &camera, SDL_Rect &playerBox, SDL_RendererFlip flip)
{
    if(isBuff == false) return;
    int x = playerBox.x + playerBox.w / 2 - mBox.w / 2;
    int y = playerBox.y + playerBox.h / 2 - mBox.h / 2;
    x -= camera.x;
    y -= camera.y;
    window.renderPlayer(mTexture, x, y, mBox, &gClips[cntFrames / 4], 0.0, NULL, flip);
    cntFrames++;
    if(cntFrames >= gClips.size() * 4)
    {
        cntFrames = 0;
        isBuff = false;
        isEnd = true;
    }
}

bool BuffEffect::checkIsEnd()
{
    if(isEnd)
    {
        isEnd = false;
        return true;
    }
    return false;
}

void BuffEffect::setIsBuff()
{
    isBuff = true;
}

HPBuff::HPBuff(SDL_Texture *mTexture)
{
    this->mTexture = mTexture;
    mBox = {0, 0, HP_BUFF_WIDTH, HP_BUFF_HEIGHT};
    for(int i = 0, x = 0; i < TOTAL_HP_BUFF_SPRITES; i++, x += HP_BUFF_TEXTURE_WIDTH)
        gClips.push_back({x, 0, HP_BUFF_TEXTURE_WIDTH, HP_BUFF_TEXTURE_HEIGHT});
    for(int i = 0, x = 0; i < TOTAL_HP_BUFF_SPRITES; i++, x += HP_BUFF_TEXTURE_WIDTH)
        gClips.push_back({x, 0, HP_BUFF_TEXTURE_WIDTH, HP_BUFF_TEXTURE_HEIGHT});
}

ATKBuff::ATKBuff(SDL_Texture *mTexture)
{
    this->mTexture = mTexture;
    mBox = {0, 0, ATK_BUFF_WIDTH, ATK_BUFF_HEIGHT};
    for(int i = 0, x = 0; i < TOTAL_ATK_BUFF_SPRITES; i++, x += ATK_BUFF_TEXTURE_WIDTH)
        gClips.push_back({x, 0, ATK_BUFF_TEXTURE_WIDTH, ATK_BUFF_TEXTURE_HEIGHT});
    for(int i = 0, x = 0; i < TOTAL_ATK_BUFF_SPRITES; i++, x += ATK_BUFF_TEXTURE_WIDTH)
        gClips.push_back({x, 0, ATK_BUFF_TEXTURE_WIDTH, ATK_BUFF_TEXTURE_HEIGHT});
}

SkillUnlock::SkillUnlock(SDL_Texture *mTexture)
{
    this->mTexture = mTexture;
    mBox = {0, 0, SKILL_UNLOCK_WIDTH, SKILL_UNLOCK_HEIGHT};
    for(int i = 0, x = 0; i < TOTAL_SKILL_UNLOCK_SPRITES; i++, x += SKILL_UNLOCK_TEXTURE_WIDTH)
        gClips.push_back({x, 0, SKILL_UNLOCK_TEXTURE_WIDTH, SKILL_UNLOCK_TEXTURE_HEIGHT});
    for(int i = 0, x = 0; i < TOTAL_SKILL_UNLOCK_SPRITES; i++, x += SKILL_UNLOCK_TEXTURE_WIDTH)
        gClips.push_back({x, 0, SKILL_UNLOCK_TEXTURE_WIDTH, SKILL_UNLOCK_TEXTURE_HEIGHT});
}