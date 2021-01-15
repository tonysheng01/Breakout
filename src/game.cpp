/*
 game.cpp
 Breakout
 Last updated on October 5, 2020
 
 Implementation of the Game class
 A singleton class that outlines and controls the life cycle of the game
 */

#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "game_object.hpp"
#include "ball.hpp"
#include "particle_generator.hpp"
#include "post_processor.hpp"
#include "text_renderer.hpp"

#include <glm/glm.hpp>
#include <irrKlang/irrKlang.h>

#include <string>
#include <algorithm>
#include <iostream>

// Declaration of singleton objects
SpriteRenderer *renderer;
GameObject *paddle;
Ball *ball;
ParticleGenerator *particles;
PostProcessor *effects;
irrklang::ISoundEngine *sound;
TextRenderer *text;

// Declaration of helper methods
Direction ClosestDir(glm::vec2 target);
bool CheckCollisions(GameObject &a, GameObject &b);
CollisionRes CheckCollisions(Ball &ball, GameObject &block);
bool RollDice(unsigned int chance);

// Game creation
Game::Game(unsigned int width, unsigned int height) : State(GAME_MENU), Keys(), Width(width), Height(height), CurLevel(0), Lives(3) {
    memset(Keys, false, sizeof(Keys));
    memset(KeysProcessed, false, sizeof(KeysProcessed));
}

// Resource cleanup
Game::~Game() {
    delete renderer;
    delete paddle;
    delete ball;
    delete particles;
    delete effects;
    delete text;
    sound->drop();
}

// Game initialization
void Game::Init() {
    // load and configure shaders
    Shader shaderSprite = ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "shaderSprite"); // game object shader
    Shader shaderParticle = ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", nullptr, "shaderParticle"); // particle shader
    Shader shaderPP = ResourceManager::LoadShader("shaders/post_processing.vert", "shaders/post_processing.frag", nullptr, "shaderPP"); // post-processing shader
    
    glm::mat4 projection = glm::ortho(0.0f, (float)Width, (float)Height, 0.0f, -1.0f, 1.0f);
    shaderSprite.setInt("spriteTexture", 0);
    shaderSprite.setMat4("projection", projection);
    shaderParticle.setInt("sprite", 0);
    shaderParticle.setMat4("projection", projection);
    
    // load textures
    ResourceManager::LoadTexture("textures/block_solid.png", "block_solid", false);
    ResourceManager::LoadTexture("textures/block_breakable.png", "block_breakable", false);
    ResourceManager::LoadTexture("textures/background.jpg", "background", true);
    ResourceManager::LoadTexture("textures/paddle.png", "paddle", false);
    ResourceManager::LoadTexture("textures/ball.png", "ball", false);
    ResourceManager::LoadTexture("textures/powerup_chaos.png", "tex_chaos", false);
    ResourceManager::LoadTexture("textures/powerup_confuse.png", "tex_confuse", false);
    ResourceManager::LoadTexture("textures/powerup_size.png", "tex_size", true);
    ResourceManager::LoadTexture("textures/powerup_passthrough.png", "tex_pass", false);
    ResourceManager::LoadTexture("textures/powerup_speed.png", "tex_speed", false);
    ResourceManager::LoadTexture("textures/powerup_sticky.png", "tex_sticky", false);
    Texture2D textureParticle = ResourceManager::LoadTexture("textures/particle.png", "particle", false);
    
    // create singleton objects
    renderer = new SpriteRenderer(shaderSprite);
    particles = new ParticleGenerator(shaderParticle, textureParticle, 500);
    effects = new PostProcessor(shaderPP, Width, Height, true);
    sound = irrklang::createIrrKlangDevice();
    text = new TextRenderer(Width, Height);
    text->Load("fonts/OCRAEXT.TTF", 48);
    
    // load levels
    for (unsigned int i = 0; i < NUM_LEVELS; i++) {
        GameLevel level;
        const char *path = ("levels/level"+std::to_string(i)+".txt").c_str();
        level.Load(path, Width, Height / 2);
        Levels.push_back(level);
    }
    CurLevel = 0;
    
    // initialize paddle and ball
    glm::vec2 paddlePos = glm::vec2(Width / 2.0f - PADDLE_SIZE.x / 2.0f, Height - PADDLE_SIZE.y);
    Texture2D paddleTexture = ResourceManager::GetTexture("paddle");
    paddle = new GameObject(paddlePos, PADDLE_SIZE, paddleTexture, glm::vec3(1.0f), true);
    glm::vec2 ballPos(paddlePos + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f));
    Texture2D ballTexture = ResourceManager::GetTexture("ball");
    ball = new Ball(ballPos, BALL_RADIUS, BALL_V0, ballTexture);
    
    // play sound
    sound->play2D("audio/breakout.mp3", true);
}

