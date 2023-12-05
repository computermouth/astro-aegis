
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

typedef struct {
    float game_time;
    float game_delta;
    GameMenuState game_menu_state;
    GamePlayState game_play_state;
    GameEntities game_entities;
    Camera3D game_camera;
} Game;

extern int GAME_SCREEN_WIDTH;
extern int GAME_SCREEN_HEIGHT;

float game_get_time();
float game_get_delta();
void game_update();
void game_set_state(GameMenuState);
bool game_get_should_quit();
GameMenuState game_get_state();
void game_quit();

#endif