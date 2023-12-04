
#ifndef AA_GAME_H
#define AA_GAME_H

#include <stdbool.h>

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAY,
    GAME_STATE_OVER,
} GameState;

extern int GAME_SCREEN_WIDTH;
extern int GAME_SCREEN_HEIGHT;

void game_update();
void game_set_state(GameState);
bool game_get_should_quit();
GameState game_get_state();

#endif