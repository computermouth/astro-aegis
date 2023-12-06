
#include "game.h"
#include "raylib.h"
#include "raymath.h"

#include "tool.h"
#include "resource.h"
#include "entity_player.h"
#include "entity.h"

#include <stddef.h>
#include <stdio.h>

const float ENTITY_PLAYER_FRAME_SPEED = 0.015 / 2;
const float ENTITY_PLAYER_FRICTION = 0.0075 / 2;
const float ENTITY_PLAYER_MAX_SPEED = .01 / 2;

void entity_player_draw_2d(Entity * player);
void entity_player_draw_3d(Entity * player);

Entity entity_player_spawn(){

    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
    Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);

    return (Entity) {
        .type = ENTITY_TYPE_PLAYER,
        .mesh = player_mesh,
        .material = player_mat,
        .transform = pp_matrix,
        .draw_2d_fn = entity_player_draw_2d,
        .draw_3d_fn = entity_player_draw_3d
    };
}

void entity_player_update(Entity * player){

    float frame_speed_increase = ENTITY_PLAYER_FRAME_SPEED * game_get_delta();
    float frame_friction_decrease = ENTITY_PLAYER_FRICTION * game_get_delta();
    float max_speed = ENTITY_PLAYER_MAX_SPEED;

    // dumb hack to calculate once
    static float max_xy_speed = 0;
    if (max_xy_speed == 0)
        max_xy_speed = Vector2Length((Vector2){max_speed, max_speed});

    float old_x = player->player_storage.dir_x;
    float old_z = player->player_storage.dir_z;

    float xdir = 0;
    float zdir = 0;

    // only read input if we're not on the menu
    if (game_get_menu_state() == GAME_MENU_STATE_PLAY) {
        if (IsKeyDown(KEY_D)) {
            xdir = 1;
        } else if (IsKeyDown(KEY_A)) {
            xdir = -1;
        }

        if (IsKeyDown(KEY_W)) {
            zdir = 1;
        } else if (IsKeyDown(KEY_S)) {
            zdir = -1;
        }
    } else {
        xdir = player->player_storage.menu_input_x;
        zdir = player->player_storage.menu_input_z;
    }

    // apply x friction
    if (player->player_storage.dir_x < 0  && xdir == 0)
        player->player_storage.dir_x = fmin(player->player_storage.dir_x + frame_friction_decrease, 0);
    else if (player->player_storage.dir_x > 0 && xdir == 0)
        player->player_storage.dir_x = fmax(player->player_storage.dir_x - frame_friction_decrease, 0);

    // apply z friction
    if (player->player_storage.dir_z < 0 && zdir == 0)
        player->player_storage.dir_z = fmin(player->player_storage.dir_z + frame_friction_decrease, 0);
    else if (player->player_storage.dir_z > 0 && zdir == 0)
        player->player_storage.dir_z = fmax(player->player_storage.dir_z - frame_friction_decrease, 0);

    // calculate velocity
    player->player_storage.dir_x = Clamp(player->player_storage.dir_x + xdir * frame_speed_increase, -max_speed, max_speed);
    player->player_storage.dir_z = Clamp(player->player_storage.dir_z + zdir * frame_speed_increase, -max_speed, max_speed);

    // dead stop turn (spend 4 frames turning around)
    if ( xdir == -1 && old_x >= 0 && player->player_storage.dir_x <=0 && player->player_storage.dir_z == 0 ){
        player->player_storage.dir_z = frame_friction_decrease * 4 ;
    } else if ( xdir == 1 && old_x <= 0 && player->player_storage.dir_x >=0 && player->player_storage.dir_z == 0 ){
        player->player_storage.dir_z = frame_friction_decrease * -4 ;
    } else if ( zdir == -1 && old_z >= 0 && player->player_storage.dir_z <=0 && player->player_storage.dir_x == 0){
        player->player_storage.dir_x = frame_friction_decrease * 4 ;
    } else if ( zdir == 1 && old_z <= 0 && player->player_storage.dir_z >=0 && player->player_storage.dir_x == 0){
        player->player_storage.dir_x = frame_friction_decrease * -4 ;
    }

    Vector2 dir_norm = Vector2Normalize((Vector2){player->player_storage.dir_x, player->player_storage.dir_z});
    // clamp diagonal speed
    if ( (fabs(player->player_storage.dir_x) + fabs(player->player_storage.dir_z)) > max_xy_speed ){
        player->player_storage.dir_x = dir_norm.x * max_speed;
        player->player_storage.dir_z = dir_norm.y * max_speed;
    }

    player->player_storage.frame_rotation = QuaternionMultiply(
        QuaternionFromAxisAngle((Vector3){0,1,0}, player->player_storage.dir_x),
        QuaternionFromAxisAngle((Vector3){0,0,1}, player->player_storage.dir_z)
    );

    if (xdir != 0 || zdir != 0){
        // player rot
        float rads = atan2f(-dir_norm.x, dir_norm.y);

        Quaternion qxz = QuaternionFromAxisAngle((Vector3){.x = 1, .y = 0, .z = 0}, rads);

        Matrix m_rot = QuaternionToMatrix(qxz);

        // player pos
        Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
        Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);

        // final transform
        Matrix pp_transform = MatrixMultiply(m_rot, pp_matrix);

        player->transform = pp_transform;
    }

}

char pdir_x[100];
char pdir_z[100];

void entity_player_draw_2d(Entity * player){
    sprintf(pdir_x, "pdir_x: %f", player->player_storage.dir_x);
    sprintf(pdir_z, "pdir_z: %f", player->player_storage.dir_z);
    DrawText(pdir_x, 32, 32, 32, RAYWHITE);
    DrawText(pdir_z, 32, 64, 32, RAYWHITE);

    DrawFPS(10, 10);
}

void entity_player_draw_3d(Entity * player){
    DrawMesh(player->mesh, player->material, player->transform);
}