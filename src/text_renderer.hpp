/*
 text_renderer.hpp
 Breakout
 Last updated on October 1, 2020
 
 Declaration of the TextRenderer class
 A singleton class to render texts.
 */

#ifndef text_renderer_hpp
#define text_renderer_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

#include "texture.hpp"
#include "shader.hpp"

// Definition of a Truetype character
struct Character {
    unsigned int Glyph; // stored as texture
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class TextRenderer {
public:
    std::unordered_map<char, Character> Characters; // stores all ASCII characters
    Shader TextShader;
    
    TextRenderer(unsigned int width, unsigned int height);
    void Load(std::string fontPath, unsigned int fontSize);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
    
private:
    unsigned int VAO;
    unsigned int VBO;
};

#endif /* text_renderer_hpp */
