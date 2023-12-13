#include <stdio.h>

#include "entity.h"
#include "raylib.h"
#include "raymath.h"

#include "entity_asteroid.h"
#include "tool.h"
#include "resource.h"
#include "game.h"

void entity_asteroid_draw_2d(Entity * asteroid);
void entity_asteroid_draw_3d(Entity * asteroid);

Entity entity_asteroid_spawn(AsteroidSize size, AsteroidColor color){

    int mod_id = -1;
    int mat_id = -1;

    switch (size){
        case ASTEROID_SIZE_LG:
            mod_id = GetRandomValue(0, 9);
            break;
        case ASTEROID_SIZE_MD:
            mod_id = GetRandomValue(10, 19);
            break;
        case ASTEROID_SIZE_SM:
            mod_id = GetRandomValue(20, 29);
            break;
        default:
            fprintf(stderr, "E: bad asteroid size -- %d\n", size);
            return (Entity){0};
    }

    switch (color){
        case ASTEROID_COLOR_BLUE:
            mat_id = GetRandomValue(0, 2);
            break;
        case ASTEROID_COLOR_GREEN:
            mat_id = GetRandomValue(3, 5);
            break;
        case ASTEROID_COLOR_RED:
            mat_id = GetRandomValue(6, 8);
            break;
        default:
            fprintf(stderr, "E: bad asteroid color -- %d\n", color);
            return (Entity){0};
    }

    Entity a = {
        .type = ENTITY_TYPE_ASTEROID,
        .mesh = asteroid_models[mod_id].meshes[0],
        .material = asteroid_materials[mat_id],
        .transform = MatrixIdentity(),
        .draw_2d_fn = entity_asteroid_draw_2d,
        .draw_3d_fn = entity_asteroid_draw_3d,
    };

    float ts = (float)(GetRandomValue(2, 7)) / 10.0f;
    Vector3 td = Vector3SubtractValue(Vector3Multiply(Vector3Normalize((Vector3){
        .x = (float)(GetRandomValue(0, 100)) / 100.0f,
        .y = (float)(GetRandomValue(0, 100)) / 100.0f,
        .z = (float)(GetRandomValue(0, 100)) / 100.0f,
    }), (Vector3){2, 2, 2}), 1);

    float rs = (float)(GetRandomValue(10, 30)) / 10.0f;
    Vector3 rd = Vector3SubtractValue(Vector3Multiply(Vector3Normalize((Vector3){
        .x = (float)(GetRandomValue(0, 100)) / 100.0f,
        .y = (float)(GetRandomValue(0, 100)) / 100.0f,
        .z = (float)(GetRandomValue(0, 100)) / 100.0f,
    }), (Vector3){2, 2, 2}), 1);

    AsteroidStorage s = {
        .color = color,
        .size = size,
        .traversal_speed = ts,
        .traversal_direction = td,
        .rotation_speed = rs,
        .rotation_direction = rd,
        .origin_rotation = game_get_globe_entity()->globe_storage.net_rotation,
        .origin_time = game_get_time(),
    };
    
    Matrix scale = MatrixScale(0.4f, 0.4f, 0.4f);
    Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(s.rotation_direction, fmodf(s.rotation_speed * game_get_time(), PI * 2)));
    Vector3 offset_in = Vector3Transform((Vector3){-3.5, 0, 0}, QuaternionToMatrix(QuaternionInvert(s.origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
    Vector3 traversal_axis = Vector3Normalize(Vector3CrossProduct(s.traversal_direction, Vector3Normalize(offset_in)));
    Matrix traversal_rot = MatrixRotate(traversal_axis, fmodf(s.traversal_speed * (game_get_time() - s.origin_time), PI * 2) );
    Matrix globe_rot = QuaternionToMatrix(q_globe);
    Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix trans = MatrixTranslate(center.x, center.y, center.z);

    Matrix asteroid_transform = MatrixIdentity();
    asteroid_transform = MatrixMultiply(asteroid_transform, scale);
    asteroid_transform = MatrixMultiply(asteroid_transform, asteroid_rotation);
    asteroid_transform = MatrixMultiply(asteroid_transform, offset);
    asteroid_transform = MatrixMultiply(asteroid_transform, traversal_rot);
    asteroid_transform = MatrixMultiply(asteroid_transform, globe_rot);
    asteroid_transform = MatrixMultiply(asteroid_transform, trans);

    a.transform = asteroid_transform;

    a.asteroid_storage = s;

    return a;
}

void entity_asteroid_update(Entity * asteroid){

    AsteroidStorage s = asteroid->asteroid_storage;
    // todo, make -3.5f
    Matrix scale = MatrixScale(0.4f, 0.4f, 0.4f);
    Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(s.rotation_direction, fmodf(s.rotation_speed * game_get_time(), PI * 2)));
    Vector3 offset_in = Vector3Transform((Vector3){-3.5, 0, 0}, QuaternionToMatrix(QuaternionInvert(s.origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
    Vector3 traversal_axis = Vector3Normalize(Vector3CrossProduct(s.traversal_direction, Vector3Normalize(offset_in)));
    Matrix traversal_rot = MatrixRotate(traversal_axis, fmodf(s.traversal_speed * (game_get_time() - s.origin_time), PI * 2) );
    Matrix globe_rot = QuaternionToMatrix(q_globe);
    Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix trans = MatrixTranslate(center.x, center.y, center.z);

    Matrix asteroid_transform = MatrixIdentity();
    asteroid_transform = MatrixMultiply(asteroid_transform, scale);
    asteroid_transform = MatrixMultiply(asteroid_transform, asteroid_rotation);
    asteroid_transform = MatrixMultiply(asteroid_transform, offset);
    asteroid_transform = MatrixMultiply(asteroid_transform, traversal_rot);
    asteroid_transform = MatrixMultiply(asteroid_transform, globe_rot);
    asteroid_transform = MatrixMultiply(asteroid_transform, trans);

    asteroid->transform = asteroid_transform;
}

void entity_asteroid_draw_2d(Entity * asteroid){
    (void)asteroid;
}

void entity_asteroid_draw_3d(Entity * asteroid){
    DrawMesh(asteroid->mesh, asteroid->material, asteroid->transform);
}