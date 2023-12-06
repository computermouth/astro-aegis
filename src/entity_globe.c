
#include "raylib.h"
#include "raymath.h"

#include "tool.h"
#include "entity_globe.h"
#include "entity.h"
#include "resource.h"


Entity entity_globe_spawn(){
    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);

    return (Entity) {
        .type = ENTITY_TYPE_GLOBE,
        .mesh = globe_mesh,
        .material = globe_mat,
        .transform = pp_matrix,
    };
}

void entity_globe_update(Entity *globe, const Entity player) {

    Quaternion old_q = QuaternionFromMatrix(globe->transform);
    Quaternion new_q = QuaternionMultiply(QuaternionInvert(player.player_storage.frame_rotation), old_q);

    Vector3 wp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix wp_matrix = MatrixTranslate(wp.x, wp.y, wp.z);
    Matrix new_transform = MatrixMultiply(QuaternionToMatrix(QuaternionNormalize(new_q)), wp_matrix);

    globe->transform = new_transform;
}