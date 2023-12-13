
#ifndef AA_GAME_H
#define AA_GAME_H

#include <stdbool.h>

#include "raylib.h"
#include "vector.h"
#include "entity.h"

typedef enum {
    GAME_MENU_STATE_INIT,
    GAME_MENU_STATE_MENU,
    GAME_MENU_STATE_PLAY,
} GameMenuState;

typedef enum {
    GAME_PLAY_STATE_INIT,
    GAME_PLAY_STATE_PLAY,
    GAME_PLAY_STATE_OVER,
} GamePlayState;

typedef struct {
    Entity player;
    Entity globe;
    vector * others;
} GameEntities;

// maybe this should be a static
// global in level.c
// with an init and deinit thing
typedef struct {
    size_t level;
    size_t wave;
    float level_start_time;
} GameLevelState;

typedef struct {
    float game_time;
    float game_delta;
    GameMenuState game_menu_state;
    GamePlayState game_play_state;
    GameLevelState game_level_state;
    GameEntities game_entities;
    Camera3D game_camera;
} Game;

extern int GAME_SCREEN_WIDTH;
extern int GAME_SCREEN_HEIGHT;

float game_get_time();
float game_get_delta();
void game_update();
void game_set_should_quit();
bool game_get_should_quit();

void game_set_menu_state(GameMenuState new_gs);
GameMenuState game_get_menu_state();
void game_set_play_state(GamePlayState new_gs);
GamePlayState game_get_play_state();
void game_quit();
Entity * game_get_player_entity();
Entity * game_get_globe_entity();
vector * game_get_other_entities();

#endif