// Game state update. Called each frame after ProcessInput.
void Game::Update(float dt) {
    ball->Move(dt, Width); // move ball
    EnforceCollisions(); // detect and resolve collisions
    particles->Update(dt, *ball, 2, glm::vec2(ball->Radius / 2.0f)); // update particles
    UpdatePowerUps(dt); // update powerup effects
    if (ShakeTime > 0.0f) {
        // controls shake effect
        ShakeTime -= dt;
        if (ShakeTime < 0.0f) {
            effects->Shake = false;
        }
    }
    if (ball->Position.y > Height) {
        // player loses a life if ball reaches bottom of screen
        Lives--;
        ResetPlayer();
        // reset game if all lives lost
        if (Lives == 0) {
            ClearPowerUps();
            ResetLevel();
            State = GAME_MENU;
        }
    }
    if (State == GAME_ACTIVE && Levels[CurLevel].Cleared()) {
        // player wins the game if all bricks are cleared
        ClearPowerUps();
        ResetLevel();
        ResetPlayer();
        effects->Chaos = true;
        State = GAME_WIN;
    }
}

// Update game state based on keyboard input.
void Game::ProcessInput(float dt) {
    if (State == GAME_MENU) {
        // player makes level selection
        if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
            State = GAME_ACTIVE;
            KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        if (Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W]) {
            CurLevel = (CurLevel + 1) % 4;
            KeysProcessed[GLFW_KEY_W] = true;
        }
        if (Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S]) {
            CurLevel = (CurLevel - 1) % 4;
            KeysProcessed[GLFW_KEY_S] = true;
        }
    } else if (State == GAME_WIN && Keys[GLFW_KEY_ENTER]) {
        // player may choose to reset game after winning
        State = GAME_MENU;
        KeysProcessed[GLFW_KEY_ENTER] = true;
        effects->Chaos = false;
    } else if (State == GAME_ACTIVE) {
        // play uses keyboard to control paddle and release ball
        float deltaDist = PADDLE_VELOCITY * dt;
        if (Keys[GLFW_KEY_A] && paddle->Position.x - deltaDist >= 0.0f) {
            paddle->Position.x -= deltaDist;
            if (ball->Stuck) {
                ball->Position.x -= deltaDist;
            }
        } else if (Keys[GLFW_KEY_D] && paddle->Position.x + deltaDist <= Width - paddle->Size.x) {
            paddle->Position.x += deltaDist;
            if (ball->Stuck) {
                ball->Position.x += deltaDist;
            }
        }
        if (Keys[GLFW_KEY_SPACE]) {
            ball->Stuck = false;
        }
    }
}

