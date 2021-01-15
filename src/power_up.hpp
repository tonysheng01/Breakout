/*
 power_up.hpp
 Breakout
 Last updated on October 1, 2020
 
 Declaration of the PowerUp class
 The PowerUp class inherits the Gameobject class, defining collectable powerups that introduce special effects in the game.
 */

#ifndef power_up_hpp
#define power_up_hpp

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>

#include "game_object.hpp"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 POWERUP_V0(0.0f, 150.0f); // downward initial velocity

class PowerUp : public GameObject {
public:
    std::string Type;
    float Duration;
    bool Active;
    
    PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D &texture);
};

#endif /* power_up_hpp */
