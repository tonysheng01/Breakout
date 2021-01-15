/*
 game_object.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the GameObject class
 Base class for all objects used in the game, including blocks, the paddle and the ball.
 */

#include "game_object.hpp"

glm::vec2 GameObject::Acceleration = glm::vec2(0.0f, 100.0f); // acceleration is shared across all GameObjects

GameObject::GameObject() : Position(0.0f), Size(1.0f), Rotation(0.0f), Velocity(0.0f), Color(0.0f), IsSolid(false), Destroyed(false), Sprite() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, bool isSolid) : Position(pos), Size(size), Sprite(sprite), Color(color), IsSolid(isSolid), Velocity(0.0f), Rotation(0.0f), Destroyed(false) {}

GameObject::~GameObject() {}

// Renders the GameObject with a SpriteRenderer
void GameObject::Draw(SpriteRenderer &renderer) {
    renderer.DrawSprite(Sprite, Position, Size, Rotation, Color);
}
