
#include "raylib.h"
#include "raymath.h"

#include "tool.h"
#include "resource.h"
#include "entity_player.h"
#include "entity.h"

Entity entity_player_spawn(){

    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
    // track planet
    // Quaternion px = QuaternionFromAxisAngle((Vector3){0.97, 0, 0}, PI / 2 + angle);
    // barrel roll
    // Quaternion pz = QuaternionFromAxisAngle((Vector3){0, 1, 0}, angle * 2);
    // Quaternion pxz = QuaternionMultiply(px, pz);
    // Matrix pp_rot = QuaternionToMatrix(QuaternionNormalize(pxz));
    Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);
    // Matrix pp_transform = MatrixMultiply(pp_rot, pp_matrix);

    return (Entity) {
        .type = ENTITY_TYPE_PLAYER,
        .mesh = player_mesh,
        .material = player_mat,
        .transform = pp_matrix
    };
}

void entity_player_update(Entity * player){}