#include "engine.h"
#include "utility/types.h"

#include <glm/ext/scalar_common.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#define HARD_FPS_CAP 400

using namespace vhm;

std::unique_ptr<WINDOW> window;

void MousePosCallback(GLFWwindow* hdl, double xpos, double ypos)
{
    if(window->handle == hdl)
    {
        window->mouse->thisPos = glm::vec2((float) xpos, (float) ypos);
    }
}

void MouseButtonCallback(GLFWwindow* hdl, int key, int action, int mods)
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

void KeyCallback(GLFWwindow* hdl, int key, int scancode, int action, int mods)
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

void ResizeCallback(GLFWwindow* hdl, int width, int height)
{
    glViewport(0, 0, width, height);
}

int vhm::InitEngine()
{
    if(glfwInit())
    {
        window = std::make_unique<WINDOW>();
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
                    window->keyboard = std::make_unique<KEYBOARD>();
                    window->mouse = std::make_unique<MOUSE>();
                    
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
            window->mouse->delta = glm::clamp(window->mouse->thisPos - window->mouse->lastPos, glm::vec2(-128.0f), glm::vec2(128.0f));

            update(time, dt);

            window->mouse->lastPos = window->mouse->thisPos;
        }
        
        if(frameCountAccumulator >= 1.0)
        {
            window->fps = glm::clamp(frameCount, 0, HARD_FPS_CAP);
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
    window.reset();
    glfwTerminate();
}

int vhm::GetWindowWidth()
{
    int width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return width;
}

int vhm::GetWindowHeight()
{
    int width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return height;
}

float vhm::GetAspectRatio()
{
    int width, height;
    glfwGetWindowSize(window->handle, &width, &height);
    return (float) width / (float) height;
}