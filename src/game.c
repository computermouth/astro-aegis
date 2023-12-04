
#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "game.h"

#include "resource.h"
#include "tool.h"


int GAME_SCREEN_WIDTH = 1280;
int GAME_SCREEN_HEIGHT = 720;

static GameState gs = GAME_STATE_MENU;
static bool game_should_quit = false;

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

    Camera3D camera = {
        .position = tool_vec3_world_pos((Vector3){8.0f, 0.0f, 0.0f}),
        .target = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f}),
        .up = (Vector3){0.0f,1.0f,0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    float delta = GetFrameTime();

    angle += delta / 8;
    angle = fmodf(angle, 2 * PI);

    Quaternion qx = QuaternionFromAxisAngle((Vector3){0.97, 0, 0}, angle);
    Quaternion qy = QuaternionFromAxisAngle((Vector3){0, 0.71, 0}, angle);
    Quaternion qz = QuaternionFromAxisAngle((Vector3){0, 0, 0.53}, angle);
    Quaternion qxyz = QuaternionMultiply(qx, QuaternionMultiply(qy, qz));
    Matrix rot = QuaternionToMatrix(QuaternionNormalize(qxyz));

    Vector3 wp = tool_vec3_world_pos((Vector3){0.0f, 0.0f, 0.0f});
    Matrix wp_matrix = MatrixTranslate(wp.x, wp.y, wp.z);
    Matrix globe_transform = MatrixMultiply(rot, wp_matrix);

    // player pos + rot
    Vector3 pp = tool_vec3_world_pos((Vector3){4.5f, 0.0f, 0.0f});
    Quaternion pz = QuaternionFromAxisAngle((Vector3){0, 1, 0}, angle * 2);
    Matrix pp_rot = QuaternionToMatrix(QuaternionNormalize(pz));
    Matrix pp_matrix = MatrixTranslate(pp.x, pp.y, pp.z);
    Matrix pp_transform = MatrixMultiply(pp_rot, pp_matrix);
    
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

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

        DrawMesh(player_mesh, player_mat, pp_transform);
        
        EndMode3D();

        // BeginMode2D

        GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

        DrawText("ASTRO AEGIS", 32, 32, 64, RAYWHITE);
        DrawLineEx((Vector2){.x = 24, .y = 100}, (Vector2){.x = 490, .y = 100}, 8.0f, RAYWHITE);
        
        if (GuiButton((Rectangle){.x = 32, .y = GAME_SCREEN_HEIGHT / 2.0 - 32 - 100, .width = 200, .height = 64}, "start")){
            gs = GAME_STATE_PLAY;
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

}

void game_update() {
    switch (gs) {
        case GAME_STATE_MENU:
            game_update_menu();
            break;
        case GAME_STATE_PLAY:
            game_update_play(angle);
            break;
        case GAME_STATE_OVER:
            game_update_menu();
            break;
    }
}

void game_set_state(GameState new_gs) {
    gs = new_gs;
}

GameState game_get_state(){
    return gs;
}