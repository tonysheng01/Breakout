/*
 resource_manager.hpp
 Breakout
 Last updated on September 30, 2020
 
 Declaration of the ResourceManager class
 A singleton resource manager that loads and stores shaders and textures.
 */

#ifndef resource_manager_hpp
#define resource_manager_hpp

#include <glad/glad.h>

#include <unordered_map>
#include <string>

#include "texture.hpp"
#include "shader.hpp"

class ResourceManager {
public:
    static std::unordered_map<std::string, Shader> Shaders; // A map of all shader objects used in the game, distinguished by names
    static std::unordered_map<std::string, Texture2D> Textures; // A map of all texture objects used in the game, distinguished by names
    
    static Shader LoadShader(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath, std::string name);
    static Shader GetShader(std::string name);
    static Texture2D LoadTexture(const char *path, std::string name, bool flipUV);
    static Texture2D GetTexture(std::string name);
    static void CleanUp();
    
private:
    ResourceManager();
    static Shader LoadShaderFromFile(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath = nullptr);
    static Texture2D LoadTextureFromFile(const char *path, bool flipUV);
};

#endif /* resource_manager_hpp */
