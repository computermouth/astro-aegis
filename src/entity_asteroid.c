
#include "raylib.h"
#include "raymath.h"

#include "entity_asteroid.h"
#include "tool.h"
#include "resource.h"
#include "game.h"

void entity_asteroid_draw_2d(Entity * asteroid);
void entity_asteroid_draw_3d(Entity * asteroid);

Entity entity_asteroid_spawn(){
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
        .draw_2d_fn = entity_asteroid_draw_2d,
        .draw_3d_fn = entity_asteroid_draw_3d,
    };
}
void entity_asteroid_update(Entity * asteroid){

}


void entity_asteroid_draw_2d(Entity * asteroid){}
void entity_asteroid_draw_3d(Entity * asteroid){
    DrawMesh(asteroid->mesh, asteroid->material, asteroid->transform);
}