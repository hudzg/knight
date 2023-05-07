#include "player-skill.h"

FireAttack::FireAttack(SDL_Texture *mTexture) : Entity(0, 0, mTexture)
{
    cntFrames = 0;
    for (int i = 0, x = 0; i < TOTAL_ATTACK_SPRITES; i++, x += FIRE_ATTACK_TEXTURE_WIDTH)
        gClips[i] = {x, 0, FIRE_ATTACK_TEXTURE_WIDTH, FIRE_ATTACK_TEXTURE_HEIGHT};
}

void FireAttack::render(RenderWindow &window, int x, int y, SDL_Rect mBox, SDL_RendererFlip flip)
{
    mBox.w = FIRE_ATTACK_WIDTH;
    mBox.h = FIRE_ATTACK_HEIGHT;
    // SDL_Rect tmpBox2 = {x, y, mBox.w, mBox.h};
    // window.renderBox(tmpBox2);
    window.renderPlayer(getTexture(), x, y, mBox, &gClips[cntFrames / 2], 0.0, NULL, flip);
    cntFrames++;
    if (cntFrames >= TOTAL_ATTACK_SPRITES * 2)
        cntFrames = 0;
}

HammerGodSkill::HammerGodSkill(SDL_Texture *mTexture) : Entity(0, 0, mTexture)
{
    cntFrames = 0;
    for (int i = 0, x = 0; i < TOTAL_ATTACK_SPRITES; i++, x += HAMMER_GOD_SKILL_TEXTURE_WIDTH)
        gClips[i] = {x, 0, HAMMER_GOD_SKILL_TEXTURE_WIDTH, HAMMER_GOD_SKILL_TEXTURE_HEIGHT};
    mBox = {0, 0, HAMMER_GOD_SKILL_RENDER_WIDTH, HAMMER_GOD_SKILL_RENDER_HEIGHT};
    isUnlock = false;
    // isUnlock = true;
}

// void HammerGodSkill::render(RenderWindow &window, int x, int y, SDL_Rect mBox, SDL_RendererFlip flip)
// {
//     mBox.w = HAMMER_GOD_SKILL_RENDER_WIDTH;
//     mBox.h = HAMMER_GOD_SKILL_RENDER_HEIGHT;
//     if(flip == SDL_FLIP_NONE) x += HAMMER_GOD_SKILL_RENDER_WIDTH / 6;
//     else x -= HAMMER_GOD_SKILL_RENDER_WIDTH / 6;
//     // SDL_Rect tmpBox2 = {x, y, mBox.w, mBox.h};
//     // window.renderBox(tmpBox2);
//     window.renderPlayer(getTexture(), x, y, mBox, &gClips[cntFrames / 6], 0.0, NULL, (flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
//     cntFrames++;
//     if (cntFrames >= TOTAL_ATTACK_SPRITES * 6 - 2)
//         cntFrames = 0;
// }

void HammerGodSkill::setPos(int x, int y, SDL_RendererFlip flip)
{
    if (flip == SDL_FLIP_NONE)
        x += HAMMER_GOD_SKILL_RENDER_WIDTH / 6;
    else
        x -= HAMMER_GOD_SKILL_RENDER_WIDTH / 6;
    this->x = x;
    this->y = y;
    this->flip = flip;
}

void HammerGodSkill::render(RenderWindow &window, SDL_Rect &camera)
{
    // SDL_Rect tmpBox2 = {x, y, mBox.w, mBox.h};
    // window.renderBox(tmpBox2);
    window.renderPlayer(getTexture(), x - camera.x, y - camera.y, mBox, &gClips[cntFrames / 6], 0.0, NULL, (flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
    cntFrames++;
    if (cntFrames >= TOTAL_ATTACK_SPRITES * 6 - 2)
        cntFrames = 0;
}

SDL_Rect HammerGodSkill::getAttackBox()
{
    SDL_Rect tmpBox = {x, y, mBox.w - mBox.w / 5, mBox.h};
    if (flip == SDL_FLIP_NONE)
        tmpBox.x += mBox.w - tmpBox.w - mBox.w / 6;
    else
        tmpBox.x += mBox.w / 6;
    return tmpBox;
}

bool HammerGodSkill::getIsUnlock()
{
    return isUnlock;
}

void HammerGodSkill::setUnlock()
{
    isUnlock = true;
}

void HammerGodSkill::Reset()
{
    cntFrames = 0;
}