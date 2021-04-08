#include "camera.h"
#include "gfx/gfx.h"
#include "engine.h"

#include <glfw/glfw3.h>

#define MOUSE_SENSITIVITY 0.1
#define BASE_MOVEMENT_SPEED 0.5

using namespace vhm;

void vhm::UpdatePerspectiveCamera(PERSPECTIVE_CAMERA& camera)
{
    glm_perspective(glm_rad(camera.fov), camera.aspect, camera.near, camera.far, camera.persMatrix);
    UpdateTransform(camera.transform, true);
}

void vhm::UniformPerspectiveCamera(u32 program, const char* view, const char* proj, PERSPECTIVE_CAMERA camera)
{
    UniformMat4(program, proj, camera.persMatrix);
    UniformMat4(program, view, camera.transform.mat);
}

void vhm::PollCameraInputs(PERSPECTIVE_CAMERA& camera)
{
    camera.transform.rot[1] += MOUSE_SENSITIVITY * window->mouse->delta[0];
    camera.transform.rot[0] += MOUSE_SENSITIVITY * window->mouse->delta[1];
    
    camera.transform.rot[0] = glm_clamp(camera.transform.rot[0], -90, 90);
    
    float xmove = -sin(camera.transform.rot[1] * M_PI/180.0) * BASE_MOVEMENT_SPEED;
    float zmove = cos(camera.transform.rot[1] * M_PI/180.0) * BASE_MOVEMENT_SPEED;

    float xmove_sideways = -sin((-90 + camera.transform.rot[1]) * M_PI/180.0) * BASE_MOVEMENT_SPEED;
    float zmove_sideways = cos((-90 + camera.transform.rot[1]) * M_PI/180.0) * BASE_MOVEMENT_SPEED;

    if(window->keyboard->keys[GLFW_KEY_SPACE])
    {
        camera.transform.pos[1] += BASE_MOVEMENT_SPEED;
    } else if (window->keyboard->keys[GLFW_KEY_LEFT_SHIFT])
    {
        camera.transform.pos[1] -= BASE_MOVEMENT_SPEED;
    }
    if(window->keyboard->keys[GLFW_KEY_D])
    {
        camera.transform.pos[0] += xmove_sideways;
        camera.transform.pos[2] += zmove_sideways;
    } else if (window->keyboard->keys[GLFW_KEY_A])
    {
        camera.transform.pos[0] -= xmove_sideways;
        camera.transform.pos[2] -= zmove_sideways;
    }
    if(window->keyboard->keys[GLFW_KEY_S])
    {
        camera.transform.pos[0] += xmove;
        camera.transform.pos[2] += zmove;
    } else if (window->keyboard->keys[GLFW_KEY_W])
    {
        camera.transform.pos[0] -= xmove;
        camera.transform.pos[2] -= zmove;
    }
}