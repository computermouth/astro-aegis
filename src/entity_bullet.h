
#ifndef AA_ENTITY_BULLET_H
#define AA_ENTITY_BULLET_H

#include "raylib.h"
#include "entity.h"
#include "weapon.h"

Entity entity_bullet_spawn(WeaponType w, Vector2 shoot_dir, float speed);
void entity_bullet_update(Entity * bullet);

#endif