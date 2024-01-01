
#include "level.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
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
#include "level.h"
#include "entity_bullet.h"
#include "entity_banner.h"
#include "assets.h"

int GAME_SCREEN_WIDTH = 1280;
int GAME_SCREEN_HEIGHT = 720;

static Game game = {
    .game_time = 0.0f,
    .game_delta = 0.0f,
    .game_menu_state = GAME_MENU_STATE_INIT,
    .game_play_state = GAME_PLAY_STATE_INIT,
    .game_entities = { 0 },
    .game_camera = { 0 },
    .menu_data.options = {
        .master_volume = 7,
        .music_volume = 10,
        .sound_volume = 10,
        .keyboard_key_mv_up = KEY_W,
        .keyboard_key_mv_dn = KEY_S,
        .keyboard_key_mv_lt = KEY_A,
        .keyboard_key_mv_rt = KEY_D,
        .keyboard_key_fr_up = KEY_UP,
        .keyboard_key_fr_dn = KEY_DOWN,
        .keyboard_key_fr_lt = KEY_LEFT,
        .keyboard_key_fr_rt = KEY_RIGHT,
        .keyboard_key_wp_lt = KEY_Q,
        .keyboard_key_wp_rt = KEY_E,
		.keyboard_char_mv_up[0] = 'W' ,
		.keyboard_char_mv_dn[0] = 'S' ,
		.keyboard_char_mv_lt[0] = 'A' ,
		.keyboard_char_mv_rt[0] = 'D' ,
		.keyboard_char_fr_up[0] = '^' ,
		.keyboard_char_fr_dn[0] = 'v' ,
		.keyboard_char_fr_lt[0] = '<' ,
		.keyboard_char_fr_rt[0] = '>' ,
		.keyboard_char_wp_lt[0] = 'Q' ,
		.keyboard_char_wp_rt[0] = 'E' ,
		.keyboard_char_mv_up[1] = '\0',
		.keyboard_char_mv_dn[1] = '\0',
		.keyboard_char_mv_lt[1] = '\0',
		.keyboard_char_mv_rt[1] = '\0',
		.keyboard_char_fr_up[1] = '\0',
		.keyboard_char_fr_dn[1] = '\0',
		.keyboard_char_fr_lt[1] = '\0',
		.keyboard_char_fr_rt[1] = '\0',
		.keyboard_char_wp_lt[1] = '\0',
		.keyboard_char_wp_rt[1] = '\0',
    }
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

void game_update_menu_state_menu_draw_2d(){
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    DrawText("ASTRO AEGIS", 32, 32, 64, RAYWHITE);
    DrawLineEx((Vector2){.x = 24, .y = 100}, (Vector2){.x = 490, .y = 100}, 8.0f, RAYWHITE);
    
    if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 - 100, .width = 200, .height = 64}, "start")){
        game.game_menu_state = GAME_MENU_STATE_PLAY;
        game.game_play_state = GAME_PLAY_STATE_INIT;
    }

    if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32, .width = 200, .height = 64}, "options")){
        game.menu_data.state = MENU_STATE_OPTIONS;
    }

    if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 + 100, .width = 200, .height = 64}, "licenses")){
        game.menu_data.state = MENU_STATE_LICENSES;
    }

#if !defined(PLATFORM_WEB)
    if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 + 200, .width = 200, .height = 64}, "exit")){
        game_set_should_quit();
    }
#endif

    if (game_get_game_options().draw_fps)
        DrawFPS(10, 10);
}

