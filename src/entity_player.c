
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "tool.h"
#include "resource.h"
#include "entity_player.h"
#include "entity.h"
#include "weapon.h"
#include "entity_banner.h"

#include <stddef.h>
#include <stdio.h>

const float ENTITY_PLAYER_FRAME_SPEED = 0.0125;
const float ENTITY_PLAYER_FRICTION = 0.0125;
const float ENTITY_PLAYER_MAX_SPEED = .0075;

void entity_player_draw_2d(Entity * player);
void entity_player_draw_3d(Entity * player);
void entity_player_check_collision(Entity * player);

Entity entity_player_spawn(){

    Matrix scale = MatrixScale(0.05f, 0.05f, 0.05f);
    Matrix rotate_y = QuaternionToMatrix(QuaternionFromAxisAngle((Vector3){0,1,0}, PI / 2 ));
    Matrix rotate_x = QuaternionToMatrix(QuaternionFromAxisAngle((Vector3){0,0,-1}, PI / 2 ));
    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
    Matrix trans = MatrixTranslate(pp.x, pp.y, pp.z);

    Matrix player_transform = MatrixIdentity();
    player_transform = MatrixMultiply(player_transform, scale);
    player_transform = MatrixMultiply(player_transform, rotate_y);
    player_transform = MatrixMultiply(player_transform, rotate_x);
    player_transform = MatrixMultiply(player_transform, trans);

    Entity p = {
        .type = ENTITY_TYPE_PLAYER,
        .mesh = player_mesh,
        .material = player_mat,
        .transform = player_transform,
        .draw_2d_fn = entity_player_draw_2d,
        .draw_3d_fn = entity_player_draw_3d
    };

    p.mesh = ship_model.meshes[0];
    p.material = ship_model.materials[1];

    p.player_storage = (PlayerStorage){ 0 };
    p.player_storage.multi = 1;
    p.player_storage.health = 3;

    return p;
}

