#include "font.h"
#include "logging.h"
#include "gfx.h"
#include "engine_io.h"
#include "engine.h"

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
    font.assignedGL = false;
    fontpath = (char*) malloc(1);
    // Default Font: HACK @ 32
    LoadFTFace("C:/Windows/Fonts/HACK-REGULAR.ttf", 32);
    LoadFontGlyphs();
    GenerateFontTextures();

    // Generate VAO model
    f32 vertices[12] = {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    f32 texels[12] = {0.0, 0.0, 0.99, 0.99, 0.99, 0.0, 0.0, 0.0, 0.0, 0.99, 0.99, 0.99};

    glGenVertexArrays(1, &vao.handle);
    glBindVertexArray(vao.handle);
    SetBufferVAO(vao, 0, 2, vertices, GL_FLOAT, 12);
    SetBufferVAO(vao, 1, 2, texels, GL_FLOAT, 12);
    glBindVertexArray(0);

    // Create Shader Program
    char* vert = ReadString("vert.glsl");
    char* frag = ReadString("frag.glsl");

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
    glDeleteTextures(1, &font.textureHandleGL);

    FreeFTFace();
    FreeFontGlyphs();
    
    glDeleteVertexArrays(1, &vao.handle);
    glDeleteBuffers(vao.vbos.size(), &vao.vbos[0]);
}
    
void FONT_RENDERER::LoadFTFace(const char* filepath, int fontSize)
{
    if(faceAssigned)
    {
        FreeFTFace();
        FreeFontGlyphs();
        font.maxWidth = 0;
        font.maxHeight = 0;
        font.capitalBearingY = 0;
    }
    i32 error = FT_New_Face(library, filepath, 0, &face);
    if(error > 0)
    {
        printf("%s %s", VHM_FT_ERR, error, FT_Error_String(error));
    }
    fontpath = (char*) realloc(fontpath, strlen(filepath) + 1);
    strcpy(fontpath, filepath);
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    faceAssigned = true;
}

void FONT_RENDERER::FreeFTFace()
{
    FT_Done_Face(face);
}

