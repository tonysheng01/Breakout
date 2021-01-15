/*
 sprite_renderer.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the SpriteRenderer class
 A singleton class to render 2D quads.
 */

#include "sprite_renderer.hpp"

SpriteRenderer::SpriteRenderer(Shader &shader) {
    this->shader = shader;
    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &quadVAO);
}

// Renders a 2D quad to current framebuffer
void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) {
    shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    shader.setMat4("model", model);
    shader.setVec3("spriteColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// Prepares VBO and VAO for rendering. Only has to be called once.
void SpriteRenderer::initRenderData() {
    float quadVertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    
    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
