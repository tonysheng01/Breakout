/*
 ball.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the Ball class
 The Ball class inherits from the GameObject class with a few properties added. It describes the ball used in the game.
 */

#include "ball.hpp"

#include <iostream>

// Default Ball constructor. A ball is stuck on the paddle when created.
Ball::Ball() : GameObject(), Radius(12.5f), Stuck(true), Sticky(false), PassThrough(false) {}

// Ball constructor with the 4 basic parameters.
Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) : GameObject(pos, glm::vec2(radius * 2), sprite, glm::vec3(1.0f), true), Radius(radius), Stuck(true), Sticky(false), PassThrough(false) {
    Velocity = velocity;
}

// Moves the ball  and returns its new position. This function is called each frame.
glm::vec2 Ball::Move(float dt, unsigned int window_width) {
    // only moves the ball if it is not stuck on the paddle
    if (!Stuck) {
        Position += dt * Velocity + 0.5f * Acceleration * dt * dt;
        Velocity += dt * Acceleration;
        // ball bounces back when reaching the top or the two sides of the screen
        if (Position.x <= 0.0f) {
            Velocity.x = -Velocity.x;
            Position.x = 0.0f;
        } else if (Position.x + Radius >= window_width) {
            Velocity.x = -Velocity.x;
            Position.x = window_width - Radius;
        }
        if (Position.y <= 0.0f) {
            Velocity.y = -Velocity.y;
            Position.y = 0.0f;
        }
    }
    return Position;
}

// Resets the ball
void Ball::Reset(glm::vec2 position, glm::vec2 velocity) {
    Position = position;
    Velocity = velocity;
    Stuck = true;
    Sticky = false;
    PassThrough = false;
}
