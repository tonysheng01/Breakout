/*
 game.hpp
 Breakout
 Last updated on October 5, 2020
 
 Declaration of the Game class
 A singleton class that outlines and controls the life cycle of the game
 */

#ifndef game_hpp
#define game_hpp

#include <glad/glad.h>
#include <GLFW/GLFW3.H>

#include <tuple>
#include <unordered_map>

#include "game_level.hpp"
#include "power_up.hpp"

const unsigned int NUM_LEVELS = 4; // number of levels
const glm::vec2 PADDLE_SIZE(100.0f, 20.0f); // size of player-controlled paddle
const float PADDLE_VELOCITY(500.0f); // horizontal velocity of the paddle
const float BALL_RADIUS = 12.5f; // default radius of the ball
const glm::vec2 BALL_V0(100.0f, -350.0f); // initial velocity of the ball

enum GameState {
    GAME_ACTIVE, // currently playing
    GAME_MENU, // level selection
    GAME_WIN // game won
};

enum Direction {
    UP, RIGHT, DOWN, LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> CollisionRes;

class Game {
public:
    GameState State; // current 
    bool Keys[1024]; // press/release state of each key
    unsigned int Width, Height; // window dimensions
    std::vector<GameLevel> Levels; // collection of all levels
    unsigned int CurLevel; // currently selected level
    std::vector<PowerUp> PowerUps; // collection of all active powerups
    bool KeysProcessed[1024]; // indication of whether actions have already been triggered in the current continuous session
    unsigned int Lives; // player lives remaining
    
    Game(unsigned int width, unsigned int height);
    ~Game();
    
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void EnforceCollisions();
    void ResetPlayer();
    void ResetLevel();
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(float dt);
    
private:
    float ShakeTime;
    std::unordered_map<std::string, unsigned int> ActivePowerUps;
    
    void ActivatePowerUp(PowerUp &powerUp);
    void ClearPowerUps();
};

#endif /* game_hpp */
