#ifndef VHM_TRANSFORM_H
#define VHM_TRANSFORM_H

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace vhm
{

struct TRANSFORM
{
    glm::mat4 mat;
    glm::vec3 pos;
    glm::vec3 rot;

    TRANSFORM()
    {
        mat = glm::mat4(1.0f);
        pos = glm::vec3(0, 0, 0);
        rot = glm::vec3(0, 0, 0);
    }
};

void UpdateTransform(TRANSFORM& tf, bool negate);
}

#endif // VHM_TRANSFORM_H