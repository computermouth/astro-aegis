#include <stdio.h>

#include "entity.h"
#include "entity_player.h"
#include "raylib.h"
#include "raymath.h"

#include "entity_asteroid.h"
#include "tool.h"
#include "resource.h"
#include "game.h"
#include "vector.h"
#include "entity_banner.h"

void entity_asteroid_draw_2d(Entity * asteroid);
void entity_asteroid_draw_3d(Entity * asteroid);
void entity_asteroid_kill(Entity * asteroid);

Entity entity_asteroid_spawn(AsteroidSize size, AsteroidColor color, Vector3 origin){

    int mod_id = -1;
    int mat_id = -1;
    float health = 7;

    switch (size){
        case ASTEROID_SIZE_LG:
            mod_id = GetRandomValue(0, 9);
            break;
        case ASTEROID_SIZE_MD:
            mod_id = GetRandomValue(10, 19);
            health = 5;
            break;
        case ASTEROID_SIZE_SM:
            mod_id = GetRandomValue(20, 29);
            health = 3;
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
        .dead = false,
    };

    float ts = (float)(GetRandomValue(2, 6)) / 10.0f;
    Vector3 td = Vector3SubtractValue(Vector3Normalize((Vector3){
        .x = (float)(GetRandomValue(0, 100)) / 100.0f,
        .y = (float)(GetRandomValue(0, 100)) / 100.0f,
        .z = (float)(GetRandomValue(0, 100)) / 100.0f,
    }), .5);

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
        .origin_position = origin,
        .origin_rotation = game_get_globe_entity()->globe_storage.net_rotation,
        .origin_time = game_get_time(),
        .health = health,
    };
    
    Matrix scale = MatrixScale(0.4f, 0.4f, 0.4f);
    Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(s.rotation_direction, fmodf(s.rotation_speed * game_get_time(), PI * 2)));
    Vector3 offset_in = Vector3Transform(s.origin_position, QuaternionToMatrix(QuaternionInvert(s.origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
    Vector3 traversal_axis = Vector3Normalize(Vector3CrossProduct(s.traversal_direction, Vector3Normalize(offset_in)));
    s.traversal_direction = traversal_axis;
    Matrix traversal_rot = MatrixRotate(traversal_axis, fmodf(s.traversal_speed * (game_get_time() + game_get_delta() - s.origin_time), PI * 2) );
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

Matrix entity_asteroid_next_transform(Entity * asteroid){

    AsteroidStorage s = asteroid->asteroid_storage;

    Matrix scale = MatrixScale(0.4f, 0.4f, 0.4f);
    Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(s.rotation_direction, fmodf(s.rotation_speed * game_get_time(), PI * 2)));
    Vector3 offset_in = Vector3Transform(s.origin_position, QuaternionToMatrix(QuaternionInvert(s.origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
    Vector3 traversal_axis = s.traversal_direction; // Vector3Normalize(Vector3CrossProduct(s.traversal_direction, Vector3Normalize(offset_in)));
    Matrix traversal_rot = MatrixRotate(traversal_axis, fmodf(s.traversal_speed * (game_get_time() + game_get_delta() - s.origin_time), PI * 2) );
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

    return asteroid_transform;
}

void entity_asteroid_update(Entity * asteroid){

    AsteroidStorage s = asteroid->asteroid_storage;

    Matrix scale = MatrixScale(0.4f, 0.4f, 0.4f);
    Matrix asteroid_rotation = QuaternionToMatrix(QuaternionFromAxisAngle(s.rotation_direction, fmodf(s.rotation_speed * game_get_time(), PI * 2)));
    Vector3 offset_in = Vector3Transform(s.origin_position, QuaternionToMatrix(QuaternionInvert(s.origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;
    Vector3 traversal_axis = s.traversal_direction; // Vector3Normalize(Vector3CrossProduct(s.traversal_direction, Vector3Normalize(offset_in)));
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

    // Vector3 t = Vector3Transform((Vector3){0,0,0}, asteroid_transform);
    // fprintf(stderr, "sp: {%f,%f,%f}\n", t.x, t.y, t.z);

    asteroid->transform = asteroid_transform;
}

Vector3 entity_asteroid_get_pos(Entity * asteroid){
    return Vector3Transform((Vector3){0,0,0}, asteroid->transform);
}

void entity_asteroid_take_damage(Entity * asteroid, WeaponType wt){
    float damage = 1.0;
    if (asteroid->asteroid_storage.color != (AsteroidColor)wt)
        damage = 0.5;

    asteroid->asteroid_storage.health -= damage;
    if (asteroid->asteroid_storage.health <= 0){
        bool critical = damage == 1.0;
        entity_asteroid_kill(asteroid);
        entity_player_add_score(10 * damage);

        weapon_powerup(wt, critical);

        // spawn banner on crit
        if (critical && game_get_player_entity()->player_storage.weapons[wt].power < 5){
            Vector3 a_pos = entity_asteroid_get_pos(asteroid);
            Vector2 screen_a_pos = GetWorldToScreen(a_pos, game_get_camera());
            Entity e = entity_banner_spawn((BannerType)wt, screen_a_pos);
            vector * oe = game_get_other_entities();
            vector_push(oe, &e);
        }
    }
}

Sound * asteroid_explosion_snds[] = {
    &asteroid_explosion1_snd,
    &asteroid_explosion2_snd,
    &asteroid_explosion3_snd,
    &asteroid_explosion4_snd,
    &asteroid_explosion5_snd,
};

void entity_asteroid_kill(Entity * asteroid){
    // mark for cleanup
    asteroid->dead = true;

    int kill_snd = GetRandomValue(0, 4);
    PlaySound(*asteroid_explosion_snds[kill_snd]);

    int count1 = GetRandomValue(2, 3);
    int count2 = 3 - count1;

    vector * eo = game_get_other_entities();
    
    Vector3 origin = (Vector3){0,0,0};
    Vector3 trans_origin = Vector3Transform(origin, asteroid->transform);
    origin = Vector3Subtract(trans_origin,(Vector3){100,100,100});

    // spawn subs
    switch (asteroid->asteroid_storage.size) {
        case ASTEROID_SIZE_LG:
            for(int i = 0; i < count1; i++){
                Entity e = entity_asteroid_spawn(ASTEROID_SIZE_MD, asteroid->asteroid_storage.color, origin);
                // give children similar direction in 90degree fork
                float rot = (float)(GetRandomValue(-45, 45)) * PI / 180;
                AsteroidStorage ps = asteroid->asteroid_storage;
                e.asteroid_storage.traversal_direction = Vector3RotateByAxisAngle(ps.traversal_direction, (Vector3){1,1,1}, rot);
                vector_push(eo, &e);
            }
            for(int i = 0; i < count2; i++){
                Entity e = entity_asteroid_spawn(ASTEROID_SIZE_SM, asteroid->asteroid_storage.color, origin);
                // give children similar direction in 90degree fork
                float rot = (float)(GetRandomValue(-45, 45)) * PI / 180;
                AsteroidStorage ps = asteroid->asteroid_storage;
                e.asteroid_storage.traversal_direction = Vector3RotateByAxisAngle(ps.traversal_direction, (Vector3){1,1,1}, rot);
                vector_push(eo, &e);
            }
            break;
        case ASTEROID_SIZE_MD:
            for(int i = 0; i < count1; i++){
                Entity e = entity_asteroid_spawn(ASTEROID_SIZE_SM, asteroid->asteroid_storage.color, origin);
                // give children similar direction in 90degree fork
                float rot = (float)(GetRandomValue(-45, 45)) * PI / 180;
                AsteroidStorage ps = asteroid->asteroid_storage;
                e.asteroid_storage.traversal_direction = Vector3RotateByAxisAngle(ps.traversal_direction, (Vector3){1,1,1}, rot);
                vector_push(eo, &e);
            }
            break;
        case ASTEROID_SIZE_SM:
            // nothing, maybe item drops or something
            break;
        default:
            fprintf(stderr, "E: bad asteroid size in kill\n");
            break;
    }
}

void entity_asteroid_draw_2d(Entity * asteroid){
    (void)asteroid;
}

void entity_asteroid_draw_3d(Entity * asteroid){
    DrawMesh(asteroid->mesh, asteroid->material, asteroid->transform);
}
