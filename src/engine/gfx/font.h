#ifndef VHM_FONT_H
#define VHM_FONT_H

#include <map>
#include "gfx.h"
#include "../utility/types.h"
#include "../utility/logging.h"
#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

extern FT_Library library;

namespace vhm
{

struct COLOR;

struct _KEY_COMPARATOR
{
    bool operator()(char const * a, char const * b) const
    {
        return strcmp(a, b) < 0;
    }
};

struct GLYPH
{
    u8* bitmap;
    char charcode;
    f32 width;
    f32 height;
    f32 advance;
    f32 bearingX;
    f32 bearingY;
};

struct FONT
{
    // map of glyph metrics+bitmap
    std::map<u32, GLYPH*> glyphs;

    // FreeType
    FT_Face ftFace;
    bool assigned;
    char* fontPath;

    // opengl
    bool assignedGL;
    GLuint textureHandleGL;
    
    // size specs
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
        std::map<char*, FONT*, _KEY_COMPARATOR> fonts;

        VAO    vao;
        GLuint program;
        char*  fontpath;

        u32 LoadFTFace(char* key, const char* filepath, u32 fontSize);
        u32 FreeFTFace(char* key);
        u32 LoadFontGlyphs(char* key);
        u32 FreeFontGlyphs(char* key);
        u32 GenerateFontTextures(char* key);
        u32 FreeFontTextures(char* key);

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
        u32 LoadNewFace(char* key, const char* filepath, int fontSize);
        /**
         * Derives the same face at a different size & creates the new font glyphs.
         */
        u32 DeriveFace(char* key, u32 newFontSize);

        u32 RenderText(char* key, const char* text, i32 screenX, i32 screenY, COLOR color);
        /**
         * Gauges the width of a string of text using a font in pixels.
         * @returns -1 if the font doesn't exist.
         */
        f32 TextWidth(char* key, const char* text);
};

}

#endif // VHM_FONT_H