// Fragment shader for rendering game objects

#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D spriteTexture;
uniform vec3 spriteColor;

void main() {
    fragColor = vec4(spriteColor, 1.0) * texture(spriteTexture, texCoords);
}
