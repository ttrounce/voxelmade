#ifndef VHM_COLOR_H
#define VHM_COLOR_H

#include "types.h"
#include <glm/ext/vector_float3.hpp>

namespace vhm
{

struct COLOR
{
    int hex;
    glm::vec3 vec;

    COLOR(int rgb)
    {
        this->hex = rgb;
        this->vec.x = ((rgb & 0xFF0000) >> 16) / 255.0;
        this->vec.y = ((rgb & 0x00FF00) >> 8)  / 255.0;
        this->vec.z = ((rgb & 0x0000FF) >> 0)  / 255.0;
    }
};
   
}
#endif // VHM_COLOR_H
