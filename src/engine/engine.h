#ifndef VHM_WINDOW_H
#define VHM_WINDOW_H

#include "utility/types.h"
#include "utility/logging.h"

#define VHM_SUCCESS 0
#define VHM_GENERAL_ERROR 1

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_TITLE "Voxelmade"

struct GLFWwindow;

namespace vhm
{
    
struct MOUSE
{
    bool buttons[7];
    vec2 lastPos;
    vec2 thisPos;
    vec2 delta;
};

struct KEYBOARD
{
    bool keys[348];
    u32 lastPressed;
};

/**
 * Represents the engine window
 */
struct WINDOW
{
    /// The GLFW handle linked to the display UI
    GLFWwindow* handle;
    /// The Keyboard
    KEYBOARD* keyboard;
    // The Mouse
    MOUSE* mouse;
    /// The current FPS
    unsigned int fps;
};

}

extern vhm::WINDOW* window;

namespace vhm
{

/**
 * @return whether the engine initialised correctly
 */
int InitEngine();

/**
 * Starts the engine loop
 * @param update update callback
 * @param draw draw callback
 */
void StartEngine(void (*update)(double, double), void (*draw)());

/**
 * Frees resources held by the engine
 */
void CleanEngine();

/**
 * @return window width
 */
i32 GetWindowWidth();

/**
 * @return window height
 */
i32 GetWindowHeight();

/**
 * @return window aspect ratio
 */
f32 GetAspectRatio();
    
}

#endif // VHM_WINDOW_H