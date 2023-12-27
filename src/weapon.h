
#ifndef AA_WEAPON_H
#define AA_WEAPON_H

#include "raylib.h"
#include "vector.h"

typedef enum {
    WEAPON_RED,
    WEAPON_GRN,
    WEAPON_BLU,
    __WEAPON_END,
} WeaponType;

typedef struct {
    float next_shot;
    float power;
} Weapon;

void weapon_fire(WeaponType wt, Weapon * w, Vector2 dir);
void weapon_powerup(WeaponType wt, bool critical);

#endif
