#if !defined(PLAYER_H)
#define PLAYER_H

#include <SDL.h>
#include "tile.h"
#include "constants.h"
#include "entity.h"
#include "utils.h"
#include "skeleton.h"
#include "boss.h"
#include "door.h"
#include <math.h>
#include "player-skill.h"
#include "secret-area.h"
#include "hud.h"
#include "buff-effect.h"

using namespace std;

class Player : public Entity
{
private:
    static const int TOTAL_PLAYER_WALK_SPRITES = 6;
    static const int TOTAL_PLAYER_IDLE_SPRITES = 4;
    static const int TOTAL_PLAYER_JUMP_SPRITES = 5;
    static const int TOTAL_PLAYER_FALL_SPRITES = 5;
    static const int TOTAL_PLAYER_ATTACK_SPRITES = 8;
    static const int TOTAL_PLAYER_DASH_SPRITES = 5;
    static const int TOTAL_PLAYER_TAKE_HIT_SPRITES = 4;
    double mVelX, mVelY;
    SDL_Rect mBox;
    bool onGround, isWalking, isJumping, isAttacking, isDashing, isTakeHit;
    int cntJump;
    int cntWalkFrames, cntIdleFrames, cntJumpFrames, cntFallFrames, cntAttackFrames, cntDashFrames, cntTakeHitFrames;
    int direction;
    SDL_Rect gPlayerIdleClips[TOTAL_PLAYER_IDLE_SPRITES];
    SDL_Rect gPlayerWalkClips[TOTAL_PLAYER_WALK_SPRITES];
    SDL_Rect gPlayerJumpClips[TOTAL_PLAYER_JUMP_SPRITES];
    SDL_Rect gPlayerFallClips[TOTAL_PLAYER_FALL_SPRITES];
    SDL_Rect gPlayerAttackClips[TOTAL_PLAYER_ATTACK_SPRITES];
    SDL_Rect gPlayerDashClips[TOTAL_PLAYER_DASH_SPRITES];
    SDL_Rect gPlayerTakeHitClips[TOTAL_PLAYER_TAKE_HIT_SPRITES];
    SDL_RendererFlip flip;
    FireAttack fireAttackAnimation;
    HammerGodSkill skill;
    PlayerPoint HP, MP, ATKP;
    bool isTakeTrap;
    int cntTimeTakeTrap;
    bool useSkill;
    vector<int> doorToAddHP;
    vector<int> doorToAddATK;
    HPBuff effectHPBuff;
    ATKBuff effectATKBuff;
    SkillUnlock effectSkillUnlock;
    int isLand;
    bool pressButtonA, pressButtonD;

public:
    static const int PLAYER_WIDTH = 64;
    static const int PLAYER_HEIGHT = 128;
    static const int PLAYER_TEXTURE_WIDTH = 480;
    static const int PLAYER_TEXTURE_HEIGHT = 320;
    static const int PLAYER_RENDER_WIDTH = 192;
    static const int PLAYER_RENDER_HEIGHT = 128;
    static const int PLAYER_VEL = 600;
    static const int PLAYER_MAX_CNT_JUMP = 20;
    static const int PLAYER_DASH_VEL_LEVEL = 2;
    static const int PLAYER_JUMP_VEL_LEVEL = 4;
    static const int GRAVITY_SPEED = 60;
    static const int MAX_FALL_SPEED = 600;
    static const int TILE_TRAP = 242;
    static const int MAX_TIME_TAKE_TRAP = 60;

    Player(float mPosX = 0, float mPosY = 0, SDL_Texture *mTexture = NULL, SDL_Texture *mFireAttackTexture = NULL, SDL_Texture *mHPTexture = NULL, SDL_Texture *mSkillTexture = NULL, SDL_Texture *mMPTexture = NULL, SDL_Texture *mATKPTexture = NULL, SDL_Texture *mHPBuffTexture = NULL, SDL_Texture *mATKBuffTexture = NULL, SDL_Texture *mSkillUnlockTexture = NULL);
    bool checkCollision(SDL_Rect &a, const SDL_Rect &b);
    bool checkCollisionWall(SDL_Rect &a, Tile *b);
    bool checkCollisionDoor(vector<Door> &doors);
    void checkCollisionTrap(const vector<SDL_Rect> &b, int &score);
    void handleEvent(SDL_Event &e, GameState &state, Mix_Chunk *sound[]);
    void move(Tile *tiles, vector<Door> &doors, SecretArea &secretArea, double timeStep = 1.0 / 60);
    void render(RenderWindow &window, SDL_Rect &camera, SkeletonFamily &skeletonFamily, Boss &boss, vector<Door> &doors, SecretArea &secretArea, Key &key, Chest &chest, Mix_Chunk *sound[], int &score);
    void renderEffect(RenderWindow &window, SDL_Rect &camera);
    void attacked(std::pair<int, int> value, int &score);
    void setCamera(SDL_Rect &camera);
    int getPosX();
    int getPosY();
    int getHP();
    SDL_Rect getBox();
    void setPressButton();
};

#endif // DOT_H