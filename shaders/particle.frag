// Fragment shader for batch-rendering particles

#version 330 core

out vec4 fragColor;

in vec2 texCoords;
in vec4 particleColor;

uniform sampler2D sprite;

void main() {
    fragColor = texture(sprite, texCoords) * particleColor;
}
