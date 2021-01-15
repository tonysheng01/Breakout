/*
 game_level.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the GameLevel class
 Defines a level in the game consisting of layers of blocks
 */

#include <sstream>
#include <fstream>
#include <iostream>

#include "game_level.hpp"

// colors of the blocks
const glm::vec3 COLOR_SOLID(0.8f, 0.8f, 0.7f);
const glm::vec3 COLOR_BREAKABLE1(0.2f, 0.6f, 1.0f);
const glm::vec3 COLOR_BREAKABLE2(0.0f, 0.7f, 0.0f);
const glm::vec3 COLOR_BREAKABLE3(0.8f, 0.8f, 0.4f);
const glm::vec3 COLOR_BREAKABLE4(1.0f, 0.5f, 0.0f);

GameLevel::GameLevel() {}

// Constructs a GameLevel from a text file
void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight) {
    Bricks.clear();
    
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream f(file);
    std::vector<std::vector<unsigned int>> tileData; // stores block data in a 2D vector
    if (f) {
        while (std::getline(f, line)) {
            std::istringstream s(line);
            std::vector<unsigned int> row;
            while (s >> tileCode) {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }if (tileData.size() > 0) {
            Init(tileData, levelWidth, levelHeight);
        }
    }
}

// Renders all undestroyed blocks in the level
void GameLevel::Draw(SpriteRenderer &renderer) {
    for (GameObject &brick : Bricks) {
        if (!brick.Destroyed) {
            brick.Draw(renderer);
        }
    }
}

// Level is cleared if all non-solid blocks are destroyed
bool GameLevel::Cleared() {
    for (GameObject &brick : Bricks) {
        if (!brick.IsSolid && !brick.Destroyed) {
            return false;
        }
    }
    return true;
}

// Creates GameObjects based on a 2D vector of block data loaded from file
void GameLevel::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
    unsigned int nrCols = (unsigned int)tileData[0].size();
    unsigned int nrRows = (unsigned int)tileData.size();
    float unit_width = levelWidth / nrCols;
    float unit_height = levelHeight / nrRows;
    
    for (unsigned int i = 0; i < nrRows; i++) {
        for (unsigned int j = 0; j < nrCols; j++) {
            if (tileData[i][j] == 0) { // a 0 signifies an empty block
                // empty blocks are no longer dealt with starting here
                continue;
            }
            
            glm::vec2 pos(unit_width * j, unit_height * i);
            glm::vec2 size(unit_width, unit_height);
            Texture2D sprite;
            bool isSolid;
            glm::vec3 color;
            if (tileData[i][j] == 1) { // a 1 signifies a solid block
                isSolid = true;
                sprite = ResourceManager::GetTexture("block_solid");
                color = COLOR_SOLID;
            } else {
                isSolid = false;
                sprite = ResourceManager::GetTexture("block_breakable");
                // numbers 2 to 5 all indicate non-solid blocks which differ in color
                switch (tileData[i][j]) {
                    case 2:
                        color = COLOR_BREAKABLE1;
                        break;
                    case 3:
                        color = COLOR_BREAKABLE2;
                        break;
                    case 4:
                        color = COLOR_BREAKABLE3;
                        break;
                    case 5:
                        color = COLOR_BREAKABLE4;
                        break;
                }
            }
            GameObject block(pos, size, sprite, color, isSolid);
            Bricks.push_back(block);
        }
    }
}
