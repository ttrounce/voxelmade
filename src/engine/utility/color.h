#ifndef VHM_COLOR_H
#define VHM_COLOR_H

#include "types.h"

namespace vhm
{

struct COLOR
{
    i32 hex;
    vec3 vec;

    COLOR(i32 rgb)
    {
        this->hex = rgb;
        this->vec[0] = ((rgb & 0xFF0000) >> 16) / 255.0;
        this->vec[1] = ((rgb & 0x00FF00) >> 8)  / 255.0;
        this->vec[2] = ((rgb & 0x0000FF) >> 0)  / 255.0;
    }
};
   
}
#endif // VHM_COLOR_H
