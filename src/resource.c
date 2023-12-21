
#include "resource.h"
#include "assets.h"

#include "raylib.h"
#include <string.h>

Mesh globe_mesh = { 0 };
Image globe_img = { 0 };
Texture globe_tex = { 0 };
Material globe_mat = { 0 };

Mesh atmosphere_mesh = { 0 };
Image atmosphere_img = { 0 };
Texture atmosphere_tex = { 0 };
Material atmosphere_mat = { 0 };

Model skybox_model = { 0 };

Image    bluetex1_img = { 0 };
Texture  bluetex1_tex = { 0 };
Material bluetex1_mat = { 0 };

Image    bluetex2_img = { 0 };
Texture  bluetex2_tex = { 0 };
Material bluetex2_mat = { 0 };

Image    bluetex3_img = { 0 };
Texture  bluetex3_tex = { 0 };
Material bluetex3_mat = { 0 };

Image    greentex1_img = { 0 };
Texture  greentex1_tex = { 0 };
Material greentex1_mat = { 0 };

Image    greentex2_img = { 0 };
Texture  greentex2_tex = { 0 };
Material greentex2_mat = { 0 };

Image    greentex3_img = { 0 };
Texture  greentex3_tex = { 0 };
Material greentex3_mat = { 0 };

Image    redtex1_img = { 0 };
Texture  redtex1_tex = { 0 };
Material redtex1_mat = { 0 };

Image    redtex2_img = { 0 };
Texture  redtex2_tex = { 0 };
Material redtex2_mat = { 0 };

Image    redtex3_img = { 0 };
Texture  redtex3_tex = { 0 };
Material redtex3_mat = { 0 };

Mesh player_mesh = { 0 };
Image player_img = { 0 };
Texture player_tex = { 0 };
Material player_mat = { 0 };

Model asteroid_b000_model = { 0 };
Model asteroid_b001_model = { 0 };
Model asteroid_b002_model = { 0 };
Model asteroid_b003_model = { 0 };
Model asteroid_b004_model = { 0 };
Model asteroid_b005_model = { 0 };
Model asteroid_b006_model = { 0 };
Model asteroid_b007_model = { 0 };
Model asteroid_b008_model = { 0 };
Model asteroid_b009_model = { 0 };

Model asteroid_m000_model = { 0 };
Model asteroid_m001_model = { 0 };
Model asteroid_m002_model = { 0 };
Model asteroid_m003_model = { 0 };
Model asteroid_m004_model = { 0 };
Model asteroid_m005_model = { 0 };
Model asteroid_m006_model = { 0 };
Model asteroid_m007_model = { 0 };
Model asteroid_m008_model = { 0 };
Model asteroid_m009_model = { 0 };

Model asteroid_s000_model = { 0 };
Model asteroid_s001_model = { 0 };
Model asteroid_s002_model = { 0 };
Model asteroid_s003_model = { 0 };
Model asteroid_s004_model = { 0 };
Model asteroid_s005_model = { 0 };
Model asteroid_s006_model = { 0 };
Model asteroid_s007_model = { 0 };
Model asteroid_s008_model = { 0 };
Model asteroid_s009_model = { 0 };

Model asteroid_models[30] = {0};
Material asteroid_materials[9] = {0};

Music cyber_spider_open_music = { 0 };
Music cyber_spider_rest_music = { 0 };

Image    red_bullet_img = { 0 };
Texture  red_bullet_tex = { 0 };
Material red_bullet_mat = { 0 };
Mesh     red_bullet_msh = { 0 };

Image    grn_bullet_img = { 0 };
Texture  grn_bullet_tex = { 0 };
Material grn_bullet_mat = { 0 };
Mesh     grn_bullet_msh = { 0 };

Image    blu_bullet_img = { 0 };
Texture  blu_bullet_tex = { 0 };
Material blu_bullet_mat = { 0 };
Mesh     blu_bullet_msh = { 0 };

