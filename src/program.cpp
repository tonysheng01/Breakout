/*
 program.cpp
 Breakout
 Last updated on October 5, 2020
 
 This is the file containing the "main" function that gets directly executed when the project is run.
 It creates a Game instance and manages a GLFW window in which the game is displayed.
 */

// Includes
#include <glad/glad.h>
#include <GLFW/GLFW3.H>

#include <iostream>

#include "game.hpp"
#include "resource_manager.hpp"

// Method declaration
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Creation of game
Game breakout(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, const char *argv[]) {
    // GLFW window initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Breakout", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    
    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Global OpenGL state configuration
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Game initialization
    breakout.Init();
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        
        // Game data update
        breakout.ProcessInput(deltaTime);
        breakout.Update(deltaTime);
        
        // Game rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        breakout.Render();
        
        glfwSwapBuffers(window);
    }
    
    // Clean-up
    ResourceManager::CleanUp();
    glfwTerminate();
    return 0;
}

// Processes key presses. Automatically called upon any keyboard action
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            breakout.Keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            breakout.Keys[key] = false;
            breakout.KeysProcessed[key] = false;
        }
    }
}

// Resizes the scene to match target width and height. Automatically called when window size is changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
