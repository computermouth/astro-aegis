
#ifndef AA_ENTITY_H
#define AA_ENTITY_H

#include "raylib.h"
#include "vector.h"

typedef enum {
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_GLOBE,
    ENTITY_TYPE_ASTEROID,
} EntityType;

typedef struct {

} GlobeEntity;

typedef struct {
    float dir_x;
    float dir_z;
} PlayerEntity;

typedef struct {
    EntityType type;
    union {
        GlobeEntity globe_entity;
        PlayerEntity player_entity;
    };
    Mesh mesh;
    Material material;
    Matrix transform;
} Entity;

#endif