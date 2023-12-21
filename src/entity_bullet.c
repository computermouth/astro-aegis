
#include <stdio.h>

#include "game.h"
#include "raylib.h"

#include "entity_bullet.h"
#include "raymath.h"
#include "resource.h"
#include "weapon.h"
#include "entity.h"
#include "tool.h"

void entity_bullet_draw_2d(Entity * bullet);
void entity_bullet_draw_3d(Entity * bullet);

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
        .origin_time = game_get_time(),
        .origin_rotation = game_get_globe_entity()->globe_storage.net_rotation,
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

    BulletStorage bs = bullet->bullet_storage;

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

    bullet->transform = bullet_transform;
}

void entity_bullet_draw_2d(Entity * bullet){
    (void)bullet;
}

void entity_bullet_draw_3d(Entity * bullet){
    DrawMesh(bullet->mesh, bullet->material, bullet->transform);
}