#ifndef VHM_GLM_H
#define VHM_GLM_H

#include <cglm/cglm.h>

namespace vhm
{

inline static void glm_Vec4Set(float x, float y, float z, float w, vec3 dest)
{
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    dest[3] = w;
}

inline static void glm_Vec3Set(float x, float y, float z, vec3 dest)
{
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
}

inline static void glm_Vec2Set(float x, float y, vec2 dest)
{
    dest[0] = x;
    dest[1] = y;
}


}

#endif // VHM_GLM_H