
#ifndef AA_ENTITY_H
#define AA_ENTITY_H

#include "raylib.h"

typedef struct {
    Mesh mesh;
    Material material;
    Matrix translation_matrix;
    Matrix transform_matrix;
    Vector3 position;
    Vector3 speed;
} Entity;

#endif