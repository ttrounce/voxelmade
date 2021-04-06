#ifndef VHM_FONT_H
#define VHM_FONT_H


#include <map>
#include "gfx.h"
#include "types.h"
#include "logging.h"

#include <ft2build.h>
#include FT_FREETYPE_H

extern FT_Library library;

namespace vhm
{

struct GLYPH
{
    unsigned char* bitmap;
    i32 width;
    i32 height;
    i32 advance;
    i32 bearingX;
    i32 bearingY;
};

struct FONT
{
    std::map<i32, GLYPH*> glyphs;

    bool assignedGL;
    u32 textureHandleGL;
    i32 maxWidth;
    i32 maxHeight;
    i32 capitalBearingY;
};

void InitFreeType();
void FreeFreeType();

class FONT_RENDERER
{
    static FONT_RENDERER* instance;

    private:
        FT_Face face;
        bool faceAssigned;
        FONT    font;
        VAO     vao;
        u32     program;
        char* fontpath;

        void LoadFTFace(const char* filepath, int fontSize);
        void FreeFTFace();
        void LoadFontGlyphs();
        void FreeFontGlyphs();
        void GenerateFontTextures();

        FONT_RENDERER();
        ~FONT_RENDERER();
    public:
        static FONT_RENDERER* GetInstance()
        {
            static FONT_RENDERER instance;
            return &instance;
        }
        /**
         * Loads a new fontface to the renderer at a certain size & creates the font glyphs.
         */
        void LoadNewFace(const char* filepath, int fontSize);
        /**
         * Derives the same face at a different size & creates the new font glyphs.
         */
        void DeriveFace(int newFontSize);

        void RenderText(const char* text, int screenX, int screenY, f32* color);
        f32 TextLength(const char* text);
};

}

#endif // VHM_FONT_H