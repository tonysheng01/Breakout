/*
 particle_generator.hpp
 Breakout
 Last updated on October 3, 2020
 
 Declaration of the ParticleGenerator class
 A singleton class responsible for particle effects
 */

#ifndef particle_generator_hpp
#define particle_generator_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "shader.hpp"
#include "texture.hpp"
#include "game_object.hpp"

// Definition of a particle
struct Particle {
    glm::vec2 Position;
    glm::vec2 Velocity;
    glm::vec4 Color;
    float Life; // remaining lifetime ranging from 0 to 1
    
    Particle() : Position(0.0f), Velocity(0.0f), Color(0.0f), Life(1.0f) {}
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader &shader, Texture2D &texture, unsigned int count);
    
    void Update(float dt, GameObject &object, unsigned int countNew, glm::vec2 offset = glm::vec2(0.0f));
    void Render();
    
private:
    std::vector<Particle> Particles;
    unsigned int Count;
    Shader ParticleShader;
    Texture2D ParticleTexture;
    unsigned int VAO;
    unsigned int lastUsedIndex = 0;
    
    void Init();
    unsigned int GetEmptySpot();
    void RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f));
};

#endif /* particle_generator_hpp */
