/*
 text_renderer.cpp
 Breakout
 Last updated on October 1, 2020
 
 Implementation of the TextRenderer class
 A singleton class to render texts.
 */

#include "text_renderer.hpp"

#include "resource_manager.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

// Creates a TextRenderer object given screen dimensions
TextRenderer::TextRenderer(unsigned int width, unsigned int height) {
    TextShader = ResourceManager::LoadShader("shaders/text.vert", "shaders/text.frag", nullptr, "text");
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    TextShader.setMat4("projection", projection);
    TextShader.setInt("text", 0);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Loads a FreeType font from a given path and generates a character map
void TextRenderer::Load(std::string fontPath, unsigned int fontSize) {
    Characters.clear();
    // initialize Freetype loader
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not initialize Freetype library" << std::endl;
    }
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // guards against alignment issues
    // loops through all ASCII characters, generates a glyph for each
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load glyph " << (unsigned int)c << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    // clean up
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

// Renders a string of text
// Pre: Load has successfully returned
void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
    TextShader.Use();
    TextShader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (Characters['H'].Bearing.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[] = {
            xpos, ypos, 0.0f, 0.0f,
            xpos, ypos + h, 0.0f, 1.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos + w, ypos + h, 1.0f, 1.0f
        };
        glBindTexture(GL_TEXTURE_2D, ch.Glyph);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        x += (ch.Advance >> 6) * scale; // multiply by 64 as advance is in units of 1/64 pixels
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
