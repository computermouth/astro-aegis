
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
} GlobeStorage;

typedef struct {
    float dir_x;
    float dir_z;
    float menu_input_x;
    float menu_input_z;
    Quaternion frame_rotation;
} PlayerStorage;

typedef struct entity{
    EntityType type;
    union {
        GlobeStorage globe_storage;
        PlayerStorage player_storage;
    };
    Mesh mesh;
    Material material;
    Quaternion rotation;
    Matrix transform;
    void (*draw_2d_fn)(struct entity *);
    void (*draw_3d_fn)(struct entity *);
} Entity;

#endif