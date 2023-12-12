
#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include "game.h"

#include "resource.h"
#include "tool.h"
#include "entity.h"
#include "vector.h"
#include "entity_globe.h"
#include "entity_player.h"
#include "entity_asteroid.h"

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

float dirchange = 0.0f;

void game_update_menu(){

    if (game.game_menu_state == GAME_MENU_STATE_INIT){
        dirchange = 0.0f;

        game.game_camera = (Camera3D) {
            .position = tool_vec3_world_pos((Vector3){8.0f, 0.0f, 0.0f}),
            .target = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f}),
            .up = (Vector3){0.0f,1.0f,0.0f},
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE,
        };

        game.game_menu_state = GAME_MENU_STATE_MENU;

        game.game_entities = (GameEntities){
            .globe = entity_globe_spawn(),
            .player = entity_player_spawn(),
            .others = NULL,
        };

        game.game_entities.player.player_storage.menu_input_x = 0;
        game.game_entities.player.player_storage.menu_input_z = 1;
    }

    // every 5s change player's direction at random
    dirchange += game.game_delta;
    if (dirchange > 5.0f){
        int r = GetRandomValue(0, 3);
        float menu_input_x = -1;
        float menu_input_z = -1;
        if (r & 0x01)
            menu_input_x = 1;
        if (r & 0x10)
            menu_input_z = 1;
        game.game_entities.player.player_storage.menu_input_x = menu_input_x;
        game.game_entities.player.player_storage.menu_input_z = menu_input_z;
        dirchange = fmodf(dirchange, 1.0f);
    }

    entity_player_update(&game.game_entities.player);
    entity_globe_update(&game.game_entities.globe);

    // dynamic camera
    float dirx = game.game_entities.player.player_storage.dir_x;
    float dirz = game.game_entities.player.player_storage.dir_z;
    game.game_camera.position = tool_vec3_world_pos((Vector3){8.0f, 0.0f + dirz * 50, 0.0f - dirx * 50});
    game.game_camera.target   = tool_vec3_world_pos((Vector3){0.0f, 0.0f + dirz * 50, 0.0f - dirx * 50});
    
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(game.game_camera);

        game.game_entities.player.draw_3d_fn(&game.game_entities.player);
        game.game_entities.globe.draw_3d_fn(&game.game_entities.globe);
        
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

        GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 + 100, .width = 200, .height = 64}, "licenses");

        if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 + 200, .width = 200, .height = 64}, "exit")){
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

        game.game_entities.others = vector_init(sizeof(Entity));

        game.game_play_state = GAME_PLAY_STATE_PLAY;
    }

    entity_player_update(&game.game_entities.player);
    entity_globe_update(&game.game_entities.globe);

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
        Entity a = entity_asteroid_spawn(ASTEROID_SIZE_LG, GetRandomValue(ASTEROID_COLOR_BLUE, ASTEROID_COLOR_RED));
        vector_push(game.game_entities.others, &a);
    }

    // dynamic camera
    float dirx = game.game_entities.player.player_storage.dir_x;
    float dirz = game.game_entities.player.player_storage.dir_z;
    game.game_camera.position = tool_vec3_world_pos((Vector3){8.0f, 0.0f + dirz * 50, 0.0f - dirx * 50});
    game.game_camera.target   = tool_vec3_world_pos((Vector3){0.0f, 0.0f + dirz * 50, 0.0f - dirx * 50});

    // update all entities
    size_t e_len = vector_size(game.game_entities.others);
    Entity * entities = vector_begin(game.game_entities.others);
    for(size_t i = 0; i < e_len; i++){
        switch (entities[i].type) {
            case ENTITY_TYPE_ASTEROID:
                entity_asteroid_update(&entities[i]);
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

        // draw all 3d entities
        e_len = vector_size(game.game_entities.others);
        entities = vector_begin(game.game_entities.others);
        for(size_t i = 0; i < e_len; i++){
            if (entities[i].draw_3d_fn){
                entities[i].draw_3d_fn(&entities[i]);
            }
        }

        // draw special 3d entities
        game.game_entities.player.draw_3d_fn(&game.game_entities.player);
        game.game_entities.globe.draw_3d_fn(&game.game_entities.globe);

        EndMode3D();

        // draw special 2d entities
        game.game_entities.player.draw_2d_fn(&game.game_entities.player);
        game.game_entities.globe.draw_2d_fn(&game.game_entities.globe);

        // draw all 2d entities
        e_len = vector_size(game.game_entities.others);
        entities = vector_begin(game.game_entities.others);
        for(size_t i = 0; i < e_len; i++){
            if (entities[i].draw_2d_fn)
                entities[i].draw_2d_fn(&entities[i]);
        }

    EndDrawing();

}

void game_update() {

    game.game_delta = GetFrameTime();
    game.game_time += game.game_delta;

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

Entity * game_get_player_entity(){
    return &game.game_entities.player;
}

Entity * game_get_globe_entity(){
    return &game.game_entities.globe;
}

vector * game_get_other_entities(){
    return game.game_entities.others;
}