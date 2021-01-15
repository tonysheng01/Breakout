/*
 post_processor.hpp
 Breakout
 Last updated on October 3, 2020
 
 Declaration of the PostProcessor class
 A singleton class responsible for post-processing effects
 */

#ifndef post_processor_hpp
#define post_processor_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"
#include "shader.hpp"

class PostProcessor {
public:
    Shader PPShader;
    Texture2D Scene; // the entire game scene stored in a texture
    unsigned int Width;
    unsigned int Height;
    bool Confuse; // the confuse effect rotates the scene by 180 degrees
    bool Shake; // the shake effect shakes the scene periodically
    bool Chaos; // the chaos effect heavily distorts the scene
    
    PostProcessor(Shader &shader, unsigned int width, unsigned int height, bool doubleRes);
    
    void BeginRender();
    void EndRender();
    void RenderToScreen(float time);
    
private:
    unsigned int FBO;
    unsigned int MSFBO; // multi-sampled framebuffer
    unsigned int RBO;
    unsigned int VAO;
    
    void InitRenderData();
};

#endif /* post_processor_hpp */
