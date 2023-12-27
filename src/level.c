
#include <stdio.h>

#include "level.h"
#include "entity.h"
#include "entity_asteroid.h"
#include "game.h"
#include "raylib.h"
#include "resource.h"
#include "vector.h"

typedef struct {
    float time;
    size_t count;
    AsteroidSize size;
    AsteroidColor color;
} level_spawnset;

level_spawnset levels[3][6] = {
    // level 1
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_SM, ASTEROID_COLOR_RED },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        // wave 2
        { .time = 23.0, .count = 2, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        { .time = 23.0, .count = 0, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        // wave 3
        { .time = 23.0, .count = 0, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { .time = 23.0, .count = 0, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
    },
    // level 2
    {
        // wave 1
        { .time = 3.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_GREEN },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        // wave 2
        { .time = 13.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_GREEN },
        { .time = 13.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
        // wave 3
        { .time = 23.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_GREEN },
        { .time = 23.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_GREEN },
    },
    // level 3
    {
        // wave 1
        { .time = 3.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        { .time = 3.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 2
        { .time = 13.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        { .time = 13.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 3
        { .time = 23.0, .count = 1, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        { .time = 23.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
    },
};

Sound * level_announcements[] = {
    &level_1_snd,
    &level_2_snd,
    &level_3_snd,
    &level_4_snd,
    &level_5_snd,
};

void level_update(vector * entities, GameLevelState * gls){
    
    // watch for level change
    if (gls->wave == 6) {
        bool no_asteroids = true;
        size_t elen = vector_size(entities);
        Entity * es = vector_begin(entities);
        for(size_t i = 0; i < elen; i++){
            if (es[i].type == ENTITY_TYPE_ASTEROID)
                no_asteroids = false;
        }

        // if level > levels
        // either finish
        // or engage infinite mode

        if (no_asteroids && gls->level != 2) {
            gls->level++;
            PlaySound(*level_announcements[gls->level]);
            gls->wave = 0;
            gls->level_start_time = game_get_time();
        }
        return;
    }

    while (gls->level_start_time + levels[gls->level][gls->wave].time < game_get_time()){
        if (gls->wave == 0)
            PlaySound(level_engage_snd);

        level_spawnset ls = levels[gls->level][gls->wave];
        for(size_t i = 0; i < ls.count; i++){
            Entity e = entity_asteroid_spawn(ls.size, ls.color, (Vector3){-3.5,0,0});
            vector_push(entities, &e);
        }
        gls->wave++;
        if (gls->wave == 6)
            return;
    }
}

char time__label[100];
char level_label[100];
char wave__label[100];

void level_draw_2d(GameLevelState gls){
    sprintf(time__label, "time : %.2f", game_get_time() - gls.level_start_time);
    sprintf(level_label, "level: %lu", gls.level);
    sprintf(wave__label, "wave : %lu", gls.wave);
    DrawText(time__label, 32,  32, 32, RAYWHITE);
    DrawText(level_label, 32, 64, 32, RAYWHITE);
    DrawText(wave__label, 32, 96, 32, RAYWHITE);
}
