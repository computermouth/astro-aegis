
#ifndef AA_RESOURCE_H
#define AA_RESOURCE_H

#include "raylib.h"

void resource_init();
void resource_quit();

extern Mesh globe_mesh;
extern Image globe_img;
extern Texture globe_tex;
extern Material globe_mat;

extern Mesh atmosphere_mesh;
extern Image atmosphere_img;
extern Texture atmosphere_tex;
extern Material atmosphere_mat;

extern Mesh player_mesh;
extern Image player_img;
extern Texture player_tex;
extern Material player_mat;

#endif