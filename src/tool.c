
#include "tool.h"

const Vector3 WORLD_POS = { 100.0f, 100.0f, 100.0f };

Vector3 tool_vec3_world_pos(Vector3 v){
    return (Vector3){.x = v.x + WORLD_POS.x, .y = v.y + WORLD_POS.y, .z = v.z + WORLD_POS.z};
}

float tool_get_bounding_sphere_radius(BoundingBox bb){
    // Calculate the radius of the sphere
    Vector3 half_extent = Vector3Subtract(bb.max, bb.min);
    return Vector3Length(half_extent) / 2.0f;
}