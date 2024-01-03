
#ifndef AA_RESOURCE_H
#define AA_RESOURCE_H

#include "raylib.h"
#include <stdint.h>

typedef struct {
    char filename[100];
    const uint8_t * data;
    unsigned int len;
} resource_load_stats;

typedef struct {
    char * product_name;
    char * copyright_holder;
    char * description;
    char * link;
    char * license;
    char * payload;
    unsigned char payload_len;
} License;

unsigned char * resource_load_file_callback(const char *actually_a_resource_load_stats, int *len);

void resource_init();
void resource_set_sound_volumes();
void resource_quit();

extern Mesh globe_mesh;
extern Image globe_img;
extern Texture globe_tex;
extern Material globe_mat;

extern Model skybox_model;

extern Mesh atmosphere_mesh;
extern Image atmosphere_img;
extern Texture atmosphere_tex;
extern Material atmosphere_mat;

extern Image    bluetex1_img;
extern Texture  bluetex1_tex;
extern Material bluetex1_mat;

extern Image    bluetex2_img;
extern Texture  bluetex2_tex;
extern Material bluetex2_mat;

extern Image    bluetex3_img;
extern Texture  bluetex3_tex;
extern Material bluetex3_mat;

extern Image    greentex1_img;
extern Texture  greentex1_tex;
extern Material greentex1_mat;

extern Image    greentex2_img;
extern Texture  greentex2_tex;
extern Material greentex2_mat;

extern Image    greentex3_img;
extern Texture  greentex3_tex;
extern Material greentex3_mat;

extern Image    redtex1_img;
extern Texture  redtex1_tex;
extern Material redtex1_mat;

extern Image    redtex2_img;
extern Texture  redtex2_tex;
extern Material redtex2_mat;

extern Image    redtex3_img;
extern Texture  redtex3_tex;
extern Material redtex3_mat;

extern Image black_img;
extern Texture black_tex;

extern Mesh player_mesh;
extern Image player_img;
extern Texture player_tex;
extern Material player_mat;

extern Model asteroid_b000_model;
extern Model asteroid_b001_model;
extern Model asteroid_b002_model;
extern Model asteroid_b003_model;
extern Model asteroid_b004_model;
extern Model asteroid_b005_model;
extern Model asteroid_b006_model;
extern Model asteroid_b007_model;
extern Model asteroid_b008_model;
extern Model asteroid_b009_model;

extern Model asteroid_m000_model;
extern Model asteroid_m001_model;
extern Model asteroid_m002_model;
extern Model asteroid_m003_model;
extern Model asteroid_m004_model;
extern Model asteroid_m005_model;
extern Model asteroid_m006_model;
extern Model asteroid_m007_model;
extern Model asteroid_m008_model;
extern Model asteroid_m009_model;

extern Model asteroid_s000_model;
extern Model asteroid_s001_model;
extern Model asteroid_s002_model;
extern Model asteroid_s003_model;
extern Model asteroid_s004_model;
extern Model asteroid_s005_model;
extern Model asteroid_s006_model;
extern Model asteroid_s007_model;
extern Model asteroid_s008_model;
extern Model asteroid_s009_model;

extern Model asteroid_models[30];
extern Material asteroid_materials[9];

extern Model ship_model;

extern Image    red_bullet_img;
extern Texture  red_bullet_tex;
extern Material red_bullet_mat;
extern Mesh     red_bullet_msh;

extern Image    grn_bullet_img;
extern Texture  grn_bullet_tex;
extern Material grn_bullet_mat;
extern Mesh     grn_bullet_msh;

extern Image    blu_bullet_img;
extern Texture  blu_bullet_tex;
extern Material blu_bullet_mat;
extern Mesh     blu_bullet_msh;

extern Music cyber_spider_open_music;
extern Music cyber_spider_rest_music;

extern Sound asteroid_explosion1_snd;
extern Sound asteroid_explosion2_snd;
extern Sound asteroid_explosion3_snd;
extern Sound asteroid_explosion4_snd;
extern Sound asteroid_explosion5_snd;

extern Sound bullet_hit_r_snd;
extern Sound bullet_hit_g_snd;
extern Sound bullet_hit_b_snd;

extern Sound level_1_snd;
extern Sound level_2_snd;
extern Sound level_3_snd;
extern Sound level_4_snd;
extern Sound level_5_snd;
extern Sound level_engage_snd;
extern Sound level_infinite_mode_snd;

extern Sound fast_clear_bonus_snd;
extern Sound multiplier_pu_snd;
extern Sound powerup_snd;
extern Sound sheild_pu_snd;
extern Sound weapon_pu_snd;

// hack to auto loop the accel noise
extern Music accel_music;

extern Sound bullet_shoot_r_snd;
extern Sound bullet_shoot_g_snd;
extern Sound bullet_shoot_b_snd;
extern Sound player_hit_snd;

extern License licenses[7];

#endif
