
#include "resource.h"
#include "assets.h"
#include "raylib.h"

Mesh globe_mesh = { 0 };
Image globe_img = { 0 };
Texture globe_tex = { 0 };
Material globe_mat = { 0 };

Mesh player_mesh = { 0 };
Image player_img = { 0 };
Texture player_tex = { 0 };
Material player_mat = { 0 };

void resource_init(){

    { // globe
        globe_mesh = GenMeshSphere(4, 10, 10);
        globe_img = LoadImageFromMemory(".png", globetex_png, globetex_png_len);
        ImageRotateCW(&globe_img);
        globe_tex = LoadTextureFromImage(globe_img);
        globe_mat = LoadMaterialDefault();
        globe_mat.maps[MATERIAL_MAP_DIFFUSE].texture = globe_tex;
    }

    { // player
        player_mesh = GenMeshCone(1, 1, 4);
        player_img = GenImageColor(1, 1, YELLOW);
        player_tex = LoadTextureFromImage(player_img);
        player_mat = LoadMaterialDefault();
        player_mat.maps[MATERIAL_MAP_DIFFUSE].texture = player_tex;
    }

}

void resource_quit(){

    UnloadMesh(globe_mesh);
    UnloadImage(globe_img);
    UnloadTexture(globe_tex);
    UnloadMaterial(globe_mat);

    UnloadMesh(player_mesh);
    UnloadImage(player_img);
    UnloadTexture(player_tex);
    UnloadMaterial(player_mat);

}