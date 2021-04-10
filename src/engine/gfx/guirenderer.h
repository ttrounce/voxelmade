#ifndef VHM_GUI_H
#define VHM_GUI_H

#include "gfx.h"
#include "../utility/types.h"

namespace vhm
{

struct COLOR;

class GUI_RENDERER
{
    private:
        static GUI_RENDERER* instance;
        VAO vao;
        GLuint program;

        GUI_RENDERER();
        ~GUI_RENDERER();
    public:
        static GUI_RENDERER* GetInstance()
        {
            static GUI_RENDERER instance;
            return &instance;
        }
        void FillRect(int screenX, int screenY, int width, int height, COLOR color);
        void TextureRect(int screenX, int screenY, int width, int height, GLuint textureHandleGL);
};

}

#endif // VHM_GUI_H