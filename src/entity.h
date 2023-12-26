
#ifndef AA_ENTITY_H
#define AA_ENTITY_H

#include "raylib.h"
#include "vector.h"
#include "weapon.h"

typedef enum {
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_GLOBE,
    ENTITY_TYPE_ASTEROID,
    ENTITY_TYPE_BULLET,
    ENTITY_TYPE_BANNER,
} EntityType;

// ordered RGB to match weapon
typedef enum {
    ASTEROID_COLOR_RED,
    ASTEROID_COLOR_GREEN,
    ASTEROID_COLOR_BLUE,
    __ASTEROID_COLOR_END,
} AsteroidColor;

typedef enum {
    ASTEROID_SIZE_LG,
    ASTEROID_SIZE_MD,
    ASTEROID_SIZE_SM,
    __ASTEROID_SIZE_END
} AsteroidSize;

typedef enum {
    BANNER_XPUP_RED,
    BANNER_XPUP_GRN,
    BANNER_XPUP_BLU,
    __BANNER_SIZE_END,
} BannerType;

typedef struct {
    Quaternion net_rotation;
} GlobeStorage;

typedef struct {
    float      dir_x;
    float      dir_z;
    float      menu_input_x;
    float      menu_input_z;
    float      score;
    int        multi;
    int        streak;
    int        health;
    float      invincible_until;
    Quaternion frame_rotation;
    Vector2    shoot_dir;
    WeaponType weapon_index;
    Weapon     weapons[__WEAPON_END];
} PlayerStorage;

typedef struct {
    AsteroidColor color;
    AsteroidSize  size;
    float         traversal_speed;
    Vector3       traversal_direction;
    float         rotation_speed;
    Vector3       rotation_direction;
    Vector3       origin_position;
    Quaternion    origin_rotation;
    float         origin_time;
    float         health;
} AsteroidStorage;

typedef struct {
    Vector2    dir;
    float      speed;
    float      distance_traveled;
    WeaponType w;
    Quaternion origin_rotation;
    float      origin_time;
} BulletStorage;

typedef struct {
    BannerType type;
    Vector2    pos;
    float      origin_time;
} BannerStorage;

typedef struct entity{
    EntityType type;
    union {
        GlobeStorage    globe_storage;
        PlayerStorage   player_storage;
        AsteroidStorage asteroid_storage;
        BulletStorage   bullet_storage;
        BannerStorage   banner_storage;
    };
    Mesh     mesh;
    Material material;
    Matrix   transform;
    void     (*draw_2d_fn)(struct entity *);
    void     (*draw_3d_fn)(struct entity *);
    bool     dead;
} Entity;

#endif