unsigned char * resource_load_file_callback(const char *actually_a_resource_load_stats, int *len);

void resource_init(){

    { // globe
        globe_mesh = GenMeshSphere(2.5, 32, 32);
        globe_img = LoadImageFromMemory(".png", globetex_png, globetex_png_len);
        ImageRotateCW(&globe_img);
        globe_tex = LoadTextureFromImage(globe_img);
        globe_mat = LoadMaterialDefault();
        globe_mat.maps[MATERIAL_MAP_DIFFUSE].texture = globe_tex;
    }

    { // atmosphere
        atmosphere_mesh = GenMeshSphere(3, 32, 32);
        atmosphere_img = GenImageColor(1, 1, YELLOW);
        atmosphere_tex = LoadTextureFromImage(atmosphere_img);
        atmosphere_mat = LoadMaterialDefault();
        atmosphere_mat.maps[MATERIAL_MAP_DIFFUSE].texture = atmosphere_tex;
        atmosphere_mat.maps[MATERIAL_MAP_DIFFUSE].color.a = 255 / 10;
    }

    { // asteroid_b000
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = skybox_glb,
            .len = skybox_glb_len,
        };
        strcpy(r.filename, skybox_filename);
        skybox_model = LoadModel(r.filename);
    }

    { // player
        player_mesh = GenMeshCone(.05, .125, 4);
        // player_mesh = GenMeshSphere(.05, 3, 3);
        player_img = GenImageGradientSquare(5, 5, 0, YELLOW, RED);
        player_tex = LoadTextureFromImage(player_img);
        player_mat = LoadMaterialDefault();
        player_mat.maps[MATERIAL_MAP_DIFFUSE].texture = player_tex;
    }

    { // bluetex1
        bluetex1_img = LoadImageFromMemory(".png", bluetex1_png, bluetex1_png_len);
        bluetex1_tex = LoadTextureFromImage(bluetex1_img);
        bluetex1_mat = LoadMaterialDefault();
        bluetex1_mat.maps[MATERIAL_MAP_DIFFUSE].texture = bluetex1_tex;
    }

    { // bluetex2
        bluetex2_img = LoadImageFromMemory(".png", bluetex2_png, bluetex2_png_len);
        bluetex2_tex = LoadTextureFromImage(bluetex2_img);
        bluetex2_mat = LoadMaterialDefault();
        bluetex2_mat.maps[MATERIAL_MAP_DIFFUSE].texture = bluetex2_tex;
    }

    { // bluetex3
        bluetex3_img = LoadImageFromMemory(".png", bluetex3_png, bluetex3_png_len);
        bluetex3_tex = LoadTextureFromImage(bluetex3_img);
        bluetex3_mat = LoadMaterialDefault();
        bluetex3_mat.maps[MATERIAL_MAP_DIFFUSE].texture = bluetex3_tex;
    }

    { // greentex1
        greentex1_img = LoadImageFromMemory(".png", greentex1_png, greentex1_png_len);
        greentex1_tex = LoadTextureFromImage(greentex1_img);
        greentex1_mat = LoadMaterialDefault();
        greentex1_mat.maps[MATERIAL_MAP_DIFFUSE].texture = greentex1_tex;
    }

    { // greentex2
        greentex2_img = LoadImageFromMemory(".png", greentex2_png, greentex2_png_len);
        greentex2_tex = LoadTextureFromImage(greentex2_img);
        greentex2_mat = LoadMaterialDefault();
        greentex2_mat.maps[MATERIAL_MAP_DIFFUSE].texture = greentex2_tex;
    }

    { // greentex3
        greentex3_img = LoadImageFromMemory(".png", greentex3_png, greentex3_png_len);
        greentex3_tex = LoadTextureFromImage(greentex3_img);
        greentex3_mat = LoadMaterialDefault();
        greentex3_mat.maps[MATERIAL_MAP_DIFFUSE].texture = greentex3_tex;
    }

    { // redtex1
        redtex1_img = LoadImageFromMemory(".png", redtex1_png, redtex1_png_len);
        redtex1_tex = LoadTextureFromImage(redtex1_img);
        redtex1_mat = LoadMaterialDefault();
        redtex1_mat.maps[MATERIAL_MAP_DIFFUSE].texture = redtex1_tex;
    }

    { // redtex2
        redtex2_img = LoadImageFromMemory(".png", redtex2_png, redtex2_png_len);
        redtex2_tex = LoadTextureFromImage(redtex2_img);
        redtex2_mat = LoadMaterialDefault();
        redtex2_mat.maps[MATERIAL_MAP_DIFFUSE].texture = redtex2_tex;
    }

    { // redtex3
        redtex3_img = LoadImageFromMemory(".png", redtex3_png, redtex3_png_len);
        redtex3_tex = LoadTextureFromImage(redtex3_img);
        redtex3_mat = LoadMaterialDefault();
        redtex3_mat.maps[MATERIAL_MAP_DIFFUSE].texture = redtex3_tex;
    }

    { // asteroid_b000
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b000_glb,
            .len = asteroid_b000_glb_len,
        };
        strcpy(r.filename, asteroid_b000_filename);
        asteroid_b000_model = LoadModel(r.filename);
    }

    { // asteroid_b001
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b001_glb,
            .len = asteroid_b001_glb_len,
        };
        strcpy(r.filename, asteroid_b001_filename);
        asteroid_b001_model = LoadModel(r.filename);
    }

    { // asteroid_b002
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b002_glb,
            .len = asteroid_b002_glb_len,
        };
        strcpy(r.filename, asteroid_b002_filename);
        asteroid_b002_model = LoadModel(r.filename);
    }

    { // asteroid_b003
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b003_glb,
            .len = asteroid_b003_glb_len,
        };
        strcpy(r.filename, asteroid_b003_filename);
        asteroid_b003_model = LoadModel(r.filename);
    }

    { // asteroid_b004
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b004_glb,
            .len = asteroid_b004_glb_len,
        };
        strcpy(r.filename, asteroid_b004_filename);
        asteroid_b004_model = LoadModel(r.filename);
    }

    { // asteroid_b005
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b005_glb,
            .len = asteroid_b005_glb_len,
        };
        strcpy(r.filename, asteroid_b005_filename);
        asteroid_b005_model = LoadModel(r.filename);
    }

    { // asteroid_b006
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b006_glb,
            .len = asteroid_b006_glb_len,
        };
        strcpy(r.filename, asteroid_b006_filename);
        asteroid_b006_model = LoadModel(r.filename);
    }

    { // asteroid_b007
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b007_glb,
            .len = asteroid_b007_glb_len,
        };
        strcpy(r.filename, asteroid_b007_filename);
        asteroid_b007_model = LoadModel(r.filename);
    }

    { // asteroid_b008
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b008_glb,
            .len = asteroid_b008_glb_len,
        };
        strcpy(r.filename, asteroid_b008_filename);
        asteroid_b008_model = LoadModel(r.filename);
    }

    { // asteroid_b009
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_b009_glb,
            .len = asteroid_b009_glb_len,
        };
        strcpy(r.filename, asteroid_b009_filename);
        asteroid_b009_model = LoadModel(r.filename);
    }

    { // asteroid_m000
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m000_glb,
            .len = asteroid_m000_glb_len,
        };
        strcpy(r.filename, asteroid_m000_filename);
        asteroid_m000_model = LoadModel(r.filename);
    }

    { // asteroid_m001
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m001_glb,
            .len = asteroid_m001_glb_len,
        };
        strcpy(r.filename, asteroid_m001_filename);
        asteroid_m001_model = LoadModel(r.filename);
    }

    { // asteroid_m002
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m002_glb,
            .len = asteroid_m002_glb_len,
        };
        strcpy(r.filename, asteroid_m002_filename);
        asteroid_m002_model = LoadModel(r.filename);
    }

    { // asteroid_m003
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m003_glb,
            .len = asteroid_m003_glb_len,
        };
        strcpy(r.filename, asteroid_m003_filename);
        asteroid_m003_model = LoadModel(r.filename);
    }

    { // asteroid_m004
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m004_glb,
            .len = asteroid_m004_glb_len,
        };
        strcpy(r.filename, asteroid_m004_filename);
        asteroid_m004_model = LoadModel(r.filename);
    }

    { // asteroid_m005
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m005_glb,
            .len = asteroid_m005_glb_len,
        };
        strcpy(r.filename, asteroid_m005_filename);
        asteroid_m005_model = LoadModel(r.filename);
    }

    { // asteroid_m006
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m006_glb,
            .len = asteroid_m006_glb_len,
        };
        strcpy(r.filename, asteroid_m006_filename);
        asteroid_m006_model = LoadModel(r.filename);
    }

    { // asteroid_m007
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m007_glb,
            .len = asteroid_m007_glb_len,
        };
        strcpy(r.filename, asteroid_m007_filename);
        asteroid_m007_model = LoadModel(r.filename);
    }

    { // asteroid_m008
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m008_glb,
            .len = asteroid_m008_glb_len,
        };
        strcpy(r.filename, asteroid_m008_filename);
        asteroid_m008_model = LoadModel(r.filename);
    }

    { // asteroid_m009
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_m009_glb,
            .len = asteroid_m009_glb_len,
        };
        strcpy(r.filename, asteroid_m009_filename);
        asteroid_m009_model = LoadModel(r.filename);
    }

    { // asteroid_s000
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s000_glb,
            .len = asteroid_s000_glb_len,
        };
        strcpy(r.filename, asteroid_s000_filename);
        asteroid_s000_model = LoadModel(r.filename);
    }

    { // asteroid_s001
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s001_glb,
            .len = asteroid_s001_glb_len,
        };
        strcpy(r.filename, asteroid_s001_filename);
        asteroid_s001_model = LoadModel(r.filename);
    }

    { // asteroid_s002
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s002_glb,
            .len = asteroid_s002_glb_len,
        };
        strcpy(r.filename, asteroid_s002_filename);
        asteroid_s002_model = LoadModel(r.filename);
    }

    { // asteroid_s003
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s003_glb,
            .len = asteroid_s003_glb_len,
        };
        strcpy(r.filename, asteroid_s003_filename);
        asteroid_s003_model = LoadModel(r.filename);
    }

    { // asteroid_s004
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s004_glb,
            .len = asteroid_s004_glb_len,
        };
        strcpy(r.filename, asteroid_s004_filename);
        asteroid_s004_model = LoadModel(r.filename);
    }

    { // asteroid_s005
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s005_glb,
            .len = asteroid_s005_glb_len,
        };
        strcpy(r.filename, asteroid_s005_filename);
        asteroid_s005_model = LoadModel(r.filename);
    }

    { // asteroid_s006
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s006_glb,
            .len = asteroid_s006_glb_len,
        };
        strcpy(r.filename, asteroid_s006_filename);
        asteroid_s006_model = LoadModel(r.filename);
    }

    { // asteroid_s007
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s007_glb,
            .len = asteroid_s007_glb_len,
        };
        strcpy(r.filename, asteroid_s007_filename);
        asteroid_s007_model = LoadModel(r.filename);
    }

    { // asteroid_s008
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s008_glb,
            .len = asteroid_s008_glb_len,
        };
        strcpy(r.filename, asteroid_s008_filename);
        asteroid_s008_model = LoadModel(r.filename);
    }

    { // asteroid_s009
        resource_load_stats r = {
            // .filename = char[100]{0},
            .data = asteroid_s009_glb,
            .len = asteroid_s009_glb_len,
        };
        strcpy(r.filename, asteroid_s009_filename);
        asteroid_s009_model = LoadModel(r.filename);
    }

    { // collection
        asteroid_models[ 0] = asteroid_b000_model;
        asteroid_models[ 1] = asteroid_b001_model;
        asteroid_models[ 2] = asteroid_b002_model;
        asteroid_models[ 3] = asteroid_b003_model;
        asteroid_models[ 4] = asteroid_b004_model;
        asteroid_models[ 5] = asteroid_b005_model;
        asteroid_models[ 6] = asteroid_b006_model;
        asteroid_models[ 7] = asteroid_b007_model;
        asteroid_models[ 8] = asteroid_b008_model;
        asteroid_models[ 9] = asteroid_b009_model;
        asteroid_models[10] = asteroid_m000_model;
        asteroid_models[11] = asteroid_m001_model;
        asteroid_models[12] = asteroid_m002_model;
        asteroid_models[13] = asteroid_m003_model;
        asteroid_models[14] = asteroid_m004_model;
        asteroid_models[15] = asteroid_m005_model;
        asteroid_models[16] = asteroid_m006_model;
        asteroid_models[17] = asteroid_m007_model;
        asteroid_models[18] = asteroid_m008_model;
        asteroid_models[19] = asteroid_m009_model;
        asteroid_models[20] = asteroid_s000_model;
        asteroid_models[21] = asteroid_s001_model;
        asteroid_models[22] = asteroid_s002_model;
        asteroid_models[23] = asteroid_s003_model;
        asteroid_models[24] = asteroid_s004_model;
        asteroid_models[25] = asteroid_s005_model;
        asteroid_models[26] = asteroid_s006_model;
        asteroid_models[27] = asteroid_s007_model;
        asteroid_models[28] = asteroid_s008_model;
        asteroid_models[29] = asteroid_s009_model;

        asteroid_materials[0] = bluetex1_mat;
        asteroid_materials[1] = bluetex2_mat;
        asteroid_materials[2] = bluetex3_mat;
        asteroid_materials[3] = greentex1_mat;
        asteroid_materials[4] = greentex2_mat;
        asteroid_materials[5] = greentex3_mat;
        asteroid_materials[6] = redtex1_mat;
        asteroid_materials[7] = redtex2_mat;
        asteroid_materials[8] = redtex3_mat;
    }

    { // bullets
        red_bullet_img = GenImageColor(1, 1, RED);
        red_bullet_tex = LoadTextureFromImage(red_bullet_img);
        red_bullet_mat = LoadMaterialDefault();
        red_bullet_mat.maps[MATERIAL_MAP_DIFFUSE].texture = red_bullet_tex;
        red_bullet_msh = GenMeshSphere(.1, 7, 7);

        grn_bullet_img = GenImageColor(1, 1, GREEN);
        grn_bullet_tex = LoadTextureFromImage(grn_bullet_img);
        grn_bullet_mat = LoadMaterialDefault();
        grn_bullet_mat.maps[MATERIAL_MAP_DIFFUSE].texture = red_bullet_tex;
        grn_bullet_msh = GenMeshSphere(.1, 7, 7);

        blu_bullet_img = GenImageColor(1, 1, BLUE);
        blu_bullet_tex = LoadTextureFromImage(blu_bullet_img);
        blu_bullet_mat = LoadMaterialDefault();
        blu_bullet_mat.maps[MATERIAL_MAP_DIFFUSE].texture = blu_bullet_tex;
        blu_bullet_msh = GenMeshSphere(.1, 7, 7);
    }

    // music
    {
        cyber_spider_open_music = LoadMusicStreamFromMemory(".xm", cyber_spider_open_xm, cyber_spider_open_xm_len);
        cyber_spider_rest_music = LoadMusicStreamFromMemory(".xm", cyber_spider_rest_xm, cyber_spider_rest_xm_len);
    }

}

unsigned char * resource_load_file_callback(const char *actually_a_resource_load_stats, int *len){
    // recast to original type
    resource_load_stats * r = (resource_load_stats *)actually_a_resource_load_stats;
    *len = r->len;
    return (unsigned char *)r->data;
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