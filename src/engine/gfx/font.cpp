#include "../utility/logging.h"
#include "../utility/basicio.h"
#include "../utility/color.h"
#include "../engine.h"
#include "font.h"
#include "gfx.h"

#include <map>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace vhm;

FT_Library library;

void vhm::InitFreeType()
{
    i32 error = FT_Init_FreeType(&library);
    if(error)
    {
        printf("%s Error initialising FreeType\n", VHM_FT_ERR);
    }
    else
    {
        printf("%s Successfully initialised FreeType\n", VHM_FT_LOG);
    }
}

void vhm::FreeFreeType()
{
    FT_Done_FreeType(library);
}

FONT_RENDERER::FONT_RENDERER()
{
    // Generate VAO model
    f32 vertices[12] = {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    f32 texels[12] = {0.0, 0.0, 0.99, 0.99, 0.99, 0.0, 0.0, 0.0, 0.0, 0.99, 0.99, 0.99};

    glGenVertexArrays(1, &vao.handle);
    glBindVertexArray(vao.handle);
    SetBufferVAO(vao, 0, 2, vertices, GL_FLOAT, 12);
    SetBufferVAO(vao, 1, 2, texels, GL_FLOAT, 12);
    glBindVertexArray(0);

    // Create Shader Program
    char* vert = ReadString("shaders/font_vert.glsl");
    char* frag = ReadString("shaders/font_frag.glsl");

    program = glCreateProgram();
    glUseProgram(program);
    CreateVertexShader(program, vert, strlen(vert));
    CreateFragmentShader(program, frag, strlen(frag));
    LinkProgram(program);
    glUseProgram(0);

    free(vert);
    free(frag);
}

FONT_RENDERER::~FONT_RENDERER()
{
    free(fontpath);
    glDeleteProgram(program);

    for(auto it = fonts.begin(); it != fonts.end(); it++)
    {
        FreeFTFace(it->first);
        FreeFontGlyphs(it->first);
        FreeFontTextures(it->first);
        delete it->second;
    }
    fonts.clear();
    
    glDeleteVertexArrays(1, &vao.handle);
    glDeleteBuffers(vao.vbos.size(), &vao.vbos[0]);
}
    
u32 FONT_RENDERER::LoadFTFace(char* key, const char* filepath, u32 fontSize)
{
    if(fonts.count(key) > 0)
    {
        printf("%s Font %s already present\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = new FONT;
    i32 error = FT_New_Face(library, filepath, 0, &font->ftFace);
    if(error > 0)
    {
        printf("%s %d %s", VHM_FT_ERR, error, FT_Error_String(error));
    }    
    font->fontPath = (char*) malloc(strlen(filepath) + 1);
    strcpy(font->fontPath, filepath);

    FT_Set_Pixel_Sizes(font->ftFace, 0, fontSize);
    font->assigned = true;
    fonts.insert(std::make_pair(key, font));
    return VHM_SUCCESS;
}

u32 FONT_RENDERER::FreeFTFace(char* key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to free a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = fonts.at(key);
    free(font->fontPath);
    FT_Done_Face(font->ftFace);
    return VHM_SUCCESS;
}

u32 FONT_RENDERER::LoadFontGlyphs(char* key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to load glyphs of a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }

    FONT* font = fonts.at(key);
    font->maxWidth = 0;
    font->maxHeight = 0;
    font->capitalBearingY = 0;

    u32 gindex;
    u64 charcode;

    charcode = FT_Get_First_Char(font->ftFace, &gindex);
    while(gindex != 0)
    {
        u32 error = FT_Load_Char(font->ftFace, charcode, FT_LOAD_RENDER);
        if(error)
        {
            printf("%s Error loading character %d\n", VHM_FT_ERR, gindex);
        }
        if(font->ftFace->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
        {
            printf("%s Error where character %c is the incorrect pixel mode (%u)\n", VHM_FT_LOG, (i32) charcode, font->ftFace->glyph->bitmap.pixel_mode);
        }

        GLYPH* glyph = new GLYPH;
        glyph->bitmap = (u8*) malloc(sizeof(u8) * font->ftFace->glyph->bitmap.width * font->ftFace->glyph->bitmap.rows);
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
        if((i32) font->ftFace->glyph->bitmap.width > font->maxWidth)
        {
            font->maxWidth = font->ftFace->glyph->bitmap.width;
        }
        if((i32) font->ftFace->glyph->bitmap.rows > font->maxHeight)
        {
            font->maxHeight = font->ftFace->glyph->bitmap.rows;
        }
        charcode = FT_Get_Next_Char(font->ftFace, charcode, &gindex);
    }
    return VHM_SUCCESS;
}

u32 FONT_RENDERER::FreeFontGlyphs(char* key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to free glyphs of a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = fonts.at(key);
    for(auto it = font->glyphs.begin(); it != font->glyphs.end(); it++)
    {
        free(it->second->bitmap);
        delete it->second;
    }
    font->glyphs.clear();
    return VHM_SUCCESS;
}

u32 FONT_RENDERER::GenerateFontTextures(char* key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to create glyph textures of a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = fonts.at(key);
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
    for(std::map<u32, GLYPH*>::iterator it = font->glyphs.begin(); it != font->glyphs.end(); it++)
    {
        LoadTextureArrayLayer(font->textureHandleGL, it->second->bitmap, 0, 0, it->second->width, it->second->height, it->first, GL_RED);
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    font->assignedGL = true;
    return VHM_SUCCESS;
}

u32 FONT_RENDERER::FreeFontTextures(char* key)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to free glyph textures of a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = fonts.at(key);
    glDeleteTextures(1, &font->textureHandleGL);
    return VHM_SUCCESS;
}


u32 FONT_RENDERER::LoadNewFace(char* key, const char* filepath, int fontSize)
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

u32 FONT_RENDERER::DeriveFace(char* key, u32 newFontSize)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to derive from a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = fonts.at(key);

    char* cpyFontPath = (char*) malloc(strlen(font->fontPath) + 1);
    strcpy(cpyFontPath, font->fontPath);

    FreeFTFace(key);
    FreeFontGlyphs(key);
    FreeFontTextures(key);
    delete font;
    fonts.erase(key);

    if(LoadFTFace(key, cpyFontPath, newFontSize) == VHM_SUCCESS)
    {
        free(cpyFontPath);
        if(LoadFontGlyphs(key) == VHM_SUCCESS)
        {
            if(GenerateFontTextures(key) == VHM_SUCCESS)
            {
                return VHM_SUCCESS;
            }
        }
    }
    free(cpyFontPath);
    return VHM_ERROR;
}

u32 FONT_RENDERER::RenderText(char* key, const char* text, i32 screenX, i32 screenY, COLOR color)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to render a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return VHM_ERROR;
    }
    FONT* font = fonts.at(key);
    // Binds
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font->textureHandleGL);

    // Color matrix uniform
    UniformVec3(program, "color", color.vec);

    // Orthographic matrix uniform
    mat4 projection;
    glm_mat4_identity(projection);
    
    glm_ortho(0.0, GetWindowWidth(), GetWindowHeight(), 0.0, 0.0001, 10.0, projection);

    UniformMat4(program, "projection", projection);

    f32 caret = 0.0;
    f32 scale[3] = {(f32) font->maxWidth, (f32) font->maxHeight, 1.0};

    for(u32 i = 0; i < strlen(text); i++)
    {
        u32 charInd = FT_Get_Char_Index(font->ftFace, text[i]);
        if(font->glyphs.find(charInd) == font->glyphs.end())
        {
            continue;
        }
        GLYPH ch = *font->glyphs.at(charInd);
        
        f32 layer = (float) (i+1) / (strlen(text) + 1);
        glUniform1f(glGetUniformLocation(program, "defaultLayerZ"), GFX_LAYER_GUI + layer);

        f32 hb = font->capitalBearingY - ch.bearingY;

        f32 advance = ch.advance;
        f32 bearingX = ch.bearingX;
        // Remove bearing from the first letter, so that it's aligned correctly with screenX.
        if(i == 0)
        {
            advance -= bearingX;
            bearingX = 0;
        }
        vec3 translate = {(f32) screenX + (caret + bearingX), (float) screenY + hb, 0.0};

        mat4 transformation;
        glm_mat4_identity(transformation);
        glm_translate(transformation, translate);
        glm_scale(transformation, scale);

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

f32 FONT_RENDERER::TextWidth(char* key, const char* text)
{
    if(fonts.count(key) == 0)
    {
        printf("%s Unable to gauge the text width of a font (%s) that doesn't exist\n", VHM_FT_ERR, key);
        return -1;
    }
    FONT* font = fonts.at(key);

    u32 len = strlen(text);
    f32 width = 0;
    for(u32 i = 0; i < len; i++)
    {
        u32 charInd = FT_Get_Char_Index(font->ftFace, text[i]);
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