
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
    
    // centered (at worldpos), no rotation
    Vector3 pp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix transform = MatrixTranslate(pp.x, pp.y, pp.z);

    Entity g = {
        .type = ENTITY_TYPE_GLOBE,
        .mesh = globe_mesh,
        .material = globe_mat,
        .transform = transform,
        .draw_2d_fn = entity_globe_draw_2d,
        .draw_3d_fn = entity_globe_draw_3d,
    };
    g.globe_storage.net_rotation = QuaternionIdentity();

    return g;
}

void entity_globe_update(Entity *globe) {

    Entity * player = game_get_player_entity();

    Quaternion old_q = globe->globe_storage.net_rotation;
    Quaternion new_q = QuaternionNormalize(QuaternionMultiply(QuaternionInvert(player->player_storage.frame_rotation), old_q));
    globe->globe_storage.net_rotation = new_q;

    // rot here is just to put the poles at the top and bottom at the start
    Quaternion rot = QuaternionNormalize(QuaternionFromAxisAngle((Vector3){1,1,0}, PI / 2));
    new_q = QuaternionMultiply(new_q, rot);
    Vector3 wp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix wp_matrix = MatrixTranslate(wp.x, wp.y, wp.z);
    Matrix new_transform = MatrixMultiply(QuaternionToMatrix(new_q), wp_matrix);

    globe->transform = new_transform;
}

void entity_globe_draw_2d(Entity * globe){
    (void)globe;
}

void entity_globe_draw_3d(Entity * globe){
    
    // skybox
    DrawMesh(skybox_model.meshes[0], skybox_model.materials[1], globe->transform);

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