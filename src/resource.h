
#ifndef AA_RESOURCE_H
#define AA_RESOURCE_H

#include "raylib.h"

void resource_init();
void resource_quit();

extern Mesh globe_mesh;
extern Image globe_img;
extern Texture globe_tex;
extern Material globe_mat;

#endif