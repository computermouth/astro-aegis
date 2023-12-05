
#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "game.h"

#include "resource.h"
#include "tool.h"
#include "entity.h"
#include "vector.h"
#include "entity_globe.h"
#include "entity_player.h"


int GAME_SCREEN_WIDTH = 1280;
int GAME_SCREEN_HEIGHT = 720;

static Game game = {
    .game_time = 0.0f,
    .game_delta = 0.0f,
    .game_menu_state = GAME_MENU_STATE_INIT,
    .game_play_state = GAME_PLAY_STATE_INIT,
    .game_entities = { 0 },
    .game_camera = { 0 },
};

static bool game_should_quit = false;

float game_get_time(){
    return game.game_time;
}

float game_get_delta(){
    return game.game_delta;
}

// emscripten quit or set bool
// checked in main loop
void game_set_should_quit(){
#if defined(PLATFORM_WEB)
    emscripten_cancel_main_loop();
#endif
    game_should_quit = true;
}

// main loop bool check for native build quit
bool game_get_should_quit(){
    return game_should_quit;
}

static float angle = 0.0f;

void game_update_menu(){

    game.game_delta = GetFrameTime();
    game.game_time += game.game_delta;

    if (game.game_menu_state == GAME_MENU_STATE_INIT){
        game.game_camera = (Camera3D) {
            .position = tool_vec3_world_pos((Vector3){8.0f, 0.0f, 0.0f}),
            .target = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f}),
            .up = (Vector3){0.0f,1.0f,0.0f},
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE,
        };
        game.game_menu_state = GAME_MENU_STATE_MENU;
    }

    angle += game.game_delta / 8;
    angle = fmodf(angle, 2 * PI);

    Quaternion qx = QuaternionFromAxisAngle((Vector3){-0.97, 0, 0}, angle);
    Quaternion qy = QuaternionFromAxisAngle((Vector3){0, -0.71, 0}, angle);
    Quaternion qz = QuaternionFromAxisAngle((Vector3){0, 0, -0.53}, angle);
    Quaternion qxyz = QuaternionMultiply(qx, QuaternionMultiply(qy, qz));
    Matrix rot = QuaternionToMatrix(QuaternionNormalize(qxyz));

    Vector3 wp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix wp_matrix = MatrixTranslate(wp.x, wp.y, wp.z);
    Matrix globe_transform = MatrixMultiply(rot, wp_matrix);

    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
    // track planet
    Quaternion px = QuaternionFromAxisAngle((Vector3){0.97, 0, 0}, PI / 2 + angle);
    // barrel roll
    Quaternion pz = QuaternionFromAxisAngle((Vector3){0, 1, 0}, angle * 2);
    Quaternion pxz = QuaternionMultiply(px, pz);
    Matrix pp_rot = QuaternionToMatrix(QuaternionNormalize(pxz));
    Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);
    // Matrix pp_transform = MatrixMultiply(pp_rot, pp_matrix);
    
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(game.game_camera);

        DrawMesh(globe_mesh, globe_mat, globe_transform);
        DrawMesh(atmosphere_mesh, atmosphere_mat, globe_transform);
        for(int i = 0; i < atmosphere_mesh.vertexCount; i += 12) {
            DrawSphereEx(
                Vector3Transform((Vector3){
                    atmosphere_mesh.vertices[i * 3 + 0],
                    atmosphere_mesh.vertices[i * 3 + 1],
                    atmosphere_mesh.vertices[i * 3 + 2],
                }, globe_transform),
                0.01,
                3,
                3,
                (Color){.r = 192, .g = 192, .b = 0, .a = 192}
            );
        }

        DrawMesh(player_mesh, player_mat, pp_matrix);
        
        EndMode3D();

        // BeginMode2D

        GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

        DrawText("ASTRO AEGIS", 32, 32, 64, RAYWHITE);
        DrawLineEx((Vector2){.x = 24, .y = 100}, (Vector2){.x = 490, .y = 100}, 8.0f, RAYWHITE);
        
        if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 - 100, .width = 200, .height = 64}, "start")){
            game.game_menu_state = GAME_MENU_STATE_PLAY;
            game.game_play_state = GAME_PLAY_STATE_INIT;
        }

        GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32, .width = 200, .height = 64}, "options");

        if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 + 100, .width = 200, .height = 64}, "exit")){
            game_set_should_quit();
        }

		DrawFPS(10, 10);

        // EndMode2D

    EndDrawing();

}

void game_update_play(){

    // run start
    if(game.game_play_state == GAME_PLAY_STATE_INIT){
        if(game.game_entities.others)
            vector_free(game.game_entities.others);
        game.game_entities.globe = entity_globe_spawn();
        game.game_entities.player = entity_player_spawn();
        game.game_entities.others = vector_init(sizeof(Entity));
    }

    entity_player_update(&game.game_entities.player);
    entity_globe_update(&game.game_entities.globe, game.game_entities.player);

    // update all entities
    size_t e_len = vector_size(game.game_entities.others);
    Entity * entities = vector_begin(game.game_entities.others);
    for(size_t i = 0; i < e_len; i++){
        switch (entities[i].type) {
            case ENTITY_TYPE_ASTEROID:
                // entity_asteroid_update();
                break;
            default:
                printf("E: encountered unknown entity type: %d\n", entities[i].type);
                break;
        }
    }

    // TODO: cleanup dead entities
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(game.game_camera);

        // draw special entities
        DrawMesh(game.game_entities.player.mesh, game.game_entities.player.material, game.game_entities.player.transform);
        // DrawMesh(game.game_entities.globe.mesh, game.game_entities.globe.material, game.game_entities.globe.transform);

        // draw all entities
        e_len = vector_size(game.game_entities.others);
        entities = vector_begin(game.game_entities.others);
        for(size_t i = 0; i < e_len; i++){
            DrawMesh(entities[i].mesh, entities[i].material, entities[i].transform);
        }

        EndMode3D();
    EndDrawing();

}

void game_update() {
    switch (game.game_menu_state) {
        case GAME_MENU_STATE_INIT:
            game_update_menu();
            break;
        case GAME_MENU_STATE_MENU:
            game_update_menu();
            break;
        case GAME_MENU_STATE_PLAY:
            game_update_play();
            break;
    }
}

void game_set_menu_state(GameMenuState new_gs) {
    game.game_menu_state = new_gs;
}

GameMenuState game_get_menu_state(){
    return game.game_menu_state;
}

void game_set_play_state(GamePlayState new_gs) {
    game.game_play_state = new_gs;
}

GamePlayState game_get_play_state(){
    return game.game_play_state;
}

void game_quit(){
    if(game.game_entities.others)
        vector_free(game.game_entities.others);
}