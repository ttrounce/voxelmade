#include "camera.h"
#include "gfx/gfx.h"
#include "engine.h"

#include <glfw/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

#define MOUSE_SENSITIVITY 0.1
#define BASE_MOVEMENT_SPEED 0.5

using namespace vhm;

void vhm::UpdatePerspectiveCamera(PERSPECTIVE_CAMERA& camera)
{
    camera.persMatrix = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.near, camera.far);
    UpdateTransform(camera.transform, true);
}

void vhm::UniformPerspectiveCamera(uint program, std::string view, std::string proj, PERSPECTIVE_CAMERA camera)
{
    UniformMat4(program, proj, camera.persMatrix);
    UniformMat4(program, view, camera.transform.mat);
}

void vhm::PollCameraInputs(PERSPECTIVE_CAMERA& camera)
{
    camera.transform.rot = glm::vec3(window->mouse->delta, 0.0) * (float) MOUSE_SENSITIVITY;
    camera.transform.rot.x = glm::clamp(camera.transform.rot.x, -90.0f, 90.0f);
    
    float xmove = -sin(camera.transform.rot.y * glm::pi<float>()/180.0) * BASE_MOVEMENT_SPEED;
    float zmove = cos(camera.transform.rot.y * glm::pi<float>()/180.0) * BASE_MOVEMENT_SPEED;

    float xmove_sideways = -sin((-90 + camera.transform.rot.y) * glm::pi<float>()/180.0) * BASE_MOVEMENT_SPEED;
    float zmove_sideways = cos((-90 + camera.transform.rot.y) * glm::pi<float>()/180.0) * BASE_MOVEMENT_SPEED;

    if(window->keyboard->keys[GLFW_KEY_SPACE])
    {
        camera.transform.pos.y += BASE_MOVEMENT_SPEED;
    } else if (window->keyboard->keys[GLFW_KEY_LEFT_SHIFT])
    {
        camera.transform.pos.y -= BASE_MOVEMENT_SPEED;
    }
    if(window->keyboard->keys[GLFW_KEY_D])
    {
        camera.transform.pos.x += xmove_sideways;
        camera.transform.pos.z += zmove_sideways;
    } else if (window->keyboard->keys[GLFW_KEY_A])
    {
        camera.transform.pos.x -= xmove_sideways;
        camera.transform.pos.z -= zmove_sideways;
    }
    if(window->keyboard->keys[GLFW_KEY_S])
    {
        camera.transform.pos.x += xmove;
        camera.transform.pos.z += zmove;
    } else if (window->keyboard->keys[GLFW_KEY_W])
    {
        camera.transform.pos.x -= xmove;
        camera.transform.pos.z -= zmove;
    }
}