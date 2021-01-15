// Fragment shader for text rendering

#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    // sampling the single-channel glyph texture and storing it in the alpha channel
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}
