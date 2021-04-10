#ifndef VHM_CAMERA_H
#define VHM_CAMERA_H

#include "transform.h"
#include "utility/types.h"
#include <string>

namespace vhm
{

struct PERSPECTIVE_CAMERA
{
    glm::mat4 persMatrix;
    float aspect, fov, near, far;
    TRANSFORM transform;

    PERSPECTIVE_CAMERA() {}
    PERSPECTIVE_CAMERA(float aspect, float fov, float near, float far)
    {
        this->aspect = aspect;
        this->fov = fov;
        this->near = near;
        this->far = far;
    }
};

void UpdatePerspectiveCamera(PERSPECTIVE_CAMERA& camera);
void UniformPerspectiveCamera(uint program, std::string view, std::string proj, PERSPECTIVE_CAMERA camera);
void PollCameraInputs(PERSPECTIVE_CAMERA& camera);

}

#endif // VHM_CAMERA_H