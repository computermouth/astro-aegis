
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
    Quaternion net_rotation;
} GlobeStorage;

typedef struct {
    float dir_x;
    float dir_z;
    float menu_input_x;
    float menu_input_z;
    Quaternion frame_rotation;
    Vector2 mouse_aim_dir;
} PlayerStorage;

typedef enum {
    ASTEROID_COLOR_BLUE,
    ASTEROID_COLOR_GREEN,
    ASTEROID_COLOR_RED,
    __ASTEROID_COLOR_END,
} AsteroidColor;

typedef enum {
    ASTEROID_SIZE_LG,
    ASTEROID_SIZE_MD,
    ASTEROID_SIZE_SM,
    __ASTEROID_SIZE_END
} AsteroidSize;

typedef struct {
    AsteroidColor color;
    AsteroidSize  size;
    float         traversal_speed;
    Vector3       traversal_direction;
    float         rotation_speed;
    Vector3       rotation_direction;
    Quaternion    origin_rotation;
    float         origin_time;
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
