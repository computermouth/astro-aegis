
#ifndef AA_LEVEL_H
#define AA_LEVEL_H

#include "vector.h"
#include "game.h"

void level_update(vector * v, GameLevelState * gls);
void level_draw_2d(GameLevelState gls);

#endif