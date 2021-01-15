/*
 ball.hpp
 Breakout
 Last updated on September 30, 2020
 
 Declaration of the Ball class
 The Ball class inherits from the GameObject class with a few properties added. It describes the ball used in the game.
 */

#ifndef ball_hpp
#define ball_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.hpp"
#include "texture.hpp"

// Definition of a Ball object. There may only be one active Ball object at any time in the game.
class Ball : public GameObject {
public:
    float Radius; // radius of the ball
    bool Stuck; // if the ball is stuck on the paddle, it will not bounce off unless instructed by the player
    bool Sticky; // if the ball is sticky, it will be stuck when caught by the paddle
    bool PassThrough; // setting this to true allows the ball to pass through all bricks it hits without bouncing back
    
    Ball();
    Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    
    glm::vec2 Move(float dt, unsigned int window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif /* ball_hpp */
