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
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY, 
    SCREEN_ENDING
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;

// TODO: Define global variables here, recommended to make them static

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib gamejam template");
    
    resource_init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    resource_quit();

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame

float game_time = 0.0f;
float angle = 0;

void UpdateDrawFrame(void)
{

    Camera3D camera = {
        .position = tool_vec3_world_pos((Vector3){10.0f, 0.0f, 10.0f}),
        .target = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f}),
        .up = (Vector3){0.0f,1.0f,0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    float delta = GetFrameTime();

    angle += delta;
    angle = fmodf(angle, 2 * PI);

    Quaternion qx = QuaternionFromAxisAngle((Vector3){0.97, 0, 0}, angle);
    Quaternion qy = QuaternionFromAxisAngle((Vector3){0, 0.71, 0}, angle);
    Quaternion qz = QuaternionFromAxisAngle((Vector3){0, 0, 0.53}, angle);
    Quaternion qxyz = QuaternionMultiply(qx, QuaternionMultiply(qy, qz));
    Matrix rot = QuaternionToMatrix(QuaternionNormalize(qxyz));

    Vector3 wp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix wp_matrix = MatrixTranslate(wp.x, wp.y, wp.z);
    Matrix globe_transform = MatrixMultiply(rot, wp_matrix);
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawMesh(globe_mesh, globe_mat, globe_transform);
        
        EndMode3D();

		DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------  
}