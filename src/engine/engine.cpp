#include "engine.h"
#include "utility/types.h"

#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#define HARD_FPS_CAP 400

using namespace vhm;

WINDOW* window;

void MousePosCallback(GLFWwindow* hdl, f64 xpos, f64 ypos)
{
    if(window->handle == hdl)
    {
        vec2 position = {(f32) xpos, (f32) ypos};
        glm_vec2_copy(position, window->mouse->thisPos);
    }
}

void MouseButtonCallback(GLFWwindow* hdl, i32 key, i32 action, i32 mods)
{
    if(window->handle == hdl)
    {
        if(action == GLFW_PRESS)
        {
            window->mouse->buttons[key] = true;
        }
        if(action == GLFW_RELEASE)
        {
            window->mouse->buttons[key] = false;
        }
    }
}

void KeyCallback(GLFWwindow* hdl, i32 key, i32 scancode, i32 action, i32 mods)
{
    if(window->handle == hdl)
    {
        if(action == GLFW_PRESS)
        {
            window->keyboard->keys[key] = true;
        }
        if(action == GLFW_RELEASE)
        {
            window->keyboard->keys[key] = false;
        }
    }
}

void ResizeCallback(GLFWwindow* hdl, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

int vhm::InitEngine()
{
    if(glfwInit())
    {
        window = new WINDOW;
        if(window)
        {
            // Creating GLFW window
            window->handle = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE, NULL, NULL);
            if(window->handle)
            {
                // Loading OpenGL
                glfwMakeContextCurrent(window->handle);
                if(gladLoadGL())
                {  
                    window->keyboard = new KEYBOARD;
                    window->mouse = new MOUSE;
                    
                    for(int i = 0; i < 7; i++)
                    {
                        window->mouse->buttons[i] = false;
                    }
                    for(int i = 0; i < 348; i++)
                    {
                        window->keyboard->keys[i] = false;
                    }

                    glfwSetCursorPosCallback(window->handle, MousePosCallback);
                    glfwSetMouseButtonCallback(window->handle, MouseButtonCallback);
                    glfwSetKeyCallback(window->handle, KeyCallback);
                    glfwSetWindowSizeCallback(window->handle, ResizeCallback);
                    return VHM_SUCCESS;
                }
                else printf("%s Could not load GL\n", VHM_GLAD_ERR);
            }
            else printf("%s Could not create GLFW window\n", VHM_GLFW_ERR);
        }
        else printf("%s Could not allocate window\n", VHM_ENGINE_ERR);
    }
    else printf("%s Could not initialise GLFW\n", VHM_GLFW_ERR);
    return VHM_GENERAL_ERROR;
}

void vhm::StartEngine(void (*update)(double, double), void (*draw)())
{
    window->fps = HARD_FPS_CAP;

    const double dt = 1.0/60.0;
    double updateAccumulator = 0.0;
    double frameCountAccumulator = 0.0;
    
    double time = 0.0;
    
    clock_t startTime = clock();

    double frameTime = 0.0;
    int frameCount = 0;        
    while(!glfwWindowShouldClose(window->handle))
    {
        clock_t endTime = clock();
        frameTime = 1.0*(endTime - startTime)/CLOCKS_PER_SEC;
        startTime = endTime;
        
        updateAccumulator += frameTime;
        frameCountAccumulator += frameTime;
        
        if(updateAccumulator >= dt)
        {
            updateAccumulator -= dt;
            time += dt;

            // mouse delta calculations
            glm_vec2_sub(window->mouse->thisPos, window->mouse->lastPos, window->mouse->delta);
            glm_vec2_clamp(window->mouse->delta, -128, 128);

            update(time, dt);

            glm_vec2_copy(window->mouse->thisPos, window->mouse->lastPos);
        }
        
        if(frameCountAccumulator >= 1.0)
        {
            window->fps = glm_clamp(frameCount, 0, HARD_FPS_CAP);
            frameCount = 0;
            frameCountAccumulator -= 1;
        }
        
        frameCount++;
        draw();
        glfwSwapBuffers(window->handle);
        glfwPollEvents();   
    }
}

void vhm::CleanEngine()
{
    delete window->keyboard;
    delete window->mouse;
    delete window;
    glfwTerminate();
}

i32 vhm::GetWindowWidth()
{
    i32 width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return width;
}

i32 vhm::GetWindowHeight()
{
    i32 width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return height;
}

f32 vhm::GetAspectRatio()
{
    i32 width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return (float) width / (float) height;
}