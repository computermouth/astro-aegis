
#include <stdio.h>

#include "entity_bullet.h"
#include "game.h"
#include "vector.h"
#include "weapon.h"

typedef struct {
    float cooldown;
    int bullet_count;
    float speed;
} WeaponPower;

WeaponPower wp[__WEAPON_END][5] = {
    [WEAPON_RED] = {
        [0] = {.cooldown = 1.0, .bullet_count = 1, .speed = 1.0},
        [1] = {.cooldown = 0.8, .bullet_count = 1, .speed = 1.1},
        [2] = {.cooldown = 0.6, .bullet_count = 1, .speed = 1.2},
        [3] = {.cooldown = 0.4, .bullet_count = 1, .speed = 1.3},
        [4] = {.cooldown = 0.2, .bullet_count = 1, .speed = 1.4},
    },
    [WEAPON_GRN] = {
        [0] = {.cooldown = 1.0, .bullet_count = 1, .speed = 1.0},
        [1] = {.cooldown = 0.9, .bullet_count = 2, .speed = 1.0},
        [2] = {.cooldown = 0.8, .bullet_count = 3, .speed = 1.0},
        [3] = {.cooldown = 0.7, .bullet_count = 4, .speed = 1.0},
        [4] = {.cooldown = 0.6, .bullet_count = 5, .speed = 1.0},
    },
    [WEAPON_BLU] = {
        [0] = {.cooldown = 1.0, .bullet_count = 1, .speed = 1.00},
        [1] = {.cooldown = 1.0, .bullet_count = 3, .speed = 1.05},
        [2] = {.cooldown = 1.0, .bullet_count = 5, .speed = 1.10},
        [3] = {.cooldown = 1.0, .bullet_count = 7, .speed = 1.15},
        [4] = {.cooldown = 1.0, .bullet_count = 9, .speed = 1.20},
    },
};

void weapon_fire(WeaponType wt, Weapon * w, Vector2 dir, vector * entities){

    int p = w->power;
    if (p > 4) p = 4;

    float gt = game_get_time();
    if (w->next_shot <= gt){
        // shoot
        // fprintf(stderr, "pew(%d)@%f\n", wt, w->next_shot);
        w->next_shot = gt + wp[wt][p].cooldown;

        for(int i = 0; i < wp[wt][p].bullet_count; i++){
            // bullet count -> angle -> Vector2Rotate(dir, angle)
            Entity e = entity_bullet_spawn(wt, dir, wp[wt][p].speed);
            vector_push(entities, &e);
        }
    }
    
}

void weapon_powerup(Weapon * w);