void game_update_menu_state_options_draw_2d(){
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    DrawText("OPTIONS", 32, 32, 64, RAYWHITE);
    DrawLineEx((Vector2){.x = 24, .y = 100}, (Vector2){.x = 490, .y = 100}, 8.0f, RAYWHITE);

    DrawText("keys", 40, 128, 32, RAYWHITE);
    DrawLineEx((Vector2){.x = 32, .y = 164}, (Vector2){.x = 128, .y = 164}, 2.0f, RAYWHITE);

    DrawText("movement:", 40, 196, 20, RAYWHITE);

    GuiSetState(STATE_DISABLED);
    if (GuiButton((Rectangle){.x = 96, .y = 228, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_mv_up)){
        // todo
        // I can't figure out how to do this,
        // the KeyPress buffer and CharPress buffer update at different times,
        // and there doesn't seem to be a way to get a char from the KeyPress

        // EndDrawing();
        // int last_key = KEY_NULL;
        // int last_char = 0;

        // for(;;){
        //     BeginDrawing();
        //         ClearBackground(BLACK);
        //         DrawText("PRESS A KEY...", GAME_SCREEN_WIDTH / 2 - MeasureText("PRESS A KEY...", 64) / 2, 200, 64, RAYWHITE);
        //     EndDrawing();
        //     last_char = GetCharPressed();
        //     last_key = GetKeyPressed();
        //     fprintf(stderr, "lk: %d, lc: %d\n", last_key, last_char);
        //     if(last_key || last_char)
        //         break;
        // }
        // fprintf(stderr, "lk: %d, lc: %d\n", last_key, last_char);
    }
    DrawText("up", 40, 228 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 96, .y = 228 + 50 * 1, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_mv_dn);
    DrawText("down", 40,       228 + 50 * 1 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 96, .y = 228 + 50 * 2, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_mv_lt);
    DrawText("left", 40,       228 + 50 * 2 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 96, .y = 228 + 50 * 3, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_mv_rt);
    DrawText("right", 40,       228 + 50 * 3 + 20 - 10, 20, RAYWHITE);

    DrawText("shooting: (arrow keys)", 240, 196, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 296, .y = 228, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_fr_up);
    DrawText("up", 240, 228 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 296, .y = 228 + 50 * 1, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_fr_dn);
    DrawText("down", 240,       228 + 50 * 1 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 296, .y = 228 + 50 * 2, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_fr_lt);
    DrawText("left", 240,       228 + 50 * 2 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 296, .y = 228 + 50 * 3, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_fr_rt);
    DrawText("right", 240,       228 + 50 * 3 + 20 - 10, 20, RAYWHITE);

    DrawText("switch weapon:", 40, 464, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 96, .y = 496, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_wp_lt);
    DrawText("left", 40, 496 + 20 - 10, 20, RAYWHITE);
    GuiButton((Rectangle){.x = 96, .y = 496 + 50 * 1, .width = 50, .height = 40}, game.menu_data.options.keyboard_char_wp_rt);
    DrawText("right", 40,       496 + 50 * 1 + 20 - 10, 20, RAYWHITE);
    GuiSetState(STATE_NORMAL);
    
    DrawText("master volume", GAME_SCREEN_WIDTH / 2. + 32, GAME_SCREEN_HEIGHT / 2.0 - 32 - 200, 20, RAYWHITE);
    GuiDrawRectangle((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 8 - 200, .width = 200, .height = 64}, 2, (Color){131, 131, 131, 255}, (Color){201, 201, 201, 255});
    float old_master = game.menu_data.options.master_volume;
    GuiSlider((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32 + 32, .y = GAME_SCREEN_HEIGHT / 2.0 + 8 - 200, .width = 200 - 64, .height = 32}, "0", "10", &game.menu_data.options.master_volume, 0, 10);
    if (old_master != game.menu_data.options.master_volume)
        SetMasterVolume(game.menu_data.options.master_volume / 10);

    DrawText("music volume", GAME_SCREEN_WIDTH / 2. + 32, GAME_SCREEN_HEIGHT / 2.0 - 32 - 100, 20, RAYWHITE);
    GuiDrawRectangle((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 8 - 100, .width = 200, .height = 64}, 2, (Color){131, 131, 131, 255}, (Color){201, 201, 201, 255});
    float old_music = game.menu_data.options.music_volume;
    GuiSlider((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32 + 32, .y = GAME_SCREEN_HEIGHT / 2.0 + 8 - 100, .width = 200 - 64, .height = 32}, "0", "10", &game.menu_data.options.music_volume, 0, 10);
    if (old_music != game.menu_data.options.music_volume){
        SetMusicVolume(cyber_spider_open_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
        SetMusicVolume(cyber_spider_rest_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
    }

    DrawText("sound volume", GAME_SCREEN_WIDTH / 2. + 32, GAME_SCREEN_HEIGHT / 2.0 - 32, 20, RAYWHITE);
    GuiDrawRectangle((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 8, .width = 200, .height = 64}, 2, (Color){131, 131, 131, 255}, (Color){201, 201, 201, 255});
    float old_sound = game.menu_data.options.sound_volume;
    GuiSlider((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32 + 32, .y = GAME_SCREEN_HEIGHT / 2.0 + 8, .width = 200 - 64, .height = 32}, "0", "10", &game.menu_data.options.sound_volume, 0, 10);
    if (old_sound != game.menu_data.options.sound_volume)
        resource_set_sound_volumes();

    GuiDrawRectangle((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 16 + 100, .width = 200, .height = 64}, 2, (Color){131, 131, 131, 255}, (Color){201, 201, 201, 255});
    GuiCheckBox((Rectangle){.x = GAME_SCREEN_WIDTH / 2. + 32 + 16, .y = GAME_SCREEN_HEIGHT / 2.0 - 0 + 100, .width = 32, .height = 32}, "draw fps", &game.menu_data.options.draw_fps);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    if (GuiButton((Rectangle){.x = GAME_SCREEN_WIDTH / 2. - 100, .y = GAME_SCREEN_HEIGHT - 100 - 32, .width = 200, .height = 64}, "menu")){
        game.menu_data.state = MENU_STATE_IDLE;
    }

    if (game_get_game_options().draw_fps)
        DrawFPS(10, 10);
}

void game_update_menu_state_licenses_draw_2d(){
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    DrawText("LICENSES", 32, 32, 64, RAYWHITE);
    DrawLineEx((Vector2){.x = 24, .y = 100}, (Vector2){.x = 490, .y = 100}, 8.0f, RAYWHITE);

    for(int i = 0; i < 7; i++){
        if (GuiButton((Rectangle){.x = 32, .y = 140 + i * 50, .width = 200, .height = 40}, licenses[i].product_name)){
            game.menu_data.license_shown = i;
        }
    }

    DrawText(
        licenses[game.menu_data.license_shown].product_name,
        32 + 200 + 32,
        140,
        20,
        RAYWHITE
    );

    DrawText(
        licenses[game.menu_data.license_shown].copyright_holder,
        32 + 200 + 32,
        140 + 30,
        20,
        RAYWHITE
    );

    DrawText(
        licenses[game.menu_data.license_shown].description,
        32 + 200 + 32,
        140 + 60,
        20,
        RAYWHITE
    );

    if (GuiButton((Rectangle){.x = 32 + 200 + 32, .y = 140 + 90, .width = 200, .height = 30}, "website")){
        OpenURL(licenses[game.menu_data.license_shown].link);
    }

    DrawText(
        licenses[game.menu_data.license_shown].license,
        32 + 200 + 32,
        140 + 130,
        20,
        RAYWHITE
    );

    // char lic[1000] = { 0 };
    // snprintf(lic, licenses[game.menu_data.license_shown].payload_len, "%s", licenses[game.menu_data.license_shown].payload);

    // GuiTextBox(
    //     (Rectangle){.x = 32 + 200 + 32, .y = 140 + 130, .width = GAME_SCREEN_WIDTH - (32 + 200 + 32) - 32, .height = GAME_SCREEN_HEIGHT - (140 + 60) - (100 + 32) - 96 },
    //     lic,
    //     10,
    //     false
    // );

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    if (GuiButton((Rectangle){.x = GAME_SCREEN_WIDTH / 2. - 100, .y = GAME_SCREEN_HEIGHT - 100 - 32, .width = 200, .height = 64}, "menu")){
        game.menu_data.state = MENU_STATE_IDLE;
    }

    if (game_get_game_options().draw_fps)
        DrawFPS(10, 10);
}

float dirchange = 0.0f;
float last_playtime = 0.0f;
float curr_playtime = 0.0f;

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
        // game.menu_data.state = MENU_STATE_IDLE;

        game.game_entities = (GameEntities){
            .globe = entity_globe_spawn(),
            .player = entity_player_spawn(),
            .others = NULL,
        };

        game.game_entities.player.player_storage.menu_input_x = 0;
        game.game_entities.player.player_storage.menu_input_z = 1;

        game.game_music_desired_volume = 0.16;
        game.game_music_current_volume = 0.04;

        // here because it gets lowered in GAME_OVER
        SetMusicVolume(cyber_spider_open_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
        SetMusicVolume(cyber_spider_rest_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);

        // don't restart the music if the player
        // is resuming from the last game
        if (!IsMusicStreamPlaying(game.game_music)){
            game.game_music = cyber_spider_open_music;
            PlayMusicStream(game.game_music);
        }
        PlayMusicStream(accel_music);
    }

    if(game.game_music_current_volume < game.game_music_desired_volume){
        game.game_music_current_volume += .03 * game_get_delta();
        SetMusicVolume(cyber_spider_open_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
        SetMusicVolume(cyber_spider_rest_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
    }

    UpdateMusicStream(game.game_music);
    UpdateMusicStream(accel_music);

    curr_playtime = GetMusicTimePlayed(game.game_music)/GetMusicTimeLength(game.game_music);
    float playdiff = curr_playtime - last_playtime;
    if (game.game_music.ctxData == cyber_spider_open_music.ctxData && curr_playtime + playdiff >= 1.0){
        StopMusicStream(game.game_music);
        game.game_music = cyber_spider_rest_music;
        PlayMusicStream(game.game_music);
    }
    last_playtime = curr_playtime;

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

        switch (game.menu_data.state){
            case MENU_STATE_IDLE:
                game_update_menu_state_menu_draw_2d();
                break;
            case MENU_STATE_OPTIONS:
                game_update_menu_state_options_draw_2d();
                break;
            case MENU_STATE_LICENSES:
                game_update_menu_state_licenses_draw_2d();
                break;
        }

        // EndMode2D

    EndDrawing();

}

void game_update_play_init(){
    if(game.game_entities.others)
        vector_free(game.game_entities.others);

    game.game_entities.others = vector_init(sizeof(Entity));

    game.game_play_state = GAME_PLAY_STATE_PLAY;

    game.game_level_state = (GameLevelState){
        .level = 0,
        .wave = 0,
        .game_start_time = game.game_time,
        .level_start_time = game.game_time
    };

    PlaySound(level_1_snd);
}

void game_update_play_play(){

    // music
    UpdateMusicStream(game.game_music);
    UpdateMusicStream(accel_music);

    curr_playtime = GetMusicTimePlayed(game.game_music)/GetMusicTimeLength(game.game_music);
    float playdiff = curr_playtime - last_playtime;
    if (game.game_music.ctxData == cyber_spider_open_music.ctxData && curr_playtime + playdiff >= 1.0){
        StopMusicStream(game.game_music);
        game.game_music = cyber_spider_rest_music;
        PlayMusicStream(game.game_music);
    }
    last_playtime = curr_playtime;

    // updates
    entity_player_update(&game.game_entities.player);
    // player died, resetting game
    if (game_get_play_state() == GAME_PLAY_STATE_OVER)
        return;

    entity_globe_update(&game.game_entities.globe);
    level_update(game.game_entities.others, &game.game_level_state);

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
            case ENTITY_TYPE_BULLET:
                entity_bullet_update(&entities[i]);
                break;
            case ENTITY_TYPE_BANNER:
                entity_banner_update(&entities[i]);
                break;
            default:
                printf("E: encountered unknown entity type: %d\n", entities[i].type);
                break;
        }
    }

    // cleanup dead entities
    e_len = vector_size(game.game_entities.others);
    entities = vector_begin(game.game_entities.others);
    vector * new_vec = vector_init(sizeof(Entity));
    for(size_t i = 0; i < e_len; i++){
        // might be more efficient to copy all living
        // to a new vector, hard to say
        if (!entities[i].dead){
            Entity e = entities[i];
            vector_push(new_vec, &e);
        }
    }
    vector_free(game.game_entities.others);
    game.game_entities.others = new_vec;

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

        level_draw_2d(game.game_level_state);

    EndDrawing();
}

char final_score[100];
char final_time[100];

void game_update_play_over(){

    if(game.game_music_current_volume > game.game_music_desired_volume){
        game.game_music_current_volume -= .02 * game_get_delta();
        SetMusicVolume(cyber_spider_open_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
        SetMusicVolume(cyber_spider_rest_music, game.game_music_current_volume * game.menu_data.options.music_volume / 10);
    }

    // music
    UpdateMusicStream(game.game_music);
    UpdateMusicStream(accel_music);

    curr_playtime = GetMusicTimePlayed(game.game_music)/GetMusicTimeLength(game.game_music);
    float playdiff = curr_playtime - last_playtime;
    if (game.game_music.ctxData == cyber_spider_open_music.ctxData && curr_playtime + playdiff >= 1.0){
        StopMusicStream(game.game_music);
        game.game_music = cyber_spider_rest_music;
        PlayMusicStream(game.game_music);
    }
    last_playtime = curr_playtime;

    // updates
    entity_globe_update(&game.game_entities.globe);

    // float camera back to center
    game.game_entities.player.player_storage.dir_x += -game.game_entities.player.player_storage.dir_x / 10 * game_get_delta();
    game.game_entities.player.player_storage.dir_z += -game.game_entities.player.player_storage.dir_z / 10 * game_get_delta();

    // dynamic camera
    float dirx = game.game_entities.player.player_storage.dir_x;
    float dirz = game.game_entities.player.player_storage.dir_z;
    game.game_camera.position = tool_vec3_world_pos((Vector3){8.0f, 0.0f + dirz * 50, 0.0f - dirx * 50});
    game.game_camera.target   = tool_vec3_world_pos((Vector3){0.0f, 0.0f + dirz * 50, 0.0f - dirx * 50});

    if (game.game_entities.others){
        // update all entities
        size_t e_len = vector_size(game.game_entities.others);
        Entity * entities = vector_begin(game.game_entities.others);
        for(size_t i = 0; i < e_len; i++){
            switch (entities[i].type) {
                case ENTITY_TYPE_ASTEROID:
                    entity_asteroid_update(&entities[i]);
                    break;
                case ENTITY_TYPE_BULLET:
                    entity_bullet_update(&entities[i]);
                    break;
                case ENTITY_TYPE_BANNER:
                    entity_banner_update(&entities[i]);
                    break;
                default:
                    printf("E: encountered unknown entity type: %d\n", entities[i].type);
                    break;
            }
        }

        // cleanup dead entities
        e_len = vector_size(game.game_entities.others);
        entities = vector_begin(game.game_entities.others);
        for(size_t i = 0; i < e_len; i++){
            // might be more efficient to copy all living
            // to a new vector, hard to say
            if (entities[i].dead){
                vector_erase(game.game_entities.others, i);
                e_len--;
                i--;
            }
        }
    }

    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(game.game_camera);

        if (game.game_entities.others){
            // draw all 3d entities
            size_t e_len = vector_size(game.game_entities.others);
            Entity * entities = vector_begin(game.game_entities.others);
            for(size_t i = 0; i < e_len; i++){
                if (entities[i].draw_3d_fn){
                    entities[i].draw_3d_fn(&entities[i]);
                }
            }

            // draw special 3d entities
            // game.game_entities.player.draw_3d_fn(&game.game_entities.player);
            game.game_entities.globe.draw_3d_fn(&game.game_entities.globe);
        }

        EndMode3D();
        // void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
        DrawTexturePro(
            black_tex,
            (Rectangle){0,0,1,1},
            (Rectangle){0,0,GAME_SCREEN_WIDTH,GAME_SCREEN_HEIGHT},
            (Vector2){0,0},
            0,
            (Color){255, 255, 255, Clamp(game_get_time() - game.game_over_state.game_over_start_time, 0, 5) / 5 * 255}
        );

        char * go_text = "GAME OVER";
        DrawText(go_text, GAME_SCREEN_WIDTH / 2 - MeasureText(go_text, 64) / 2, 100, 64, (Color){255, 255, 255, Clamp(game_get_time() - game.game_over_state.game_over_start_time, 0, 5) / 5 * 255});

        DrawText(final_score, GAME_SCREEN_WIDTH / 2 - MeasureText(final_score, 32) / 2, 250, 32, (Color){255, 255, 255, Clamp(game_get_time() - game.game_over_state.game_over_start_time, 0, 5) / 5 * 255});
        DrawText(final_time,  GAME_SCREEN_WIDTH / 2 - MeasureText(final_time,  32) / 2, 350, 32, (Color){255, 255, 255, Clamp(game_get_time() - game.game_over_state.game_over_start_time, 0, 5) / 5 * 255});

        if ( Clamp(game_get_time() - game.game_over_state.game_over_start_time, 0, 7) == 7 ){
            if (GuiButton((Rectangle){.x = GAME_SCREEN_WIDTH / 2.0 - 100, .y = 450, .width = 200, .height = 64}, "menu")){
                game_set_menu_state(GAME_MENU_STATE_INIT);
                if (game.game_entities.others != NULL){
                    vector_free(game.game_entities.others);
                    game.game_entities.others = NULL;
                }
            }
        }

    EndDrawing();
}

void game_update_play(){

    switch (game.game_play_state){
        case GAME_PLAY_STATE_INIT:
            game_update_play_init();
            // fallthrough to play_play
        case GAME_PLAY_STATE_PLAY:
            game_update_play_play();
            break;
        case GAME_PLAY_STATE_OVER:
            game_update_play_over();
            break;
    }

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

void game_init_game_over(){
    game.game_play_state = GAME_PLAY_STATE_OVER;
    game.game_music_desired_volume = 0.04;
    game.game_music_current_volume = 0.16;
    SetMusicVolume(accel_music, 0);

    sprintf(final_score, "score: %d", (int)game.game_entities.player.player_storage.score);
    sprintf(final_time, "time: %.2f",  game_get_time() - game.game_level_state.game_start_time);

    game.game_over_state.game_over_start_time = game_get_time();
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

Weapon * game_get_weapon(WeaponType w){
    return &game.game_entities.player.player_storage.weapons[w];
}

Camera game_get_camera(){
    return game.game_camera;
}

GameOptions game_get_game_options(){
    return game.menu_data.options;
}