// Renders the game. Called each frame after Update.
void Game::Render() {
    // scene is first rendered to the post processor's framebuffer
    effects->BeginRender();
        
    Texture2D background = ResourceManager::GetTexture("background");
    renderer->DrawSprite(background, glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), 0.0f, glm::vec3(1.0f));
    Levels[CurLevel].Draw(*renderer);
    paddle->Draw(*renderer);
    for (PowerUp &powerUp : PowerUps) {
        if (!powerUp.Destroyed) {
            powerUp.Draw(*renderer);
        }
    }
    particles->Render();
    ball->Draw(*renderer);
    text->RenderText("Lives: " + std::to_string(Lives), 5.0f, 5.0f, 0.5f);
    
    effects->EndRender();
    
    // scene is rendered to screen after post processing is done
    effects->RenderToScreen(glfwGetTime());
    if (State == GAME_MENU) {
        text->RenderText("Press ENTER to start", 250.0f, Height / 2.0f - 20.0f, 0.5f);
        text->RenderText("Press W or S to select level", 235.0f, Height / 2.0f + 20.0f, 0.4f);
    } else if (State == GAME_WIN) {
        text->RenderText("You WON!!!", 320.0f, Height / 2.0f - 20.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, Height / 2.0f + 20.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

// Checks and resolves collisions between game objects
void Game::EnforceCollisions() {
    for (GameObject &brick : Levels[CurLevel].Bricks) {
        // ignore destroyed blocks
        if (brick.Destroyed) {
            continue;
        }
        
        // skip if no collision occurs
        CollisionRes collision = CheckCollisions(*ball, brick);
        if (!std::get<0>(collision)) {
            continue;
        }
        
        if (!brick.IsSolid) {
            // non-solid blocks are destroyed when hit by the ball
            brick.Destroyed = true;
            SpawnPowerUps(brick);
            sound->play2D("audio/bleep.mp3", false);
        } else {
            // when the ball hits solid blocks, a short shake effect is introduced
            ShakeTime = 0.05f;
            effects->Shake = true;
            sound->play2D("audio/solid.wav", false);
        }
        
        // the ball does not bounce back if the pass-through effect is enabled
        if (ball->PassThrough && !brick.IsSolid) {
            continue;
        }
        
        // make the ball bounce back when it hits a block. This is done by reversing the velocity in the closest orthogonal direction of the collision
        // also re-positions the ball if it overlaps with a block in any frame
        Direction dir = std::get<1>(collision);
        glm::vec2 diff = std::get<2>(collision);
        if (dir == LEFT || dir == RIGHT) {
            ball->Velocity.x = -ball->Velocity.x;
            float penetration = ball->Radius - std::abs(diff.x);
            ball->Position.x += (dir == LEFT ? penetration : -penetration);
        } else {
            ball->Velocity.y = -ball->Velocity.y;
            float penetration = ball->Radius - std::abs(diff.y);
            ball->Position.y += (dir == UP ? -penetration : penetration);
        }
    }
    
    // resolves collisions between the ball and the paddle
    CollisionRes collisionPaddle = CheckCollisions(*ball, *paddle);
    if (!ball->Stuck && std::get<0>(collisionPaddle)) {
        // the direction in which the ball bounces back is determined by the relative position of the contact point
        float paddleCenter = paddle->Position.x + paddle->Size.x * 0.5f;
        float dispFromCenter = ball->Position.x + ball->Radius - paddleCenter;
        float percentage = dispFromCenter / (paddle->Size.x * 0.5f);
        float strength = 2.0f;
        glm::vec2 v0 = ball->Velocity;
        ball->Velocity.x = BALL_V0.x * percentage * strength;
        ball->Velocity.y = -1.0f * abs(v0.y);
        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(v0);
        ball->Stuck = ball->Sticky; // makes ball stuck on the paddle if the sticky paddle effect is enabled
        sound->play2D("audio/bleep.wav", false);
    }
    
    // resolves collisions between powerups and the paddle
    for (PowerUp &powerUp : PowerUps) {
        if (!powerUp.Destroyed) {
            if (powerUp.Position.y >= Height) {
                powerUp.Destroyed = true;
            }
            if (CheckCollisions(*paddle, powerUp)) {
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Active = true;
                sound->play2D("audio/powerup.wav", false);
            }
        }
    }
}

// Clears all powerup effects and discards all remaining powerups
void Game::ClearPowerUps() {
    PowerUps.clear();
    ActivePowerUps.clear();
    effects->Chaos = false;
    effects->Confuse = false;
}

// Resets the ball and the paddle to their original positions
void Game::ResetPlayer() {
    paddle->Size = PADDLE_SIZE;
    paddle->Position = glm::vec2(Width * 0.5f - PADDLE_SIZE.x * 0.5f, Height - PADDLE_SIZE.y);
    ball->Reset(paddle->Position + glm::vec2(PADDLE_SIZE.x * 0.5f - BALL_RADIUS, -BALL_RADIUS * 2.0f), BALL_V0);
    paddle->Color = glm::vec3(1.0f);
    ball->Color = glm::vec3(1.0f);
}

// Reloads and resets the current level
void Game::ResetLevel() {
    Lives = 3;
    const char *path = ("levels/level"+std::to_string(CurLevel)+".txt").c_str();
    Levels[CurLevel].Load(path, Width, Height / 2);
}

// Randomly spawn powerups at the position of a block
void Game::SpawnPowerUps(GameObject &block) {
    Texture2D tex_speed = ResourceManager::GetTexture("tex_speed");
    Texture2D tex_sticky = ResourceManager::GetTexture("tex_sticky");
    Texture2D tex_pass = ResourceManager::GetTexture("tex_pass");
    Texture2D tex_size = ResourceManager::GetTexture("tex_size");
    Texture2D tex_confuse = ResourceManager::GetTexture("tex_confuse");
    Texture2D tex_chaos = ResourceManager::GetTexture("tex_chaos");
    
    if (RollDice(20)) {
        // 5% probability of spawning a "speed increase" powerup
        PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, tex_speed));
    } else if (RollDice(19)) {
        // 5% probability of spawning a "sticky paddle" powerup
        PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 5.0f, block.Position, tex_sticky));
    } else if (RollDice(18)) {
        // 5% probability of spawning a "pass through" powerup
        PowerUps.push_back(PowerUp("pass_through", glm::vec3(0.5f, 1.0f, 0.5f), 3.0f, block.Position, tex_pass));
    } else if (RollDice(17)) {
        // 5% probability of spawning a "pad size increase" powerup
        PowerUps.push_back(PowerUp("pad_size_increase", glm::vec3(1.0f, 0.6f, 0.4f), 3.0f, block.Position, tex_size));
    } else if (RollDice(8)) {
        // 10% probability of spawning a "confuse" powerup
        PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 3.0f, block.Position, tex_confuse));
    } else if (RollDice(7)) {
        // 10% probability of spawning a "chaos" powerup
        PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 3.0f, block.Position, tex_chaos));
    }
}

