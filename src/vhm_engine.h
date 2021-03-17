#ifndef VHM_WINDOW_H
#define VHM_WINDOW_H

#include "vhm_log.h"

#define VHM_SUCCESS 0
#define VHM_GENERAL_ERROR 1

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_TITLE "Voxelmade"

struct GLFWwindow;

/**
 * Represents the engine window
 */
struct VHM_WINDOW
{
    /// The GLFW handle linked to the display UI
    GLFWwindow* handle;
    /// The current FPS
    unsigned int fps;
};

extern VHM_WINDOW* window;

/**
 * @return whether the engine initialised correctly
 */
int vhm_InitEngine();

/**
 * Starts the engine loop
 * @param update update callback
 * @param draw draw callback
 */
void vhm_StartEngine(void (*update)(double, double), void (*draw)());

/**
 * Frees resources held by the engine
 */
void vhm_CleanEngine();

#endif // VHM_WINDOW_H