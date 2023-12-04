
#include "resource.h"
#include "assets.h"
#include "raylib.h"

Mesh globe_mesh = { 0 };
Image globe_img = { 0 };
Texture globe_tex = { 0 };
Material globe_mat = { 0 };

Mesh atmosphere_mesh = { 0 };
Image atmosphere_img = { 0 };
Texture atmosphere_tex = { 0 };
Material atmosphere_mat = { 0 };

Mesh player_mesh = { 0 };
Image player_img = { 0 };
Texture player_tex = { 0 };
Material player_mat = { 0 };

void resource_init(){

    { // globe
        globe_mesh = GenMeshSphere(4, 32, 32);
        globe_img = LoadImageFromMemory(".png", globetex_png, globetex_png_len);
        ImageRotateCW(&globe_img);
        globe_tex = LoadTextureFromImage(globe_img);
        globe_mat = LoadMaterialDefault();
        globe_mat.maps[MATERIAL_MAP_DIFFUSE].texture = globe_tex;
    }

    { // atmosphere
        atmosphere_mesh = GenMeshSphere(4.1, 32, 32);
        atmosphere_img = GenImageColor(1, 1, YELLOW);
        atmosphere_tex = LoadTextureFromImage(atmosphere_img);
        atmosphere_mat = LoadMaterialDefault();
        atmosphere_mat.maps[MATERIAL_MAP_DIFFUSE].texture = atmosphere_tex;
        atmosphere_mat.maps[MATERIAL_MAP_DIFFUSE].color.a = 255 / 10;
    }

    { // player
        player_mesh = GenMeshCone(.05, .125, 4);
        // player_img = GenImageColor(1, 1, YELLOW);
        player_img = GenImageGradientSquare(5, 5, 0, YELLOW, RED);
        // player_img = GenImageGradientRadial(5, 5, 1, YELLOW, RED);
        player_tex = LoadTextureFromImage(player_img);
        player_mat = LoadMaterialDefault();
        player_mat.maps[MATERIAL_MAP_DIFFUSE].texture = player_tex;
        // glow??
        // player_mat.maps[MATERIAL_MAP_EMISSION].texture = player_tex;
        // player_mat.maps[MATERIAL_MAP_EMISSION].color = WHITE;
    }

}

void resource_quit(){

    UnloadMesh(globe_mesh);
    UnloadImage(globe_img);
    UnloadTexture(globe_tex);
    UnloadMaterial(globe_mat);

    UnloadMesh(atmosphere_mesh);
    UnloadImage(atmosphere_img);
    UnloadTexture(atmosphere_tex);
    UnloadMaterial(atmosphere_mat);

    UnloadMesh(player_mesh);
    UnloadImage(player_img);
    UnloadTexture(player_tex);
    UnloadMaterial(player_mat);

}