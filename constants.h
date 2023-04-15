#ifndef CONSTANTS_H
#define CONSTANTS_H

const int LEVEL_WIDTH = 5120;
const int LEVEL_HEIGHT = 1280;

const int SCREEN_WIDTH = 1368;
const int SCREEN_HEIGHT = 768;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TILE_TEXTURE_WIDTH = 64;
const int TILE_TEXTURE_HEIGHT = 64;
const int TOTAL_TILES = 1600;
const int TILE_SET_WIDTH = 960;
const int TOTAL_WALL_TILES = 16;

enum TextureID
{
    PLAYER_TEXTURE,
    TILE_TEXTURE,
    FIRE_ATTACK_TEXTURE,
    SKELETON_TEXTURE,
    HP_TEXTURE,
    TOTAL_TEXTURE
};

enum TileSprites
{
    TILE_RED,
    TILE_GREEN,
    TILE_BLUE,
    TILE_CENTER,
    TILE_TOP,
    TILE_TOPRIGHT,
    TILE_RIGHT,
    TILE_BOTTOMRIGHT,
    TILE_BOTTOM,
    TILE_BOTTOMLEFT,
    TILE_LEFT,
    TILE_TOPLEFT,
    TOTAL_TILE_SPRITES = 270
};

#endif