void entity_player_update(Entity * player){

    PlayerStorage * ps = &player->player_storage;

    float frame_speed_increase = ENTITY_PLAYER_FRAME_SPEED * game_get_delta();
    float frame_friction_decrease = ENTITY_PLAYER_FRICTION * game_get_delta();
    float max_speed = ENTITY_PLAYER_MAX_SPEED;

    // dumb hack to calculate once
    static float max_xy_speed = 0;
    if (max_xy_speed == 0)
        max_xy_speed = Vector2Length((Vector2){max_speed, max_speed});

    float xdir = 0;
    float zdir = 0;

    // only read input if we're not on the menu
    if (game_get_menu_state() == GAME_MENU_STATE_PLAY) {
        if (IsKeyDown(game_get_game_options().keyboard_key_mv_rt)) {
            xdir = 1;
        } else if (IsKeyDown(game_get_game_options().keyboard_key_mv_lt)) {
            xdir = -1;
        }

        if (IsKeyDown(game_get_game_options().keyboard_key_mv_up)) {
            zdir = 1;
        } else if (IsKeyDown(game_get_game_options().keyboard_key_mv_dn)) {
            zdir = -1;
        }

        ps->shoot_dir.x = 0;
        if (IsKeyDown(game_get_game_options().keyboard_key_fr_lt)) {
            ps->shoot_dir.x = -1;
        } else if (IsKeyDown(game_get_game_options().keyboard_key_fr_rt)) {
            ps->shoot_dir.x = 1;
        }
        ps->shoot_dir.y = 0;
        if (IsKeyDown(game_get_game_options().keyboard_key_fr_up)) {
            ps->shoot_dir.y = -1;
        } else if (IsKeyDown(game_get_game_options().keyboard_key_fr_dn)) {
            ps->shoot_dir.y = 1;
        }

        // weapon switch
        if (IsKeyReleased(game_get_game_options().keyboard_key_wp_lt)){
            ps->weapon_index = (ps->weapon_index + __WEAPON_END - 1) % __WEAPON_END;
        } else if (IsKeyReleased(game_get_game_options().keyboard_key_wp_rt)){
            ps->weapon_index = (ps->weapon_index + 1) % __WEAPON_END;
        }

        // bool was_firing = ps->weapons[ps->weapon_index].is_firing;
        // ps->weapons[ps->weapon_index].is_firing = (ps->shoot_dir.x != 0 || ps->shoot_dir.y != 0);
        // if (!was_firing && ps->weapons[ps->weapon_index].is_firing){
        //     ps->weapons[ps->weapon_index].time_last_fired = game_get_time();
        //     ps->weapons[ps->weapon_index].fire_start_time = game_get_time();
        // }

    } else {
        xdir = ps->menu_input_x;
        zdir = ps->menu_input_z;
    }

    // apply x friction
    if (ps->dir_x < 0  && xdir == 0)
        ps->dir_x = fmin(ps->dir_x + frame_friction_decrease, 0);
    else if (ps->dir_x > 0 && xdir == 0)
        ps->dir_x = fmax(ps->dir_x - frame_friction_decrease, 0);

    // apply z friction
    if (ps->dir_z < 0 && zdir == 0)
        ps->dir_z = fmin(ps->dir_z + frame_friction_decrease, 0);
    else if (ps->dir_z > 0 && zdir == 0)
        ps->dir_z = fmax(ps->dir_z - frame_friction_decrease, 0);

    // calculate velocity
    ps->dir_x = Clamp(ps->dir_x + xdir * frame_speed_increase, -max_speed, max_speed);
    ps->dir_z = Clamp(ps->dir_z + zdir * frame_speed_increase, -max_speed, max_speed);

    // dead stop turn (spend 2 frames turning around)
    // if ( xdir == -1 && old_x >= 0 && player->player_storage.dir_x <=0 && player->player_storage.dir_z == 0 ){
    //     player->player_storage.dir_z = frame_friction_decrease * 2 ;
    // } else if ( xdir == 1 && old_x <= 0 && player->player_storage.dir_x >=0 && player->player_storage.dir_z == 0 ){
    //     player->player_storage.dir_z = frame_friction_decrease * -2 ;
    // } else if ( zdir == -1 && old_z >= 0 && player->player_storage.dir_z <=0 && player->player_storage.dir_x == 0){
    //     player->player_storage.dir_x = frame_friction_decrease * 2 ;
    // } else if ( zdir == 1 && old_z <= 0 && player->player_storage.dir_z >=0 && player->player_storage.dir_x == 0){
    //     player->player_storage.dir_x = frame_friction_decrease * -2 ;
    // }

    // clamp diagonal speed
    if ( (fabs(ps->dir_x) + fabs(ps->dir_z)) > max_xy_speed ){

        // stick on diagonal
        if (fabs(fabs(ps->dir_x) - fabs(ps->dir_z)) < frame_speed_increase){
            ps->dir_x = xdir * max_xy_speed / 2;
            ps->dir_z = zdir * max_xy_speed / 2;
        // steal speed from the other axis
        } else {
            if (fabs(ps->dir_x) > max_xy_speed / 2) {
                ps->dir_x -= xdir * frame_speed_increase * 2;
            } else if (fabs(ps->dir_z) > max_xy_speed / 2) {
                ps->dir_z -= zdir * frame_speed_increase * 2;
            }
        }
    }

    // fmod is to make sure the accel fades in at game start
    SetMusicVolume(accel_music, game_get_game_options().sound_volume / 10 * 10 * (fabsf(ps->dir_x) + fabs(ps->dir_z)));

    ps->frame_rotation = QuaternionMultiply(
        QuaternionFromAxisAngle((Vector3){0,1,0}, ps->dir_x),
        QuaternionFromAxisAngle((Vector3){0,0,1}, ps->dir_z)
    );

    Vector2 dir_norm = Vector2Normalize((Vector2){ps->dir_x, ps->dir_z});
    if (xdir != 0 || zdir != 0){
        // player rot
        float rads = atan2f(-dir_norm.x, dir_norm.y);

        Quaternion qxz = QuaternionFromAxisAngle((Vector3){.x = 1, .y = 0, .z = 0}, rads);
        Matrix m_rot = QuaternionToMatrix(qxz);

        Matrix scale = MatrixScale(0.05f, 0.05f, 0.05f);
        Matrix rotate_y = QuaternionToMatrix(QuaternionFromAxisAngle((Vector3){0,1,0}, PI / 2 ));
        Matrix rotate_x = QuaternionToMatrix(QuaternionFromAxisAngle((Vector3){0,0,-1}, PI / 2 ));
        // player pos + rot
        Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
        Matrix trans = MatrixTranslate(pp.x, pp.y, pp.z);

        Matrix player_transform = MatrixIdentity();
        player_transform = MatrixMultiply(player_transform, scale);
        player_transform = MatrixMultiply(player_transform, rotate_y);
        player_transform = MatrixMultiply(player_transform, rotate_x);
        player_transform = MatrixMultiply(player_transform, m_rot);
        player_transform = MatrixMultiply(player_transform, trans);

        player->transform = player_transform;
    }

    if (ps->shoot_dir.x || ps->shoot_dir.y){
        ps->shoot_dir = Vector2Normalize(ps->shoot_dir);
        weapon_fire(ps->weapon_index, &ps->weapons[ps->weapon_index], ps->shoot_dir);
    }

    if (
        game_get_menu_state() == GAME_MENU_STATE_PLAY && 
        player->player_storage.invincible_until < game_get_time()
    )
        entity_player_check_collision(player);
}

