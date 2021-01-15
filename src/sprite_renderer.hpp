/*
 sprite_renderer.hpp
 Breakout
 Last updated on September 30, 2020
 
 Declaration of the SpriteRenderer class
 A singleton class to render 2D quads.
 */

#ifndef sprite_renderer_hpp
#define sprite_renderer_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"

class SpriteRenderer {
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    
    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f), float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    
private:
    Shader shader;
    unsigned int quadVAO;
    
    void initRenderData();
};

#endif /* sprite_renderer_hpp */
