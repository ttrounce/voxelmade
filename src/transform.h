#ifndef VHM_TRANSFORM_H
#define VHM_TRANSFORM_H

#include "glm.h"

namespace vhm
{

struct TRANSFORM
{
    mat4 mat;
    vec3 pos;
    vec3 rot;

    TRANSFORM()
    {
        glm_mat4_identity(mat);
        vhm::glm_Vec3Set(0.0, 0.0, 0.0, pos);
        vhm::glm_Vec3Set(0.0, 0.0, 0.0, rot);
    }
};

void UpdateTransform(TRANSFORM& tf, bool negate);
}

#endif // VHM_TRANSFORM_H