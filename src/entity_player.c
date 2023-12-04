
#include "entity_player.h"


Entity entity_player_spawn(){
    return (Entity) {
        .mesh = { 0 },
        .material = { 0 },
        .translation_matrix = { 0 },
        .transform_matrix = { 0 },
        .position = { 0 },
        .speed = { 0 },
    };
}