void entity_player_kill(){
    game_init_game_over();
}

void entity_player_take_damage(Entity * player){
    player->player_storage.streak = 0;
    player->player_storage.multi = 1;
    player->player_storage.health--;

    if (player->player_storage.health <= 0){
        entity_player_kill();
    }

    // invincible for 1s
    player->player_storage.invincible_until = game_get_time() + 1.0;
}

void entity_player_check_collision(Entity * player){

    vector * oe = game_get_other_entities();
    size_t e_len = vector_size(oe);
    Entity * entities = vector_begin(oe); 

    for (size_t i = 0; i < e_len; i++){
        Entity * other = &entities[i];
        if (other->type != ENTITY_TYPE_ASTEROID)
            continue;

        Vector3 o_pos = Vector3Transform((Vector3){0,0,0}, other->transform);

        Vector3 p_pos = tool_vec3_world_pos((Vector3){3.5,0,0});

        BoundingBox bb = GetMeshBoundingBox(other->mesh);
        bb.min = Vector3Multiply(bb.min, (Vector3){.4,.4,.4});
        bb.max = Vector3Multiply(bb.max, (Vector3){.4,.4,.4});

        // get radius of a sphere on which the corners of the bounding
        // box lie, halve it because it's fuggin huge
        float bsr = tool_get_bounding_sphere_radius(bb) * .5;

        // .05 is a magic number, ship looks about 1/4 the size of a bullet
        if (CheckCollisionSpheres(o_pos, bsr, p_pos, .05)) {
            if(!other->dead){
                
                // boost smash
                // if (player.is_boosting)
                //  //do entity_asteroid_take_damage(other, b->bullet_storage.w);
                // else
                PlaySound(player_hit_snd);
                entity_player_take_damage(player);
                return;
            }
        }
    }
}

char pdir_x[100];
char pdir_z[100];
char mouse_dir_z[100];
char mouse_dir_z[100];
char health[100];
char streak[100];
char multi[100];
char score[100];
char w_r[100];
char w_g[100];
char w_b[100];