// Updates the status of all existing powerups in the game
void Game::UpdatePowerUps(float dt) {
    for (PowerUp &powerUp : PowerUps) {
        // updates position of the powerup
        powerUp.Position += powerUp.Velocity * dt;
        if (!powerUp.Active) {
            continue;
        }
        
        powerUp.Duration -= dt;
        if (powerUp.Duration <= 0.0f) {
            // for powerups with a limited duration, removes its effect at the end of its lifetime
            powerUp.Active = false;
            if (powerUp.Type == "sticky" && --ActivePowerUps["sticky"] == 0) {
                ball->Sticky = false;
                paddle->Color = glm::vec3(1.0f);
            } else if (powerUp.Type == "pass_through" && --ActivePowerUps["pass_through"] == 0) {
                ball->PassThrough = false;
                ball->Color = glm::vec3(1.0f);
            } else if (powerUp.Type == "pad_size_increase" && --ActivePowerUps["pad_size_increase"] == 0) {
                paddle->Size = PADDLE_SIZE;
            } else if (powerUp.Type == "confuse" && --ActivePowerUps["confuse"] == 0) {
                effects->Confuse = false;
            } else if (powerUp.Type == "chaos" && --ActivePowerUps["chaos"] == 0) {
                effects->Chaos = false;
            }
        }
    }
    // remove inactive powerups from the list
    PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(), [](const PowerUp &powerUp) {return powerUp.Destroyed && !powerUp.Active;}), PowerUps.end());
}

// Activates a powerup in the game
void Game::ActivatePowerUp(PowerUp &powerUp) {
    if (powerUp.Type == "speed") {
        ball->Velocity *= 1.2f;
    } else if (powerUp.Type == "sticky") {
        ball->Sticky = true;
        paddle->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    } else if (powerUp.Type == "pass_through") {
        ball->PassThrough = true;
        ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    } else if (powerUp.Type == "pad_size_increase" && paddle->Size.x == PADDLE_SIZE.x) {
        paddle->Size.x += 50;
    } else if (powerUp.Type == "confuse") {
        effects->Confuse = true;
    } else if (powerUp.Type == "chaos") {
        effects->Chaos = true;
    }
    ActivePowerUps[powerUp.Type]++;
}

// Given a vector in 2D space, returns the most closely-aligned direction out of the four primary orthogonal direction vectors
Direction ClosestDir(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };
    float max = 0.0f;
    unsigned int closestInd;
    for (unsigned int i = 0; i < 4; i++) {
        float curDot = glm::dot(compass[i], target);
        if (curDot > max) {
            max = curDot;
            closestInd = i;
        }
    }
    return (Direction)closestInd;
}

// Checks collision between two AABBs
bool CheckCollisions(GameObject &a, GameObject &b) {
    bool colX = (a.Position.x + a.Size.x >= b.Position.x) && (b.Position.x + b.Size.x >= a.Position.x);
    bool colY = (a.Position.y + a.Size.y >= b.Position.y) && (b.Position.y + b.Size.y >= a.Position.y);
    return colX && colY;
}

// Checks collision between a ball and a block (represented by an AABB)
// Returns the entering direction vector as weel as its closest orthogonal orientation if there is a collision
CollisionRes CheckCollisions(Ball &ball, GameObject &block) {
    glm::vec2 C = ball.Position + ball.Size / 2.0f;
    glm::vec2 B = block.Position + block.Size / 2.0f;
    glm::vec2 D = C - B;
    D = glm::clamp(D, -block.Size / 2.0f, block.Size / 2.0f);
    glm::vec2 P = B + D;
    glm::vec2 diff = P - C;
    if (glm::length(diff) <= ball.Radius) {
        return std::make_tuple(true, ClosestDir(diff), diff);
    } else {
        return std::make_tuple(false, UP, glm::vec2(0.0f)); // the last two parameters are meaningless if there is no collision
    }
}

// Randomly returns a boolean result where the probability of returning true is 1/range
bool RollDice(unsigned int range) {
    unsigned int random = rand() % range;
    return random == 0;
}
