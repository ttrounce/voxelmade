#ifndef VHM_CAMERA_H
#define VHM_CAMERA_H

#include "transform.h"
#include "../utility/types.h"

namespace vhm
{

struct PERSPECTIVE_CAMERA
{
    mat4 persMatrix;
    float aspect, fov, near, far;
    TRANSFORM transform;

    PERSPECTIVE_CAMERA() {}
    PERSPECTIVE_CAMERA(f32 aspect, f32 fov, f32 near, f32 far)
    {
        this->aspect = aspect;
        this->fov = fov;
        this->near = near;
        this->far = far;
    }
};

void UpdatePerspectiveCamera(PERSPECTIVE_CAMERA& camera);
void UniformPerspectiveCamera(u32 program, const char* view, const char* proj, PERSPECTIVE_CAMERA camera);
void PollCameraInputs(PERSPECTIVE_CAMERA& camera);

}

#endif // VHM_CAMERA_H