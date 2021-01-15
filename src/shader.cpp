/*
 shader.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the Shader class
 The Shader class encapsulates built-in shader methods and provides utility functions to compile, use and manipulate shaders.
 */

#include "shader.hpp"

#include <iostream>

Shader::Shader() {}

// Compiles and links a shader program given source codes stored as C-type strings
void Shader::Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource) {
    unsigned int vertex, fragment, geometry;
    
    // compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // compile geometry shader if provided. Geometry shaders are currently not used in the game but this functionality is reserved for future updates.
    if (geometrySource) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometrySource, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    
    // link shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometrySource) {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    
    // clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometrySource) {
        glDeleteShader(geometry);
    }
}

// Set shader as active
void Shader::Use() const {
    glUseProgram(ID);
}

// Assign value to a named uniform variable op type bool
void Shader::setBool(const std::string &name, bool value) const {
    this->Use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

// Assign value to a named uniform variable of type int
void Shader::setInt(const std::string &name, int value) const {
    this->Use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// Assign value to a named uniform variable of type float
void Shader::setFloat(const std::string &name, float value) const {
    this->Use();
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// Assign value to a named uniform variable of type vec2 by passing in a glm::vec2
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    this->Use();
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

// Assign value to a named uniform variable of type vec2 by passing in 2 floats
void Shader::setVec2(const std::string &name, float x, float y) const {
    this->Use();
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

// Assign value to a named uniform variable of type vec3 by passing in a glm::vec3
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    this->Use();
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

// Assign value to a named uniform variable of type vec3 by passing in 3 floats
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    this->Use();
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

// Assign value to a named uniform variable of type vec4 by passing in a glm::vec4
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    this->Use();
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

// Assign value to a named uniform variable of type vec4 by passing in 4 floats
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    this->Use();
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

// Assign value to a named uniform variable of type mat2 (2x2 matrix) by passing in a glm::mat2
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    this->Use();
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Assign value to a named uniform variable of type mat3 (3x3 matrix) by passing in a glm::mat3
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    this->Use();
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Assign value to a named uniform variable of type mat4 (4x4 matrix) by passing in a glm::mat4
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    this->Use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Checks and reports shader compilation and linkage errors
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
