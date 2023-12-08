
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

typedef enum {
    ASTEROID_COLOR_BLUE,
    ASTEROID_COLOR_RED,
    ASTEROID_COLOR_GREEN,
} AsteroidColor;

typedef enum {
    ASTEROID_LG,
    ASTEROID_MD,
    ASTEROID_SM,
} AsteroidSize;

typedef struct {
    AsteroidColor color;
    AsteroidSize size;
    float dir_x;
    float dir_z;
} AsteroidStorage;

typedef struct entity{
    EntityType type;
    union {
        GlobeStorage globe_storage;
        PlayerStorage player_storage;
        AsteroidStorage asteroid_storage;
    };
    Mesh mesh;
    Material material;
    Matrix transform;
    void (*draw_2d_fn)(struct entity *);
    void (*draw_3d_fn)(struct entity *);
} Entity;

#endif