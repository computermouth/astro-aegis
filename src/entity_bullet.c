
#include <stdio.h>

#include "entity_player.h"
#include "game.h"
#include "raylib.h"

#include "entity_bullet.h"
#include "raymath.h"
#include "resource.h"
#include "vector.h"
#include "weapon.h"
#include "entity.h"
#include "tool.h"

void entity_bullet_draw_2d(Entity * bullet);
void entity_bullet_draw_3d(Entity * bullet);
void entity_bullet_kill(Entity * bullet);
void entity_bullet_check_collision(Entity * b, Matrix old_transform);

Entity entity_bullet_spawn(WeaponType w, Vector2 shoot_dir, float speed){

    Entity b = {
        .draw_2d_fn = entity_bullet_draw_2d,
        .draw_3d_fn = entity_bullet_draw_3d,
        // .material = LoadMaterialDefault(),
        // .mesh = red_bullet_msh,
        .transform = MatrixIdentity(),
        .type = ENTITY_TYPE_BULLET,
    };

    switch (w){
        case WEAPON_RED:
            b.material = red_bullet_mat;
            b.mesh = red_bullet_msh;
            break;
        case WEAPON_GRN:
            b.material = grn_bullet_mat;
            b.mesh = grn_bullet_msh;
            break;
        case WEAPON_BLU:
            b.material = blu_bullet_mat;
            b.mesh = blu_bullet_msh;
            break;
        default:
            fprintf(stderr, "E: unknown weapon???\n");
            break;
    }

    BulletStorage bs = {
        .dir = shoot_dir,
        .speed = speed,
        .distance_traveled = 0,
        .origin_time = game_get_time(),
        .origin_rotation = game_get_globe_entity()->globe_storage.net_rotation,
        .w = w,
    };

    b.bullet_storage = bs;

    Vector3 offset_in = Vector3Transform((Vector3){3.5, 0, 0}, QuaternionToMatrix(QuaternionInvert(bs.origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;

    Vector3 initial_d = {0, bs.dir.y, bs.dir.x};
    Vector3 rotated_direction = Vector3Transform(initial_d, QuaternionToMatrix(QuaternionInvert(q_globe)));
    Vector3 traversal_axis = Vector3Normalize(Vector3CrossProduct(rotated_direction, Vector3Normalize(offset_in)));
    Matrix traversal_rot = MatrixRotate(traversal_axis, fmodf(bs.speed * (game_get_time() - bs.origin_time), PI * 2) );
    Matrix globe_rot = QuaternionToMatrix(q_globe);
    Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix trans = MatrixTranslate(center.x, center.y, center.z);

    Matrix bullet_transform = MatrixIdentity();
    bullet_transform = MatrixMultiply(bullet_transform, offset);
    bullet_transform = MatrixMultiply(bullet_transform, traversal_rot);
    bullet_transform = MatrixMultiply(bullet_transform, globe_rot);
    bullet_transform = MatrixMultiply(bullet_transform, trans);

    b.transform = bullet_transform;

    return b;
}

void entity_bullet_update(Entity * bullet){

    BulletStorage * bs = &bullet->bullet_storage;

    bs->distance_traveled += bs->speed * game_get_delta();
    if (bs->distance_traveled > 1){
        entity_bullet_kill(bullet);
        return;
    }
    
    Matrix bullet_rotation = QuaternionToMatrix(QuaternionFromAxisAngle((Vector3){1,0,0}, fmodf(32 * game_get_time(), PI * 2)));

    Vector3 offset_in = Vector3Transform((Vector3){3.5, 0, 0}, QuaternionToMatrix(QuaternionInvert(bs->origin_rotation)));
    Matrix offset = MatrixTranslate(offset_in.x, offset_in.y, offset_in.z);
    Quaternion q_globe = game_get_globe_entity()->globe_storage.net_rotation;

    Vector3 initial_d = {0, bs->dir.y, bs->dir.x};
    Vector3 rotated_direction = Vector3Transform(initial_d, QuaternionToMatrix(QuaternionInvert(bs->origin_rotation)));
    Vector3 traversal_axis = Vector3Normalize(Vector3CrossProduct(rotated_direction, Vector3Normalize(offset_in)));
    Matrix traversal_rot = MatrixRotate(traversal_axis, fmodf(bs->speed * (game_get_time() - bs->origin_time), PI * 2) );
    Matrix globe_rot = QuaternionToMatrix(q_globe);
    Vector3 center = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix trans = MatrixTranslate(center.x, center.y, center.z);

    Matrix bullet_transform = MatrixIdentity();
    bullet_transform = MatrixMultiply(bullet_transform, bullet_rotation);
    bullet_transform = MatrixMultiply(bullet_transform, offset);
    bullet_transform = MatrixMultiply(bullet_transform, traversal_rot);
    bullet_transform = MatrixMultiply(bullet_transform, globe_rot);
    bullet_transform = MatrixMultiply(bullet_transform, trans);

    Matrix old_transform = bullet->transform;
    bullet->transform = bullet_transform;

    entity_bullet_check_collision(bullet, old_transform);
}

void entity_bullet_check_collision(Entity * b, Matrix old_transform){

    vector * oe = game_get_other_entities();
    size_t e_len = vector_size(oe);
    Entity * entities = vector_begin(oe); 

    for (size_t i = 0; i < e_len; i++){
        Entity * other = &entities[i];
        if (other->type != ENTITY_TYPE_ASTEROID)
            continue;

        Vector3 o_pos = Vector3Transform((Vector3){0,0,0}, other->transform);

        Vector3 b_old_pos = Vector3Transform((Vector3){0,0,0}, old_transform);
        Vector3 b_new_pos = Vector3Transform((Vector3){0,0,0}, b->transform);

        BoundingBox bb = GetMeshBoundingBox(other->mesh);
        bb.min = Vector3Multiply(bb.min, (Vector3){.4,.4,.4});
        bb.max = Vector3Multiply(bb.max, (Vector3){.4,.4,.4});

        // get radius of a sphere on which the corners of the bounding
        // box lie, halve it because it's fuggin huge
        float bsr = tool_get_bounding_sphere_radius(bb) * .5;

        if ( // .2 is a magic number matching approx bullet size
            (CheckCollisionSpheres(o_pos, bsr, b_old_pos, .2)) ||
            (CheckCollisionSpheres(o_pos, bsr, b_new_pos, .2))
        ){
            fprintf(stderr, "collides!\n");
            entity_bullet_kill(b);
            return;
        }
    }
}

void entity_bullet_kill(Entity * b){
    b->dead = true;
}

void entity_bullet_draw_2d(Entity * bullet){
    (void)bullet;
}

void entity_bullet_draw_3d(Entity * bullet){
    DrawMesh(bullet->mesh, bullet->material, bullet->transform);
}