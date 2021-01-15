/*
 resource_manager.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the ResourceManager class
 A singleton resource manager that loads and stores shaders and textures.
 */

#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unordered_map<std::string, Shader> ResourceManager::Shaders;
std::unordered_map<std::string, Texture2D> ResourceManager::Textures;

ResourceManager::ResourceManager() {}

// Loads and compiles a shader from source paths and assigns it a name, and returns it
Shader ResourceManager::LoadShader(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath, std::string name) {
    Shaders[name] = LoadShaderFromFile(vShaderPath, fShaderPath, gShaderPath);
    return Shaders[name];
}

// Gets a shader by name
Shader ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

// Loads a texture from a source path, assigns it a name, and returns it
Texture2D ResourceManager::LoadTexture(const char *path, std::string name, bool flipUV) {
    Textures[name] = LoadTextureFromFile(path, flipUV);
    return Textures[name];
}

// Gets a texture by name
Texture2D ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

// Releases all allocated space for resources. Called at program termination.
void ResourceManager::CleanUp() {
    for (auto i : Shaders) {
        glDeleteProgram(i.second.ID);
    }
    for (auto i : Textures) {
        glDeleteTextures(1, &i.second.ID);
    }
}

// Helper method to load and compile a shader program given its source paths
// Only called inside LoadShader
Shader ResourceManager::LoadShaderFromFile(const char *vShaderPath, const char *fShaderPath, const char *gShaderPath) {
    // pipeline: filestream -> stringstream -> string -> c_str
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vShaderPath);
        fShaderFile.open(fShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // geometry shaders are reserved for future updates
        if (gShaderPath) {
            gShaderFile.open(gShaderPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderPath ? gShaderCode : nullptr);
    return shader;
}

// Helper method to load a texture given its source path
// Only called inside LoadTexture
Texture2D ResourceManager::LoadTextureFromFile(const char *path, bool flipUV) {
    Texture2D texture;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flipUV);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (nrChannels == 4) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}
