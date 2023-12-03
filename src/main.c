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

#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

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
    
    // TODO: Load resources / Initialize variables at this point

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
        .position = (Vector3){10.0f, 10.0f, 10.0f},
        .target = (Vector3){0.0f,0.0f,0.0f},
        .up = (Vector3){0.0f,1.0f,0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    float delta = GetFrameTime();

    Mesh cube = GenMeshCube(4, 4, 4);
    Image cube_img = GenImageGradientLinear(10, 10, 1, DARKPURPLE, MAGENTA);
    Texture cube_tex = LoadTextureFromImage(cube_img);
    Model cube_model = LoadModelFromMesh(cube);
    cube_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = cube_tex;
    // Material cube_mat = { 0 };
    // SetMaterialTexture(&cube_mat, MATERIAL_MAP_DIFFUSE, cube_tex);

    angle += delta;
    angle = fmodf(angle, 2 * PI);

    Quaternion qx = QuaternionFromAxisAngle((Vector3){0.97, 0, 0}, angle);
    Quaternion qy = QuaternionFromAxisAngle((Vector3){0, 0.71, 0}, angle);
    Quaternion qz = QuaternionFromAxisAngle((Vector3){0, 0, 0.53}, angle);
    Quaternion qxyz = QuaternionMultiply(qx, QuaternionMultiply(qy, qz));
    Matrix rot = QuaternionToMatrix(QuaternionNormalize(qxyz));
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        // DrawCube((Vector3){1.0f,1.0f,1.0f}, 1, 1, 1, DARKGREEN);
        // DrawCubeWires((Vector3){1.0f,1.0f,1.0f}, 1, 1, 1, GREEN);

        DrawMesh(cube, cube_model.materials[0], rot);
        // DrawModel(cube_model, (Vector3){3.0f,3.0f,1.0f}, 1, WHITE);
        // DrawModelWires(cube_model, (Vector3){3.0f,3.0f,1.0f}, 1, RED);
        
        EndMode3D();

		DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------  
}