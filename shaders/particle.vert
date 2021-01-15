// Vertex shader for batch-rendering particles

#version 330 core

layout (location = 0) in vec2 iPos;
layout (location = 1) in vec2 iTexCoords;

out vec2 texCoords;
out vec4 particleColor;

const int MAX_COUNT = 500; // max number of particles

uniform mat4 projection;
uniform vec2 offsets[MAX_COUNT]; // offsets of individual particles relative to input position
uniform vec4 colors[MAX_COUNT]; // colors of individual particles

void main() {
    float scale = 10.0f;
    texCoords = iTexCoords;
    particleColor = colors[gl_InstanceID];
    gl_Position = projection * vec4(iPos * scale + offsets[gl_InstanceID], 0.0, 1.0);
}
