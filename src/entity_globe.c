
#include "raylib.h"
#include "raymath.h"

#include "tool.h"
#include "entity_globe.h"
#include "entity.h"
#include "resource.h"
#include "game.h"

void entity_globe_draw_2d(Entity * globe);
void entity_globe_draw_3d(Entity * globe);

Entity entity_globe_spawn(){
    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Quaternion rot = QuaternionFromAxisAngle((Vector3){1,1,0}, PI / 2);
    Matrix wp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);
    Matrix transform = MatrixMultiply(QuaternionToMatrix(QuaternionNormalize(rot)), wp_matrix);

    return (Entity) {
        .type = ENTITY_TYPE_GLOBE,
        .mesh = globe_mesh,
        .material = globe_mat,
        .transform = transform,
        .draw_2d_fn = entity_globe_draw_2d,
        .draw_3d_fn = entity_globe_draw_3d,
    };
}

void entity_globe_update(Entity *globe) {

    Entity * player = game_get_player_entity();

    Quaternion old_q = QuaternionFromMatrix(globe->transform);
    Quaternion new_q = QuaternionMultiply(QuaternionInvert(player->player_storage.frame_rotation), old_q);

    Vector3 wp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix wp_matrix = MatrixTranslate(wp.x, wp.y, wp.z);
    Matrix new_transform = MatrixMultiply(QuaternionToMatrix(QuaternionNormalize(new_q)), wp_matrix);

    globe->transform = new_transform;
}

void entity_globe_draw_2d(Entity * globe){
}

void entity_globe_draw_3d(Entity * globe){
    DrawMesh(globe->mesh, globe->material, globe->transform);

    // dots
    for(int i = 0; i < atmosphere_mesh.vertexCount; i += 12) {
        DrawSphereEx(
            Vector3Transform((Vector3){
                atmosphere_mesh.vertices[i * 3 + 0],
                atmosphere_mesh.vertices[i * 3 + 1],
                atmosphere_mesh.vertices[i * 3 + 2],
            }, globe->transform),
            0.01,
            5,
            5,
            (Color){.r = 192, .g = 192, .b = 0, .a = 192}
        );
    }
    
    // atmosphere
    DrawMesh(atmosphere_mesh, atmosphere_mat, globe->transform);
}