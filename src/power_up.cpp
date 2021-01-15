/*
 power_up.cpp
 Breakout
 Last updated on October 1, 2020
 
 Implementation of the PowerUp class
 The PowerUp class inherits the Gameobject class, defining collectable powerups that introduce special effects in the game.
 */

#include "power_up.hpp"

PowerUp::PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D &texture) : GameObject(position, POWERUP_SIZE, texture, color, false), Type(type), Duration(duration), Active(false) {
    Velocity = POWERUP_V0;
}