void entity_player_draw_2d(Entity * player){
    // sprintf(pdir_x, "pdir_x: %f", player->player_storage.dir_x);
    // sprintf(pdir_z, "pdir_z: %f", player->player_storage.dir_z);
    // DrawText(pdir_x, 1000, 32, 32, RAYWHITE);
    // DrawText(pdir_z, 1000, 64, 32, RAYWHITE);

    // sprintf(pdir_x, "mdir_x: %f", player->player_storage.shoot_dir.x);
    // sprintf(pdir_z, "mdir_y: %f", player->player_storage.shoot_dir.y);
    // DrawText(pdir_x, 1000, 96, 32, RAYWHITE);
    // DrawText(pdir_z, 1000, 128, 32, RAYWHITE);

    // sprintf(streak, "streak: %d",  player->player_storage.streak);
    sprintf(multi, "multi: %dx", player->player_storage.multi);
    sprintf(score, "score: %d",  (int)player->player_storage.score);
    DrawText(multi, 32, GAME_SCREEN_HEIGHT - 32 * 2, 32, RAYWHITE);
    DrawText(score, 32, GAME_SCREEN_HEIGHT - 32 * 3, 32, RAYWHITE);

    sprintf(health, "shields: %d", player->player_storage.health);
    if (player->player_storage.weapons[WEAPON_RED].power < 4)
        sprintf(w_r   , "phaser: %.3f", player->player_storage.weapons[WEAPON_RED].power + 1);
    else
        sprintf(w_r   , "phaser: MAX");
    if (player->player_storage.weapons[WEAPON_GRN].power < 4)
        sprintf(w_g   , "spread: %.3f", player->player_storage.weapons[WEAPON_GRN].power + 1);
    else
        sprintf(w_g   , "spread: MAX");
    if (player->player_storage.weapons[WEAPON_BLU].power < 4)
        sprintf(w_b   , "ngwave: %.3f", player->player_storage.weapons[WEAPON_BLU].power + 1);
    else
        sprintf(w_b   , "ngwave: MAX");

    DrawText(health,   GAME_SCREEN_WIDTH - 32 - MeasureText("phaser: 5.555",  32), GAME_SCREEN_HEIGHT - 32 * 6, 32, ORANGE);
    DrawText(w_r,      GAME_SCREEN_WIDTH - 32 - MeasureText("phaser: 5.555",     32), GAME_SCREEN_HEIGHT - 32 * 4, 32, RED);
    DrawText(w_g,      GAME_SCREEN_WIDTH - 32 - MeasureText("phaser: 5.555",     32), GAME_SCREEN_HEIGHT - 32 * 3, 32, GREEN);
    DrawText(w_b,      GAME_SCREEN_WIDTH - 32 - MeasureText("phaser: 5.555",     32), GAME_SCREEN_HEIGHT - 32 * 2, 32, BLUE);

    char * selector = ">";
    DrawText(selector, GAME_SCREEN_WIDTH - 32 - MeasureText("phaser: 5.555",     32) - MeasureText(selector, 32) * 2, GAME_SCREEN_HEIGHT - 32 * 4 + (32 * player->player_storage.weapon_index), 32, RAYWHITE);

    if (game_get_game_options().draw_fps)
        DrawFPS(10, 10);
}

void entity_player_draw_3d(Entity * player){
    
    float gt = game_get_time();
    // blink if invis
    if (
        player->player_storage.invincible_until > gt && 
        fmodf(gt, .2) < 0.1
    ){ return; }

    DrawMesh(player->mesh, player->material, player->transform);
}

void entity_player_inc_streak(){
    Entity * p = game_get_player_entity();
    p->player_storage.streak++;
    // increase multi
    if (p->player_storage.streak % 23 == 0){
        PlaySound(powerup_snd);
        PlaySound(multiplier_pu_snd);
        Entity e = entity_banner_spawn(BANNER_TTS_MULTI, (Vector2){GAME_SCREEN_WIDTH - 32, 100});
        vector *oe = game_get_other_entities();
        vector_push(oe, &e);
        p->player_storage.multi++;
    }
    // gain health
    if (p->player_storage.streak % 97 == 0){
        PlaySound(powerup_snd);
        PlaySound(sheild_pu_snd);
        Entity e = entity_banner_spawn(BANNER_TTS_SHD_PU, (Vector2){GAME_SCREEN_WIDTH - 32, 100});
        vector *oe = game_get_other_entities();
        vector_push(oe, &e);
        p->player_storage.health++;
    }
    // other gain health
    if (p->player_storage.score - p->player_storage.last_health_reward >= 251){
        PlaySound(powerup_snd);
        PlaySound(sheild_pu_snd);
        Entity e = entity_banner_spawn(BANNER_TTS_SHD_PU, (Vector2){GAME_SCREEN_WIDTH - 32, 100});
        vector *oe = game_get_other_entities();
        vector_push(oe, &e);
        p->player_storage.health++;
        p->player_storage.last_health_reward += 251;
    }
}

void entity_player_add_score(int i){
    entity_player_inc_streak();
    Entity * p = game_get_player_entity();
    p->player_storage.score += i * p->player_storage.multi;
}
