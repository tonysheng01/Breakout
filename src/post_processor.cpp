/*
 post_processor.cpp
 Breakout
 Last updated on October 3, 2020
 
 Implementation of the PostProcessor class
 A singleton class responsible for post-processing effects
 */

#include "post_processor.hpp"

#include <iostream>

// The post processor works by temporarily storing render data in intermediate framebuffers and post-process the entire scene as a texture.
PostProcessor::PostProcessor(Shader &shader, unsigned int width, unsigned int height, bool doubleRes) : PPShader(shader), Scene(), Width(width), Height(height), Chaos(false), Shake(false), Confuse(false) {
    // framebuffer size is doubled if using a retina screen
    if (doubleRes) {
        Width *= 2;
        Height *= 2;
    }
    
    glGenFramebuffers(1, &MSFBO);
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);
    // initialize multi-sampled framebuffer to which the scene is rendered in the first pass
    // a multi-sampled FBO is used to guard against visible aliasing
    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, Width, Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
    }
    // initialize intermediate framebuffer to blit the MSFBO to
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    Scene.Generate(Width, Height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Scene.ID, 0); // scene will be stored in this->Scene
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize intermediate FBO" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    InitRenderData();
    PPShader.setInt("scene", 0);
    // kernel sampling offsets
    float offset = 1.0f / 300.0f;
    glm::vec2 offsets[9] = {
        glm::vec2(-offset, offset),
        glm::vec2(0.0f, offset),
        glm::vec2(offset, offset),
        glm::vec2(-offset, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(offset, 0.0f),
        glm::vec2(-offset, -offset),
        glm::vec2(0.0f, -offset),
        glm::vec2(offset, -offset)
    };
    // kernel for edge detection effect
    int edge_kernel[9] = {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    };
    // kernel for blur effect
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    for (unsigned int i = 0; i < 9; i++) {
        PPShader.setVec2("offsets["+std::to_string(i)+"]", offsets[i]);
        PPShader.setInt("edge_kernel["+std::to_string(i)+"]", edge_kernel[i]);
        PPShader.setFloat("blur_kernel["+std::to_string(i)+"]", blur_kernel[i]);
    }
}

// Called before rendering the scene
void PostProcessor::BeginRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Called after the scene is rendered
void PostProcessor::EndRender() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST); // blit MSFBO to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Renders the post-processed scene to screen
void PostProcessor::RenderToScreen(float time) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind back to default framebuffer
    PPShader.Use();
    PPShader.setFloat("time", time);
    PPShader.setInt("chaos", Chaos);
    PPShader.setInt("confuse", Confuse);
    PPShader.setInt("shake", Shake);
    glActiveTexture(GL_TEXTURE0);
    Scene.Bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// Fills VBO and VAO
void PostProcessor::InitRenderData() {
    float quadVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
