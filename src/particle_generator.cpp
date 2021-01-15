/*
 particle_generator.cpp
 Breakout
 Last updated on October 3, 2020
 
 Implementation of the ParticleGenerator class
 A singleton class responsible for particle effects
 */

#include "particle_generator.hpp"

#include <string>

ParticleGenerator::ParticleGenerator(Shader &shader, Texture2D &texture, unsigned int count) : ParticleShader(shader), ParticleTexture(texture), Count(count) {
    Init();
}

// Called each frame to replace dead particles and update status of remaining particles
void ParticleGenerator::Update(float dt, GameObject &object, unsigned int countNew, glm::vec2 offset) {
    // respawn a given number of particles
    for (unsigned int i = 0; i < countNew; i++) {
        int fillInd = GetEmptySpot();
        RespawnParticle(Particles[fillInd], object, offset);
    }
    // update particle states
    for (unsigned int i = 0; i < Count; i++) {
        Particle &p = Particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f) {
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

// Called each frame after Update to render particles
// Instanced rendering is used to improve efficiency
void ParticleGenerator::Render() {
    unsigned int numActive = 0;
    for (Particle &particle : Particles) {
        if (particle.Life > 0.0f) {
            numActive++;
            ParticleShader.setVec2("offsets["+std::to_string(numActive-1)+"]", particle.Position);
            ParticleShader.setVec4("colors["+std::to_string(numActive-1)+"]", particle.Color);
        }
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // particles are rendered additively on top of the scene
    ParticleShader.Use();
    glActiveTexture(0);
    ParticleTexture.Bind();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numActive);
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // reset blend mode
}

// Initiates Particles array and fills VAO. Particles are rendered as tiny colored squares.
void ParticleGenerator::Init() {
    float quadVertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
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
    
    for (unsigned int i = 0; i < Count; i++) {
        Particles.push_back(Particle());
    }
}

// Searches for a dead particle to be replaced. Returns the index of the first such particle.
unsigned int ParticleGenerator::GetEmptySpot() {
    // starts after the last replaced index to increase probability of a hit
    for (unsigned int i = lastUsedIndex; i < Count; i++) {
        if (Particles[i].Life <= 0.0f) {
            lastUsedIndex = i;
            return i;
        }
    }
    // perform a linear search from the beginning otherwise
    for (unsigned int i = 0; i < lastUsedIndex; i++) {
        if (Particles[i].Life <= 0.0f) {
            lastUsedIndex = i;
            return i;
        }
    }
    // replaces the first particle if none is dead
    lastUsedIndex = 0;
    return 0;
}

// Spawn a particle that adheres to a game object
void ParticleGenerator::RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset) {
    float random = (rand() % 100 - 50) / 10.0f;
    float rColor = 0.5f + rand() % 100 / 100.0f;
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(glm::vec3(rColor), 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}
