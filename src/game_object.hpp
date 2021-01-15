/*
 game_object.hpp
 Breakout
 Last updated on September 30, 2020
 
 Declaration of the GameObject class
 Base class for all objects used in the game, including blocks, the paddle and the ball.
 */

#ifndef game_object_hpp
#define game_object_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"

class GameObject {
public:
    glm::vec2 Position;
    glm::vec2 Size;
    float Rotation;
    glm::vec2 Velocity;
    glm::vec3 Color;
    bool IsSolid; // setting this to true prevents a GameObject from being destroyed by the ball
    bool Destroyed;
    Texture2D Sprite;
    
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), bool isSolid = false);
    virtual ~GameObject();
    
    virtual void Draw(SpriteRenderer &renderer);
    
protected:
    static glm::vec2 Acceleration; // acceleration is constant throughout the game
};

#endif /* game_object_hpp */
