#include "vhm_engine.h"
#include "vhm_memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

VHM_WINDOW* window;

int vhm_InitEngine()
{
    if(glfwInit())
    {
        window = vhm_mnew(VHM_WINDOW);
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
                    return VHM_SUCCESS;
                }
                else printf("%s Could not load GL\n", VHM_GLAD_LOG);
            }
            else printf("%s Could not create GLFW window\n", VHM_GLFW_LOG);
        }
        else printf("%s Could not allocate window\n", VHM_ENGINE_LOG);
    }
    else printf("%s Could not initialise GLFW\n", VHM_GLFW_LOG);
    return VHM_GENERAL_ERROR;
}

void vhm_StartEngine(void (*update)(double, double), void (*draw)())
{
    const double dt = 1.0/60.0;
    double updateAccumulator = 0.0;
    double frameCountAccumulator = 0.0;
    
    double time = 0.0;
    
    clock_t startTime = clock();
    
    double frameTime;
    
    int frameCount;        
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

            update(time, dt);
        }
        
        if(frameCountAccumulator >= 1.0)
        {
            window->fps = frameCount;
            frameCount = 0;
            frameCountAccumulator -= 1;
        }
        
        frameCount++;
        draw();
        glfwSwapBuffers(window->handle);
        glfwPollEvents();   
    }
}

void vhm_CleanEngine()
{
    vhm_mfree(window);
    glfwTerminate();
}