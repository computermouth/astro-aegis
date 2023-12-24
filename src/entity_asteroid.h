
#ifndef AA_ENTITY_ASTEROID_H
#define AA_ENTITY_ASTEROID_H

#include "entity.h"

Entity entity_asteroid_spawn(AsteroidSize size, AsteroidColor color, Vector3 origin);
void entity_asteroid_take_damage(Entity * asteroid, WeaponType w);
void entity_asteroid_update(Entity * asteroid);

#endif