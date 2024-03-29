
#include <stdio.h>

#include "entity_player.h"
#include "level.h"
#include "entity.h"
#include "entity_asteroid.h"
#include "game.h"
#include "raylib.h"
#include "resource.h"
#include "vector.h"
#include "entity_banner.h"

// todo, option-ize
typedef struct {
    float time;
    size_t count;
    AsteroidSize size;
    AsteroidColor color;
} level_spawnset;

level_spawnset levels[5][10] = {
    // level 1 (small reds)
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_SM, ASTEROID_COLOR_RED },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        // wave 2
        { .time = 23.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
    },
    // level 2 (small greens)
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_SM, ASTEROID_COLOR_GREEN },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        // wave 2
        { .time = 23.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        { .time = 23.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
    },
    // level 3 (small blue + small reds)
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_SM, ASTEROID_COLOR_BLUE },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 2
        { .time = 23.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        { .time = 23.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 2
        { .time = 43.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        { .time = 43.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { 0 },
        { 0 },
        { 0 },
        { 0 },
    },
    // level 4 (bit of everything)
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_SM, ASTEROID_COLOR_RED },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        // wave 2
        { .time = 23.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        { .time = 23.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        // wave 3
        { .time = 43.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        { .time = 43.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 4
        { .time = 63.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_GREEN },
        { .time = 63.0, .count = 2, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { 0 },
        { 0 },
    },
    // level 5 (more everything)
    {
        // wave 1
        { .time = 3.0, .count = 3, ASTEROID_SIZE_SM, ASTEROID_COLOR_RED },
        { .time = 3.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        // wave 2
        { .time = 33.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        { .time = 33.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        { .time = 33.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 3
        { .time = 63.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_GREEN },
        { .time = 63.0, .count = 2, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { .time = 63.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        { .time = 63.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
    },
};

Sound * level_announcements[] = {
    &level_1_snd,
    &level_2_snd,
    &level_3_snd,
    &level_4_snd,
    &level_5_snd,
    &level_infinite_mode_snd
};

void level_update(vector * entities, GameLevelState * gls){
    
    bool no_asteroids = true;
    size_t elen = vector_size(entities);
    Entity * es = vector_begin(entities);
    for(size_t i = 0; i < elen; i++){
        if (es[i].type == ENTITY_TYPE_ASTEROID)
            no_asteroids = false;
    }

    // set vars for fast clear
    float next = 30;
    if (gls->level < 5 && levels[gls->level][gls->wave].time != 0)
        next = levels[gls->level][gls->wave].time;
    if (no_asteroids && gls->wave != 0 && gls->level_start_time + next - gls->total_clear_offset > game_get_time()){
        float diff = (gls->level_start_time + gls->total_clear_offset + next) - game_get_time();
        gls->total_clear_offset += diff;
        PlaySound(fast_clear_bonus_snd);
        Entity e = entity_banner_spawn(BANNER_TTS_FAST_CLEAR, (Vector2){GetScreenWidth() - 32, 100});
        vector *oe = game_get_other_entities();
        vector_push(oe, &e);
        entity_player_add_score(diff * 5);
    }

    // watch for level change
    if (gls->wave == 9) {
        // if level > levels
        // either finish
        // or engage infinite mode

        if (no_asteroids && gls->level != 4) {
            gls->level++;
            gls->total_clear_offset = 0;
            PlaySound(*level_announcements[gls->level]);
            Entity e = entity_banner_spawn(BANNER_TTS_LVL_ONE + gls->level, (Vector2){GetScreenWidth() - 32, 100});
            vector *oe = game_get_other_entities();
            vector_push(oe, &e);
            gls->wave = 0;
            gls->level_start_time = game_get_time();
        } else if (no_asteroids && gls->level == 4) {
            // infinite mode
            gls->level++;
            PlaySound(*level_announcements[5]);
            Entity e = entity_banner_spawn(BANNER_TTS_LVL_INFINITE, (Vector2){GetScreenWidth() - 32, 100});
            vector *oe = game_get_other_entities();
            vector_push(oe, &e);
            gls->wave = 0;
            gls->level_start_time = game_get_time();
            gls->infinite_spawn_timer = 4;
            gls->next_infinite_spawn = game_get_time() + 3;
        }
        return;
    }

    // wave spawns
    while (gls->level <=4 && gls->level_start_time + levels[gls->level][gls->wave].time - gls->total_clear_offset < game_get_time()){
        if (gls->wave == 0){
            PlaySound(level_engage_snd);
            Entity e = entity_banner_spawn(BANNER_TTS_LVL_ENGAGE, (Vector2){GetScreenWidth() - 32, 100});
            vector *oe = game_get_other_entities();
            vector_push(oe, &e);
        }

        level_spawnset ls = levels[gls->level][gls->wave];
        for(size_t i = 0; i < ls.count; i++){
            Entity e = entity_asteroid_spawn(ls.size, ls.color, (Vector3){-3.5,0,0});
            vector_push(entities, &e);
        }
        gls->wave++;
        if (gls->wave == 9)
            return;
    }

    // infinite mode
    if (gls->level == 5 && game_get_time() > gls->next_infinite_spawn){
        gls->infinite_spawn_timer *= 0.99;
        gls->next_infinite_spawn += gls->infinite_spawn_timer;

        if (gls->infinite_spawn_timer == 4){
            PlaySound(level_engage_snd);
            Entity e = entity_banner_spawn(BANNER_TTS_LVL_ENGAGE, (Vector2){GetScreenWidth() - 32, 100});
            vector *oe = game_get_other_entities();
            vector_push(oe, &e);
        }

        Entity e;
        switch (GetRandomValue(1, 5)){
            case 1: // 1LG
                e = entity_asteroid_spawn(ASTEROID_SIZE_LG, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                break;
            case 2: // 2MD
                e = entity_asteroid_spawn(ASTEROID_SIZE_MD, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                e = entity_asteroid_spawn(ASTEROID_SIZE_MD, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                break;
            case 3: // 3SM
                e = entity_asteroid_spawn(ASTEROID_SIZE_SM, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                e = entity_asteroid_spawn(ASTEROID_SIZE_SM, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                e = entity_asteroid_spawn(ASTEROID_SIZE_SM, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                break;
            case 4: // 1LG - 1SM
                e = entity_asteroid_spawn(ASTEROID_SIZE_LG, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                e = entity_asteroid_spawn(ASTEROID_SIZE_SM, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                break;
            case 5: // 1LG - 1MD - 1SM
                e = entity_asteroid_spawn(ASTEROID_SIZE_LG, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                e = entity_asteroid_spawn(ASTEROID_SIZE_MD, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                e = entity_asteroid_spawn(ASTEROID_SIZE_SM, GetRandomValue(0, 2), (Vector3){-3.5,0,0});
                vector_push(entities, &e);
                break;
        }
    }
}

char game__time_label[100];
char level_time_label[100];
char level______label[100];
char wave_______label[100];

void level_draw_2d(GameLevelState gls){
    sprintf(game__time_label, "time: %.2f", game_get_time() - gls.game_start_time);
    if (gls.level != 5)
        sprintf(level______label, "level: %lu", gls.level + 1);
    else
        sprintf(level______label, "level: INFINITE");
    sprintf(level_time_label, "level_time: %.2f", game_get_time() - gls.level_start_time);
    // sprintf(wave_______label, "wave : %lu", gls.wave);
    DrawText(game__time_label, 32,  32, 32, RAYWHITE);
    DrawText(level______label, 32, 64, 32, RAYWHITE);
    DrawText(level_time_label, 32, 96, 32, RAYWHITE);
    // DrawText(wave_______label, 32, 128, 32, RAYWHITE);
}
