
#include <stdio.h>

#include "level.h"
#include "entity.h"
#include "entity_asteroid.h"
#include "game.h"
#include "vector.h"

typedef struct {
    float time;
    size_t count;
    AsteroidSize size;
    AsteroidColor color;
} level_spawnset;

level_spawnset levels[2][6] = {
    // level 1
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { .time = 3.0, .count = 3, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        // wave 2
        { .time = 13.0, .count = 4, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { .time = 13.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
        // wave 3
        { .time = 23.0, .count = 4, ASTEROID_SIZE_LG, ASTEROID_COLOR_RED },
        { .time = 23.0, .count = 3, ASTEROID_SIZE_MD, ASTEROID_COLOR_RED },
    },
    // level 2
    {
        // wave 1
        { .time = 3.0, .count = 2, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        { .time = 3.0, .count = 3, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 2
        { .time = 13.0, .count = 4, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        { .time = 13.0, .count = 1, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
        // wave 3
        { .time = 23.0, .count = 4, ASTEROID_SIZE_LG, ASTEROID_COLOR_BLUE },
        { .time = 23.0, .count = 3, ASTEROID_SIZE_MD, ASTEROID_COLOR_BLUE },
    },
};

void level_update(vector * v, GameLevelState * gls){

    // watch for level change
    if (gls->wave == 6) {
        if (vector_size(v) == 0) {
            gls->level++;
            gls->wave = 0;
            gls->level_start_time = game_get_time();
        }
        return;
    }

    while (gls->level_start_time + levels[gls->level][gls->wave].time < game_get_time()){
        level_spawnset ls = levels[gls->level][gls->wave];
        for(size_t i = 0; i < ls.count; i++){
            fprintf(stderr, "spawn: %d %d\n", ls.size, ls.color);
            Entity e = entity_asteroid_spawn(ls.size, ls.color);
            vector_push(v, &e);
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
    DrawText(time__label, 32,  96, 32, RAYWHITE);
    DrawText(level_label, 32, 128, 32, RAYWHITE);
    DrawText(wave__label, 32, 160, 32, RAYWHITE);
}
