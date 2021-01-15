// Vertex shader for post-processing effects

#version 330 core

layout (location = 0) in vec2 iPos;
layout (location = 1) in vec2 iTexCoords;

out vec2 texCoords;

uniform bool chaos;
uniform bool shake;
uniform bool confuse;
uniform float time;

void main() {
    gl_Position = vec4(iPos, 0.0f, 1.0f);
    texCoords = iTexCoords;
    
    if (chaos) {
        // rotates texture coordinates in a circular fashion. Repeat if out of bounds.
        float strength = 0.3;
        texCoords = vec2(texCoords.x + sin(time) * strength, texCoords.y + cos(time) * strength);
    }
    if (confuse) {
        // inverts texture coordinates
        texCoords = vec2(1.0) - texCoords;
    }
    if (shake) {
        // slightly shakes the scene by directly modifying vertex positions
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}
