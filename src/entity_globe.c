
#include "entity_globe.h"
#include "entity.h"


Entity entity_globe_spawn(){
    return (Entity) {
        .type = ENTITY_TYPE_GLOBE,
        .mesh = { 0 },
        .material = { 0 },
        .transform = { 0 }
    };
}

void entity_globe_update(Entity * globe, const Entity player){



}