/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

// ray includes
#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

// stdlib includes
#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

// local includes
#include "resource.h"
#include "tool.h"
#include "game.h"
#include "entity.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, "astro aegis");
    SetExitKey(KEY_NULL);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose() && !game_get_should_quit())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    game_quit();
    resource_quit();

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// emscripten sucks farts
bool load_started = false;
bool load_finished = false;
float old_t = 0;

void load_start(){

    InitAudioDevice();
    SetMasterVolume(0);
    SetLoadFileDataCallback(resource_load_file_callback);

    char * welcome = "a raylib slo-jam game";
    char * by = "by";
    char * computermouth = "@computermouth";
    int font_size = 64;
    int tw_welcome       = MeasureText(welcome, font_size);
    int tw_by            = MeasureText(by, font_size);
    int tw_computermouth = MeasureText(computermouth, font_size);
    old_t = GetTime();

    BeginDrawing();
        ClearBackground(BLACK);
        DrawText(welcome,       GetScreenWidth() / 2 - tw_welcome       / 2, GetScreenHeight() * 1 / 5,   font_size, RAYWHITE);
        DrawText(by,            GetScreenWidth() / 2 - tw_by            / 2, GetScreenHeight() * 2.2 / 5, font_size, RAYWHITE);
        DrawText(computermouth, GetScreenWidth() / 2 - tw_computermouth / 2, GetScreenHeight() * 3.7 / 5, font_size, RAYWHITE);
    EndDrawing();

    load_started = true;
}

void load_finish(){
    resource_init();
    resource_set_sound_volumes();
    float new_t = GetTime();
    WaitTime(2 - (new_t - old_t));
    SetRandomSeed(GetTime() * 5417 + GetFrameTime() * 7919);
    load_finished = true;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame

// master volume fade-in
float fade_t = 0;
void UpdateDrawFrame(void) {

    // crappy emscripten hacks
    if (!load_started){
        load_start();
        return;
    } else if (!load_finished){
        load_finish();
        return;
    }

    if (fade_t == 0)
        fade_t = GetTime();

    if ((GetTime() - fade_t) / 2 < 1)
        SetMasterVolume(fmodf((GetTime() - fade_t) / 2, 1) * game_get_game_options().sound_volume / 10);
    game_update();
}
