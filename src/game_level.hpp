/*
 game_level.hpp
 Breakout
 Last updated on September 30, 2020
 
 Declaration of the GameLevel class
 Defines a level in the game consisting of layers of blocks
 */

#ifndef game_level_hpp
#define game_level_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "game_object.hpp"
#include "sprite_renderer.hpp"
#include "resource_manager.hpp"

class GameLevel {
public:
    std::vector<GameObject> Bricks; // stores data of all blocks in the level in order
    
    GameLevel();
    
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight); // loads a level from an external text file
    void Draw(SpriteRenderer &renderer);
    bool Cleared();
    
private:
    void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight); // called inside Load
};

#endif /* game_level_hpp */
