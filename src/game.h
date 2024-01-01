
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
    MENU_STATE_IDLE,
    MENU_STATE_OPTIONS,
    MENU_STATE_LICENSES,
} MenuSubstate;

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
    bool draw_fps;
    float master_volume;
    float music_volume;
    float sound_volume;
    int keyboard_key_mv_up;
    int keyboard_key_mv_dn;
    int keyboard_key_mv_lt;
    int keyboard_key_mv_rt;
    int keyboard_key_fr_up;
    int keyboard_key_fr_dn;
    int keyboard_key_fr_lt;
    int keyboard_key_fr_rt;
    int keyboard_key_wp_lt;
    int keyboard_key_wp_rt;
    char keyboard_char_mv_up[2];
    char keyboard_char_mv_dn[2];
    char keyboard_char_mv_lt[2];
    char keyboard_char_mv_rt[2];
    char keyboard_char_fr_up[2];
    char keyboard_char_fr_dn[2];
    char keyboard_char_fr_lt[2];
    char keyboard_char_fr_rt[2];
    char keyboard_char_wp_lt[2];
    char keyboard_char_wp_rt[2];
} GameOptions;

typedef struct {
    MenuSubstate state;
    GameOptions options;
    int license_shown;
} MenuData;

// maybe this should be a static
// global in level.c
// with an init and deinit thing
typedef struct {
    size_t level;
    size_t wave;
    float game_start_time;
    float level_start_time;
    float next_infinite_spawn;
    float infinite_spawn_timer;
} GameLevelState;

typedef struct {
    float game_over_start_time;
    Vector2 time_pos_start;
    Vector2 time_pos_end;
    Vector2 score_pos_start;
    Vector2 score_pos_end;
} GameOverState;

typedef struct {
    float game_time;
    float game_delta;
    Music game_music;
    float game_music_desired_volume;
    float game_music_current_volume;
    MenuData menu_data;
    GameMenuState game_menu_state;
    GamePlayState game_play_state;
    GameLevelState game_level_state;
    GameOverState game_over_state;
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
void game_init_game_over();

void game_set_menu_state(GameMenuState new_gs);
GameMenuState game_get_menu_state();
void game_set_play_state(GamePlayState new_gs);
GamePlayState game_get_play_state();
void game_quit();
Entity * game_get_player_entity();
Entity * game_get_globe_entity();
vector * game_get_other_entities();
Weapon * game_get_weapon(WeaponType w);
Camera game_get_camera();
GameOptions game_get_game_options();

#endif
