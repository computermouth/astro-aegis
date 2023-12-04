
#include "tool.h"

const Vector3 WORLD_POS = { 100.0f, 100.0f, 100.0f };

Vector3 tool_vec3_world_pos(Vector3 v){
    return (Vector3){.x = v.x + WORLD_POS.x, .y = v.y + WORLD_POS.y, .z = v.z + WORLD_POS.z};
}