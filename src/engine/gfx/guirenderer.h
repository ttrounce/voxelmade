#ifndef VHM_GUI_H
#define VHM_GUI_H

#include "gfx.h"
#include "../utility/types.h"
#include <memory>

namespace vhm
{

struct COLOR;

class GUI_RENDERER
{
    private:
        VAO vao;
        GLuint program;
    public:
        GUI_RENDERER();
        ~GUI_RENDERER();
        void FillRect(int screenX, int screenY, int width, int height, COLOR color);
        void TextureRect(int screenX, int screenY, int width, int height, GLuint textureHandleGL);
};

extern std::unique_ptr<GUI_RENDERER> guiRenderer;

}

#endif // VHM_GUI_H