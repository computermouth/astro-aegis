
#include "game.h"
#include "raylib.h"
#include "raymath.h"

#include "tool.h"
#include "resource.h"
#include "entity_player.h"
#include "entity.h"
#include <bits/stdint-uintn.h>
#include <stdio.h>

Entity entity_player_spawn(){

    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
    Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);

    return (Entity) {
        .type = ENTITY_TYPE_PLAYER,
        .mesh = player_mesh,
        .material = player_mat,
        .transform = pp_matrix
    };
}

void entity_player_update(Entity * player){

    float frame_speed_increase = 0.04 * game_get_delta();
    float frame_friction_decrease = 0.02 * game_get_delta();
    float max_speed = .03;

    float old_x = player->player_storage.dir_x;
    float old_z = player->player_storage.dir_z;

    // apply friction
    if (player->player_storage.dir_x < 0 )
        player->player_storage.dir_x = fmin(player->player_storage.dir_x + frame_friction_decrease, 0);
    else if (player->player_storage.dir_x > 0 )
        player->player_storage.dir_x = fmax(player->player_storage.dir_x - frame_friction_decrease, 0);

    if (player->player_storage.dir_z < 0 )
        player->player_storage.dir_z = fmin(player->player_storage.dir_z + frame_friction_decrease, 0);
    else if (player->player_storage.dir_z > 0 )
        player->player_storage.dir_z = fmax(player->player_storage.dir_z - frame_friction_decrease, 0);

    float xdir = 0;
    float zdir = 0;

    if (IsKeyDown(KEY_D)) {
        xdir = 1;
    } else if (IsKeyDown(KEY_A)){
        xdir = -1;
    }

    if (IsKeyDown(KEY_W)){
        zdir = 1;
    } else if (IsKeyDown(KEY_S)) {
        zdir = -1;
    }

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