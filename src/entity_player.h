
#ifndef AA_ENTITY_PLAYER_H
#define AA_ENTITY_PLAYER_H

#include "entity.h"

extern const float ENTITY_PLAYER_FRAME_SPEED;
extern const float ENTITY_PLAYER_FRICTION;
extern const float ENTITY_PLAYER_MAX_SPEED;

Entity entity_player_spawn();
void entity_player_update(Entity * player);
void entity_player_add_score(int i);
void entity_player_inc_streak();

#endif
