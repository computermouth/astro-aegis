
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "entity_bullet.h"
#include "game.h"
#include "vector.h"
#include "weapon.h"
#include "resource.h"

typedef struct {
    float cooldown;
    int bullet_count;
    float speed;
} WeaponPower;

#define MAX_WEAPON_LV 5

WeaponPower wp[__WEAPON_END][MAX_WEAPON_LV] = {
    [WEAPON_RED] = { // 5.1x
        [0] = {.cooldown = 0.50, .bullet_count = 1, .speed = 1.0},
        [1] = {.cooldown = 0.35, .bullet_count = 1, .speed = 1.1},
        [2] = {.cooldown = 0.20, .bullet_count = 1, .speed = 1.2},
        [3] = {.cooldown = 0.15, .bullet_count = 1, .speed = 1.3},
        [4] = {.cooldown = 0.10, .bullet_count = 1, .speed = 1.4},
    },
    [WEAPON_GRN] = { // 7.2x
        [0] = {.cooldown = 0.5, .bullet_count = 1, .speed = 1.0},
        [1] = {.cooldown = 0.5, .bullet_count = 2, .speed = 1.1},
        [2] = {.cooldown = 0.4, .bullet_count = 2, .speed = 1.2},
        [3] = {.cooldown = 0.4, .bullet_count = 2, .speed = 1.3},
        [4] = {.cooldown = 0.3, .bullet_count = 3, .speed = 1.4},
    },
    [WEAPON_BLU] = { // 9x
        [0] = {.cooldown = 0.5, .bullet_count = 1, .speed = 1.0},
        [1] = {.cooldown = 0.5, .bullet_count = 3, .speed = 1.1},
        [2] = {.cooldown = 0.5, .bullet_count = 5, .speed = 1.2},
        [3] = {.cooldown = 0.5, .bullet_count = 7, .speed = 1.3},
        [4] = {.cooldown = 0.5, .bullet_count = 9, .speed = 1.4},
    },
};

Sound * weapon_shoot_noises[] = {
    &bullet_shoot_r_snd,
    &bullet_shoot_g_snd,
    &bullet_shoot_b_snd,
};

void weapon_fire(WeaponType wt, Weapon * w, Vector2 dir){

    int p = w->power;
    if (p > 4) p = 4;

    float gt = game_get_time();
    if (w->next_shot <= gt){
        // shoot
        w->next_shot = gt + wp[wt][p].cooldown;

        PlaySound(*weapon_shoot_noises[wt]);

        vector * entities = game_get_other_entities();

        float max_angle = 45;
        if (wt == WEAPON_BLU)
            max_angle = 90;
        
        float spread_increment = (max_angle  * PI / 180) / wp[wt][p].bullet_count;

        switch (wp[wt][p].bullet_count){
            case 1:
                {
                    Entity e = entity_bullet_spawn(wt, dir, wp[wt][p].speed);
                    vector_push(entities, &e);
                }
                break;
            case 2:
                {
                    Entity e1 = entity_bullet_spawn(wt, Vector2Rotate(dir, -spread_increment), wp[wt][p].speed);
                    Entity e2 = entity_bullet_spawn(wt, Vector2Rotate(dir,  spread_increment), wp[wt][p].speed);
                    vector_push(entities, &e1);
                    vector_push(entities, &e2);
                }
                break;
            default:
                { // center
                    Entity e = entity_bullet_spawn(wt, dir, wp[wt][p].speed);
                    vector_push(entities, &e);
                }
                for(int i = 0; i < (wp[wt][p].bullet_count - 1) / 2; i++){
                    Entity e1 = entity_bullet_spawn(wt, Vector2Rotate(dir, -spread_increment * (i + 1)), wp[wt][p].speed);
                    Entity e2 = entity_bullet_spawn(wt, Vector2Rotate(dir,  spread_increment * (i + 1)), wp[wt][p].speed);
                    vector_push(entities, &e1);
                    vector_push(entities, &e2);
                }
                break;
        }
    }
}
        // entity_asteroid_kill(asteroid);
        // entity_player_add_score(10 * damage);
        // if (damage == 1.0){
        //     Weapon * w = game_get_weapon(wt);
        //     // each strong asteroid kill is 1/10 a level
        //     float old_power = w->power;
        //     w->power += (damage == 1.0) * .5;

        //     if ((int)old_power != (int)w->power){
        //         PlaySound(powerup_snd);
        //         PlaySound(weapon_pu_snd);
        //     }

        //     Vector3 a_pos = entity_asteroid_get_pos(asteroid);
        //     Vector2 screen_a_pos = GetWorldToScreen(a_pos, game_get_camera());
        //     Entity e = entity_banner_spawn((BannerType)wt, screen_a_pos);
        //     vector * oe = game_get_other_entities();
        //     vector_push(oe, &e);
        // }

void weapon_powerup(WeaponType wt, bool critical){
    Weapon * w = game_get_weapon(wt);

    // each critical asteroid kill is 1/10 a level
    // each weak kill is 1/40 a level
    float old_power = w->power;
    // .00001 is to account for some FP inaccuracy
    float inc = .025001 + (critical * 3 * .025001) ;
    // w->power = fmodf(w->power + inc, MAX_WEAPON_LV);
    w->power += inc;

    if (w->power < 5 && (int)old_power != (int)w->power){
        PlaySound(powerup_snd);
        PlaySound(weapon_pu_snd);
    }
}
