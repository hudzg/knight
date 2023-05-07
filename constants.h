#ifndef CONSTANTS_H
#define CONSTANTS_H

const int MOD = 1000000007;

const int LEVEL_WIDTH = 5120 * 2;
const int LEVEL_HEIGHT = 1280;

const int SCREEN_WIDTH = 1368;
const int SCREEN_HEIGHT = 768;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TILE_TEXTURE_WIDTH = 64;
const int TILE_TEXTURE_HEIGHT = 64;
const int TOTAL_TILES = 1600 * 2;
const int TILE_SET_WIDTH = 960;
const int TOTAL_WALL_TILES = 16;


const double PI = 3.1415926535897932384626433832795;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int SCORE_SKELETON = 100;
const int SCORE_BOSS = 2000;
const int SCORE_UNLOCK_SKILL = 2000;

const int TOTAL_HIGH_SCORES = 5;

enum TextureID
{
    PLAYER_TEXTURE,
    TILE_TEXTURE,
    FIRE_ATTACK_TEXTURE,
    HAMMER_SKILL_TEXTURE,
    SKELETON_TEXTURE,
    HP_TEXTURE,
    MP_TEXTURE,
    ATKP_TEXTURE,
    BOSS_TEXTURE,
    FIRE_BALL_TEXTURE,
    MENU_BACKGROUND_TEXTURE,
    MENU_BUTTON_TEXTURE,
    MENU_TITLE_TEXTURE,
    GUIDE_MENU_BUTTON_TEXTURE,
    GUIDE_MENU_BUTTON_IMAGES_TEXTURE,
    SUBMENU_BACKGROUND_TEXTURE,
    SUBMENU_BUTTON_TEXTURE,
    SUBMENU_TITLE_TEXTURE,
    DOOR_TEXTURE,
    SECRET_AREA_TEXTURE,
    KEY_TEXTURE,
    CHEST_TEXTURE,
    HP_BUFF_TEXTURE,
    ATK_BUFF_TEXTURE,
    SKILL_UNLOCK_TEXTURE,
    TOTAL_TEXTURE
};

enum MenuSoundEffect
{
    SELECT_BUTTON_SOUND,
    TOTAL_MENU_SOUND_EFFECT
};

enum PlayerSoundEffect
{
    PLAYER_ATTACK_SOUND,
    PLAYER_BOOM_SOUND,
    PLAYER_DASH_SOUND,
    PLAYER_JUMP_SOUND,
    PLAYER_LAND_SOUND,
    PLAYER_WALK_SOUND,
    PLAYER_TAKE_HIT_SOUND,
    PLAYER_HP_BUFF_SOUND,
    PLAYER_ATK_BUFF_SOUND,
    PLAYER_PAUSE_SOUND,
    TOTAL_PLAYER_SOUND_EFFECT
};

enum BossSoundEffect
{
    BOSS_ATTACK_SOUND,
    BOSS_FIRE_SOUND,
    BOSS_BOOM_SOUND,
    TOTAL_BOSS_SOUND_EFFECT
};

enum Font
{
    FONT_40,
    FONT_80,
    FONT2_40,
    TOTAL_FONT
};

enum TileSprites
{
    TILE_TRAP = 242,
    TOTAL_TILE_SPRITES = 270
};

enum GameState
{
    STATE_PLAY,
    STATE_MENU,
    STATE_AGAIN,
    STATE_PAUSE_MENU,
    STATE_GAME_OVER_MENU,
    STATE_WIN_MENU,
    STATE_GUIDE_MENU,
    STATE_HIGH_SCORES_MENU,
    STATE_QUIT,
    TOTAL_STATE
};

#endif