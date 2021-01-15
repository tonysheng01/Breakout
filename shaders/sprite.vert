// Vertex shader for rendering game objects

#version 330 core

layout (location = 0) in vec2 iPos;
layout (location = 1) in vec2 iTexCoords;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 model;

void main() {
    texCoords = iTexCoords;
    gl_Position = projection * model * vec4(iPos, 0.0, 1.0);
}