void FONT_RENDERER::LoadFontGlyphs()
{
    u32 gindex;
    u64 charcode;

    charcode = FT_Get_First_Char(face, &gindex);
    while(gindex != 0)
    {
        u32 error = FT_Load_Char(face, charcode, FT_LOAD_RENDER);
        if(error)
        {
            printf("%s Error loading character %d\n", VHM_FT_ERR, gindex);
        }
        if(face->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
        {
            printf("%s Error where character %c is the incorrect pixel mode (%u)\n", VHM_FT_LOG, charcode, face->glyph->bitmap.pixel_mode);
        }

        GLYPH* glyph = new GLYPH;
        glyph->bitmap = (u8*) malloc(sizeof(u8) * face->glyph->bitmap.width * face->glyph->bitmap.rows);
        memcpy(glyph->bitmap, face->glyph->bitmap.buffer, sizeof(unsigned char) * face->glyph->bitmap.width * face->glyph->bitmap.rows);
        glyph->width = face->glyph->bitmap.width;
        glyph->height = face->glyph->bitmap.rows;
        glyph->advance = face->glyph->advance.x / 64;
        glyph->bearingX = face->glyph->metrics.horiBearingX / 64;
        glyph->bearingY = face->glyph->metrics.horiBearingY / 64;
        font.glyphs.insert(std::make_pair(gindex, glyph));

        if(charcode == '^')
        {
            font.capitalBearingY = glyph->bearingY;
        }
        if(face->glyph->bitmap.width > font.maxWidth)
        {
            font.maxWidth = face->glyph->bitmap.width;
        }
        if(face->glyph->bitmap.rows > font.maxHeight)
        {
            font.maxHeight = face->glyph->bitmap.rows;
        }
        charcode = FT_Get_Next_Char(face, charcode, &gindex);
    }
}

void FONT_RENDERER::FreeFontGlyphs()
{
    for(auto it = font.glyphs.begin(); it != font.glyphs.end(); it++)
    {
        free(it->second->bitmap);
        delete it->second;
    }
    font.glyphs.clear();
}

void FONT_RENDERER::GenerateFontTextures()
{
    // Delete the previous texture if it exists.
    if(font.assignedGL)
    {
        glDeleteTextures(1, &font.textureHandleGL);
    }

    // Generates a texture and stores the bitmaps in an OpenGL Texture Array
    glGenTextures(1, &font.textureHandleGL);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font.textureHandleGL);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    SetMinMag(GL_TEXTURE_2D_ARRAY, GL_LINEAR, GL_LINEAR);
    SetTextureArrayStorage(font.textureHandleGL, GL_R8, font.maxWidth, font.maxHeight, font.glyphs.size());
    for(std::map<i32, GLYPH*>::iterator it = font.glyphs.begin(); it != font.glyphs.end(); it++)
    {
        LoadTextureArrayLayer(font.textureHandleGL, it->second->bitmap, 0, 0, it->second->width, it->second->height, it->first, GL_RED);
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    font.assignedGL = true;
}

void FONT_RENDERER::LoadNewFace(const char* filepath, int fontSize)
{
    LoadFTFace(filepath, fontSize);
    LoadFontGlyphs();
    GenerateFontTextures();
}

void FONT_RENDERER::DeriveFace(int newFontSize)
{
    printf("deriving new face @ %d\n", newFontSize);
    LoadFTFace(fontpath, newFontSize);
    LoadFontGlyphs();
    GenerateFontTextures();
}

void FONT_RENDERER::RenderText(const char* text, int screenX, int screenY, vec3 color)
{
    // Binds
    glUseProgram(program);
    glBindVertexArray(vao.handle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font.textureHandleGL);

    // Color matrix uniform
    UniformVec3(program, "color", color);

    // Orthographic matrix uniform
    mat4 temp, projection;
    glm_mat4_identity(projection);
    
    glm_ortho(0.0, GetWindowWidth(), GetWindowHeight(), 0.0, 0.0001, 10.0, projection);

    UniformMat4(program, "projection", projection);

    f32 caret = 0.0;
    f32 scale[3] = {(f32) font.maxWidth, (f32) font.maxHeight, 1.0};

    for(u32 i = 0; i < strlen(text); i++)
    {
        u32 charInd = FT_Get_Char_Index(face, text[i]);
        if(font.glyphs.find(charInd) == font.glyphs.end())
        {
            continue;
        }
        GLYPH ch = *font.glyphs.at(charInd);

        f32 zind = (float) i / strlen(text);

        f32 hb = font.capitalBearingY - ch.bearingY;

        vec3 translate = {(f32) screenX + (caret + ch.bearingX), (float) screenY + hb, zind};

        mat4 transformation;
        glm_mat4_identity(transformation);
        glm_translate(transformation, translate);
        glm_scale(transformation, scale);

        UniformMat4(program, "transformation", transformation);

        glUniform1i(glGetUniformLocation(program, "layer"), FT_Get_Char_Index(face, text[i]));

        // Drawing
        glDrawArrays(GL_TRIANGLES, 0, 12);
        caret += ch.advance;
    }

    // Unbinds
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glBindVertexArray(0);
    glUseProgram(0);

}

f32 FONT_RENDERER::TextLength(const char* text)
{
    i32 len = strlen(text);
    f32 width;
    for(u32 i = 0; i < len; i++)
    {
        u32 charInd = FT_Get_Char_Index(face, text[i]);
        if(font.glyphs.find(charInd) == font.glyphs.end())
        {
            continue;
        }
        GLYPH ch = *font.glyphs.at(charInd);
        
        if(i == len - 1)
        {
            width += ch.advance - ch.bearingX;
        }
        else
        {
            width += ch.advance;
        }
    }
    return width;
}