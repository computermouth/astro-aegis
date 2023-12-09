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

// Entity entity_asteroid_spawn(){
    
//     // todo, make -3.5f
//     Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
//     Matrix scale = MatrixScale(0.25f, 0.25f, 0.25f);
//     Matrix offset = MatrixTranslate(3.5f, 0, 0);
//     Matrix trans = MatrixTranslate(center.x, center.y, center.z);
//     Matrix mrot = QuaternionToMatrix(QuaternionIdentity());
//     Matrix asteroid_transform = MatrixIdentity();
//     asteroid_transform = MatrixMultiply(asteroid_transform, scale);
//     asteroid_transform = MatrixMultiply(asteroid_transform, offset);
//     asteroid_transform = MatrixMultiply(asteroid_transform, mrot);
//     asteroid_transform = MatrixMultiply(asteroid_transform, trans);
    
//     // asteroid_transform = MatrixMultiply(asteroid_transform, trans);

//     Entity a = {
//         .type = ENTITY_TYPE_ASTEROID,
//         .mesh = asteroid_b000_model.meshes[0], // todo
//         .material = bluetex1_mat, // todo
//         .transform = asteroid_transform,
//         .draw_2d_fn = entity_asteroid_draw_2d,
//         .draw_3d_fn = entity_asteroid_draw_3d,
//     };

//     AsteroidStorage s = {
//         .color = ASTEROID_COLOR_BLUE,
//         .size = ASTEROID_LG,
//         .speed = (Vector2){-.7,0},
//         .rotation_direction = (Vector3){1,1,1},
//         .rotation_speed = 2.5,
//         .origin_rotation = QuaternionIdentity(),
//     };

//     a.asteroid_storage = s;

//     return a;
// }

Entity entity_asteroid_spawn(){
    
    // todo, make -3.5f
    Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix scale = MatrixScale(0.25f, 0.25f, 0.25f);
    Matrix offset = MatrixTranslate(3.5f, 0, 0);
    Matrix trans = MatrixTranslate(center.x, center.y, center.z);
    Matrix mrot = QuaternionToMatrix(QuaternionIdentity());
    Matrix asteroid_transform = MatrixIdentity();
    asteroid_transform = MatrixMultiply(asteroid_transform, scale);
    asteroid_transform = MatrixMultiply(asteroid_transform, offset);
    asteroid_transform = MatrixMultiply(asteroid_transform, mrot);
    asteroid_transform = MatrixMultiply(asteroid_transform, trans);
    
    // asteroid_transform = MatrixMultiply(asteroid_transform, trans);

    Entity a = {
        .type = ENTITY_TYPE_ASTEROID,
        .mesh = asteroid_b000_model.meshes[0], // todo
        .material = bluetex1_mat, // todo
        .transform = asteroid_transform,
        .draw_2d_fn = entity_asteroid_draw_2d,
        .draw_3d_fn = entity_asteroid_draw_3d,
    };

    AsteroidStorage s = {
        .color = ASTEROID_COLOR_BLUE,
        .size = ASTEROID_LG,
        .speed = (Vector2){-.7,0},
        .rotation_direction = (Vector3){1,1,1},
        .rotation_speed = 2.5,
        .origin_rotation = game_get_globe_entity()->globe_storage.net_rotation,
        .origin_time = game_get_time(),
    };

    a.asteroid_storage = s;

    return a;
}

Quaternion QuaternionConjugate(Quaternion q) {
    Quaternion result;
    result.w = q.w;
    result.x = -q.x;
    result.y = -q.y;
    result.z = -q.z;
    return result;
}

void entity_asteroid_update(Entity * asteroid){

    Matrix scale = MatrixScale(0.25f, 0.25f, 0.25f);
    // Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(asteroid->asteroid_storage.rotation_direction, fmodf(asteroid->asteroid_storage.rotation_speed * game_get_time(), PI * 2)));
    Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(asteroid->asteroid_storage.rotation_direction, fmodf(asteroid->asteroid_storage.rotation_speed * game_get_time(), PI * 2)));
    float spawn_len = Vector3Length((Vector3){3.5,0,0});
    Vector3 offset_in = Vector3Multiply(Vector3Normalize((Vector3){1, 1, 1}), (Vector3){spawn_len, spawn_len, spawn_len});
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
    // float angle = Vector2Angle((Vector2){0,0}, asteroid->asteroid_storage.speed);
    // fprintf(stderr, "angle: %f\n", angle);
    Vector3 rotation_axis = Vector3Normalize(Vector3CrossProduct((Vector3){1, 0, 0}, Vector3Normalize(offset_in)));
    Matrix traversal_rot = MatrixRotate(rotation_axis, fmodf(game_get_time() - asteroid->asteroid_storage.origin_time, PI * 2) );
    // Matrix traversal_rot = MatrixRotateXYZ((Vector3){.x = offset_in.x * game_get_time(), .y = offset_in.y * game_get_time(), .z = offset_in.z * game_get_time()});
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

// void entity_asteroid_update(Entity * asteroid){

//     Matrix scale = MatrixScale(0.25f, 0.25f, 0.25f);
//     // Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(asteroid->asteroid_storage.rotation_direction, fmodf(asteroid->asteroid_storage.rotation_speed * game_get_time(), PI * 2)));
//     Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(asteroid->asteroid_storage.rotation_direction, fmodf(asteroid->asteroid_storage.rotation_speed * game_get_time(), PI * 2)));
//     float spawn_len = Vector3Length((Vector3){3.5,0,0});
//     Vector3 offset_in = Vector3Multiply(Vector3Normalize((Vector3){-.97, .59, .33}), (Vector3){spawn_len, spawn_len, spawn_len});
//     Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
//     Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
//     // float angle = Vector2Angle((Vector2){0,0}, asteroid->asteroid_storage.speed);
//     // fprintf(stderr, "angle: %f\n", angle);
//     Vector3 rotation_axis = Vector3Normalize(Vector3CrossProduct((Vector3){1, 0, 0}, Vector3Normalize(offset_in)));
//     Matrix traversal_rot = MatrixRotate(rotation_axis, fmodf(game_get_time(), PI * 2) );
//     // Matrix traversal_rot = MatrixRotateXYZ((Vector3){.x = offset_in.x * game_get_time(), .y = offset_in.y * game_get_time(), .z = offset_in.z * game_get_time()});
//     Matrix globe_rot = QuaternionToMatrix(q_globe);
//     Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
//     Matrix trans = MatrixTranslate(center.x, center.y, center.z);

//     Matrix asteroid_transform = MatrixIdentity();
//     asteroid_transform = MatrixMultiply(asteroid_transform, scale);
//     asteroid_transform = MatrixMultiply(asteroid_transform, asteroid_rotation);
//     asteroid_transform = MatrixMultiply(asteroid_transform, offset);
//     asteroid_transform = MatrixMultiply(asteroid_transform, traversal_rot);
//     asteroid_transform = MatrixMultiply(asteroid_transform, globe_rot);
//     asteroid_transform = MatrixMultiply(asteroid_transform, trans);

//     asteroid->transform = asteroid_transform;
// }


void entity_asteroid_draw_2d(Entity * asteroid){
    (void)asteroid;
}

void entity_asteroid_draw_3d(Entity * asteroid){
    DrawMesh(asteroid->mesh, asteroid->material, asteroid->transform);
}