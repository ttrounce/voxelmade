#ifndef VHM_WINDOW_H
#define VHM_WINDOW_H

#include "utility/types.h"
#include "utility/logging.h"
#include <glm/ext/vector_float2.hpp>
#include <memory>

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
    glm::vec2 lastPos;
    glm::vec2 thisPos;
    glm::vec2 delta;
};

struct KEYBOARD
{
    bool keys[348];
    uint lastPressed;
};

/**
 * Represents the engine window
 */
struct WINDOW
{
    /// The GLFW handle linked to the display UI
    GLFWwindow* handle;
    /// The Keyboard
    std::unique_ptr<KEYBOARD> keyboard;
    // The Mouse
    std::unique_ptr<MOUSE> mouse;
    /// The current FPS
    unsigned int fps;
};

}

extern std::unique_ptr<vhm::WINDOW> window;

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
int GetWindowWidth();

/**
 * @return window height
 */
int GetWindowHeight();

/**
 * @return window aspect ratio
 */
float GetAspectRatio();
    
}

#endif // VHM_WINDOW_H