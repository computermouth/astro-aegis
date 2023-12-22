
#ifndef AA_ENTITY_BANNER_H
#define AA_ENTITY_BANNER_H

#include "entity.h"

Entity entity_banner_spawn(BannerType bt, Vector2 screen_pos);
void entity_banner_update(Entity * banner);

#endif