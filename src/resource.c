
#include "resource.h"
#include "assets.h"

Mesh globe_mesh = { 0 };
Image globe_img = { 0 };
Texture globe_tex = { 0 };
Material globe_mat = { 0 };

void resource_init(){

    globe_mesh = GenMeshSphere(4, 10, 10);
    globe_img = LoadImageFromMemory(".png", globetex_png, globetex_png_len);
    ImageRotateCW(&globe_img);
    globe_tex = LoadTextureFromImage(globe_img);
    globe_mat = LoadMaterialDefault();
    globe_mat.maps[MATERIAL_MAP_DIFFUSE].texture = globe_tex;

}

void resource_quit(){

    UnloadMesh(globe_mesh);
    UnloadImage(globe_img);
    UnloadTexture(globe_tex);
    UnloadMaterial(globe_mat);

}