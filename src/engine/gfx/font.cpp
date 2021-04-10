#include "../utility/logging.h"
#include "../utility/basicio.h"
#include "../utility/color.h"
#include "../engine.h"
#include "font.h"
#include "gfx.h"

#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace vhm;

FT_Library library;

std::unique_ptr<FONT_RENDERER> vhm::fontRenderer;

void vhm::InitFreeType()
{
    int error = FT_Init_FreeType(&library);
    if(error)
    {
        printf("%s Error initialising FreeType\n", VHM_FT_ERR);
    }
    else
    {
        printf("%s Successfully initialised FreeType\n", VHM_FT_LOG);
    }
    fontRenderer = std::make_unique<FONT_RENDERER>();
}

void vhm::FreeFreeType()
{
    FT_Done_FreeType(library);
}

FONT_RENDERER::FONT_RENDERER()
{
    // Generate VAO model
    float vertices[12] = {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    float texels[12] = {0.0, 0.0, 0.99, 0.99, 0.99, 0.0, 0.0, 0.0, 0.0, 0.99, 0.99, 0.99};

    glGenVertexArrays(1, &vao.handle);
    glBindVertexArray(vao.handle);
    SetBufferVAO(vao, 0, 2, vertices, GL_FLOAT, 12);
    SetBufferVAO(vao, 1, 2, texels, GL_FLOAT, 12);
    glBindVertexArray(0);

    // Create Shader Program
    std::string vert = ReadString("shaders/font_vert.glsl");
    std::string frag = ReadString("shaders/font_frag.glsl");

    program = glCreateProgram();
    glUseProgram(program);
    CreateVertexShader(program, vert, vert.length());
    CreateFragmentShader(program, frag, frag.length());
    LinkProgram(program);
    glUseProgram(0);
}

FONT_RENDERER::~FONT_RENDERER()
{
    glDeleteProgram(program);

    fonts.clear();
    
    glDeleteVertexArrays(1, &vao.handle);
    glDeleteBuffers(vao.vbos.size(), &vao.vbos[0]);
}
    
uint FONT_RENDERER::LoadFTFace(std::string key, std::string filepath, uint fontSize)
{
    if(fonts.count(key) > 0)
    {
        printf("%s Font %s already present\n", VHM_FT_ERR, key.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<FONT> font = std::make_shared<FONT>();
    int error = FT_New_Face(library, filepath.c_str(), 0, &font->ftFace);
    if(error > 0)
    {
        printf("%s %d %s", VHM_FT_ERR, error, FT_Error_String(error));
    }    
    font->fontPath = filepath;
    FT_Set_Pixel_Sizes(font->ftFace, 0, fontSize);
    font->assigned = true;
    fonts.insert(std::make_pair(key, font));
    return VHM_SUCCESS;
}


uint FONT_RENDERER::LoadFontGlyphs(std::string key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to load glyphs of a font (%s) that doesn't exist\n", VHM_FT_ERR, key.c_str());
        return VHM_ERROR;
    }

    std::shared_ptr<FONT> font = fonts.at(key);
    font->maxWidth = 0;
    font->maxHeight = 0;
    font->capitalBearingY = 0;

    uint gindex;
    ulong charcode;

    charcode = FT_Get_First_Char(font->ftFace, &gindex);
    while(gindex != 0)
    {
        uint error = FT_Load_Char(font->ftFace, charcode, FT_LOAD_RENDER);
        if(error)
        {
            printf("%s Error loading character %d\n", VHM_FT_ERR, gindex);
        }
        if(font->ftFace->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
        {
            printf("%s Error where character %c is the incorrect pixel mode (%u)\n", VHM_FT_LOG, (int) charcode, font->ftFace->glyph->bitmap.pixel_mode);
        }

        GLYPH* glyph = new GLYPH;
        glyph->bitmap = (uchar*) malloc(sizeof(uchar) * font->ftFace->glyph->bitmap.width * font->ftFace->glyph->bitmap.rows);
        memcpy(glyph->bitmap, font->ftFace->glyph->bitmap.buffer, sizeof(unsigned char) * font->ftFace->glyph->bitmap.width * font->ftFace->glyph->bitmap.rows);
        glyph->width = font->ftFace->glyph->bitmap.width;
        glyph->height = font->ftFace->glyph->bitmap.rows;
        glyph->advance = font->ftFace->glyph->advance.x / 64.0;
        glyph->bearingX = font->ftFace->glyph->metrics.horiBearingX / 64.0;
        glyph->bearingY = font->ftFace->glyph->metrics.horiBearingY / 64.0;
        glyph->charcode = charcode;
        font->glyphs.insert(std::make_pair(gindex, glyph));

        if(charcode == '^')
        {
            font->capitalBearingY = glyph->bearingY;
        }
        if((int) font->ftFace->glyph->bitmap.width > font->maxWidth)
        {
            font->maxWidth = font->ftFace->glyph->bitmap.width;
        }
        if((int) font->ftFace->glyph->bitmap.rows > font->maxHeight)
        {
            font->maxHeight = font->ftFace->glyph->bitmap.rows;
        }
        charcode = FT_Get_Next_Char(font->ftFace, charcode, &gindex);
    }
    return VHM_SUCCESS;
}

uint FONT_RENDERER::GenerateFontTextures(std::string key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to create glyph textures of a font (%s) that doesn't exist\n", VHM_FT_ERR, key.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<FONT> font = fonts.at(key);
    // Delete the previous texture if it exists.
    if(font->assignedGL)
    {
        glDeleteTextures(1, &font->textureHandleGL);
    }

    // Generates a texture and stores the bitmaps in an OpenGL Texture Array
    glGenTextures(1, &font->textureHandleGL);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font->textureHandleGL);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    SetMinMag(GL_TEXTURE_2D_ARRAY, GL_LINEAR, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
    SetTextureArrayStorage(font->textureHandleGL, GL_R8, font->maxWidth, font->maxHeight, font->glyphs.size());
    for(std::map<uint, GLYPH*>::iterator it = font->glyphs.begin(); it != font->glyphs.end(); it++)
    {
        LoadTextureArrayLayer(font->textureHandleGL, it->second->bitmap, 0, 0, it->second->width, it->second->height, it->first, GL_RED);
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    font->assignedGL = true;
    return VHM_SUCCESS;
}

uint FONT_RENDERER::LoadFace(std::string key, std::string filepath, int fontSize)
{
    if(LoadFTFace(key, filepath, fontSize) == VHM_SUCCESS)
    {
        if(LoadFontGlyphs(key) == VHM_SUCCESS)
        {
            if(GenerateFontTextures(key) == VHM_SUCCESS)
            {
                return VHM_SUCCESS;
            }
        }
    }
    return VHM_ERROR;
}

uint FONT_RENDERER::DeriveFace(std::string key, uint newFontSize)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to derive from a font (%s) that doesn't exist\n", VHM_FT_ERR, key.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<FONT> font = fonts.at(key);

    fonts.erase(key);

    if(LoadFTFace(key, font->fontPath, newFontSize) == VHM_SUCCESS)
    {
        if(LoadFontGlyphs(key) == VHM_SUCCESS)
        {
            if(GenerateFontTextures(key) == VHM_SUCCESS)
            {
                return VHM_SUCCESS;
            }
        }
    }
    return VHM_ERROR;
}

uint FONT_RENDERER::CloneFace(std::string key, std::string newKey, uint newFontSize)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to derive from a font (%s) that doesn't exist\n", VHM_FT_ERR, key.c_str());
        return VHM_ERROR;
    }
    if(fonts.count(newKey) != 0)
    {
        printf("%s Unable to replace an existing font (%s) with a derived font\n", VHM_FT_ERR, newKey.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<FONT> font = fonts.at(key);
    if(LoadFTFace(newKey, font->fontPath, newFontSize) == VHM_SUCCESS)
    {
        if(LoadFontGlyphs(newKey) == VHM_SUCCESS)
        {
            if(GenerateFontTextures(newKey) == VHM_SUCCESS)
            {
                return VHM_SUCCESS;
            }
        }
    }
    return VHM_ERROR;
}

uint FONT_RENDERER::RenderText(std::string key, std::string text, int screenX, int screenY, COLOR color)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to render a font (%s) that doesn't exist\n", VHM_FT_ERR, key.c_str());
        return VHM_ERROR;
    }
    std::shared_ptr<FONT> font = fonts.at(key);
    // Binds
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font->textureHandleGL);

    // Color matrix uniform
    UniformVec3(program, "color", color.vec);

    // Orthographic matrix uniform
    glm::mat4 projection = glm::ortho<float>(0.0, GetWindowWidth(), GetWindowHeight(), 0.0, 0.0001, 10.0);
    UniformMat4(program, "projection", projection);

    float caret = 0.0;
    glm::vec3 scale((float) font->maxWidth, (float) font->maxHeight, 1.0);

    for(uint i = 0; i < text.length(); i++)
    {
        uint charInd = FT_Get_Char_Index(font->ftFace, text[i]);
        if(font->glyphs.find(charInd) == font->glyphs.end())
        {
            continue;
        }
        GLYPH ch = *font->glyphs.at(charInd);
        
        float layer = (float) (i+1) / (text.length() + 1);
        glUniform1f(glGetUniformLocation(program, "defaultLayerZ"), GFX_LAYER_GUI + layer);

        float hb = font->capitalBearingY - ch.bearingY;

        float advance = ch.advance;
        float bearingX = ch.bearingX;
        // Remove bearing from the first letter, so that it's aligned correctly with screenX.
        if(i == 0)
        {
            advance -= bearingX;
            bearingX = 0;
        }
        glm::vec3 translate((float) screenX + (caret + bearingX), (float) screenY + hb, 0.0);

        glm::mat4 transformation(1.0f);
        transformation = glm::translate(transformation, translate);
        transformation = glm::scale(transformation, scale);

        UniformMat4(program, "transformation", transformation);

        glUniform1i(glGetUniformLocation(program, "layer"), FT_Get_Char_Index(font->ftFace, text[i]));

        // Drawing
        glDrawArrays(GL_TRIANGLES, 0, 12);
        caret += advance;
    }

    // Unbinds
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    return VHM_SUCCESS;
}

float FONT_RENDERER::TextWidth(std::string key, std::string text)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to gauge the text width of a font (%s) that doesn't exist\n", VHM_FT_ERR, key.c_str());
        return -1;
    }
    std::shared_ptr<FONT> font = fonts.at(key);

    uint len = text.length();
    float width = 0;
    for(uint i = 0; i < len; i++)
    {
        uint charInd = FT_Get_Char_Index(font->ftFace, text[i]);
        if(font->glyphs.find(charInd) == font->glyphs.end())
        {
            continue;
        }
        GLYPH* ch = font->glyphs.at(charInd);
        
        if(i == len - 1)
        {
            width += ch->advance - ch->bearingX;
        }
        else
        {
            width += ch->advance;
        }
    }
    return width;
}