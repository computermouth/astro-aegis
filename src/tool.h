
#ifndef AA_TOOL_H
#define AA_TOOL_H

#include "raylib.h"
#include "raymath.h"

typedef struct {
    Vector3 center;
    float radius;
} ToolBoundingSphere;

float tool_get_bounding_sphere_radius(BoundingBox bb);
Vector3 tool_vec3_world_pos(Vector3);

#endif