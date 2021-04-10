#ifndef VHM_FONT_H
#define VHM_FONT_H

#include <map>
#include "gfx.h"
#include "../utility/types.h"
#include "../utility/logging.h"
#include <glad/glad.h>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

extern FT_Library library;

namespace vhm
{

struct COLOR;

struct GLYPH
{
    uchar* bitmap;
    char charcode;
    float width;
    float height;
    float advance;
    float bearingX;
    float bearingY;
};

struct FONT
{
    // map of glyph metrics+bitmap
    std::map<uint, GLYPH*> glyphs;

    // FreeType
    FT_Face ftFace;
    bool assigned;
    std::string fontPath;

    // opengl
    bool assignedGL;
    GLuint textureHandleGL;
    
    // size specs
    int maxWidth;
    int maxHeight;
    int capitalBearingY;

    ~FONT()
    {
        FT_Done_Face(ftFace);
        for(auto it = glyphs.begin(); it != glyphs.end(); it++)
        {
            free(it->second->bitmap);
            delete it->second;
        }
        glyphs.clear();
        glDeleteTextures(1, &textureHandleGL);
    }
};

void InitFreeType();
void FreeFreeType();

class FONT_RENDERER
{

    private:
        std::map<std::string, std::shared_ptr<FONT>> fonts;

        VAO    vao;
        GLuint program;
        std::string  fontpath;

        uint LoadFTFace(std::string key, std::string filepath, uint fontSize);
        uint LoadFontGlyphs(std::string key);
        uint GenerateFontTextures(std::string key);
    public:
        FONT_RENDERER();
        ~FONT_RENDERER();
        /**
         * Loads a new fontface to the renderer at a certain size & creates the font glyphs.
         */
        uint LoadFace(std::string key, std::string filepath, int fontSize);
        /**
         * Derives the same face at a different size & creates the new font glyphs.
         */
        uint DeriveFace(std::string key, uint newFontSize);

        /**
         * Derives the same face at a different size & stores it as a new font.
         */
        uint CloneFace(std::string key, std::string newKey, uint newFontSize);

        uint RenderText(std::string key, std::string text, int screenX, int screenY, COLOR color);
        /**
         * Gauges the width of a string of text using a font in pixels.
         * @returns -1 if the font doesn't exist.
         */
        float TextWidth(std::string key, std::string text);
};

extern std::unique_ptr<FONT_RENDERER> fontRenderer;

}

#endif // VHM_FONT_H