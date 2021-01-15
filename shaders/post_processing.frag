// Fragment shader for post-processing effects

#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D scene;
uniform vec2 offsets[9]; // sampling offsets
uniform int edge_kernel[9]; // 3x3 edge detection kernel
uniform float blur_kernel[9]; // 3x3 blur kernel
uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main() {
    vec4 color = vec4(0.0f);
    vec3 sample[9];
    if (chaos || shake) {
        // taking a total of 9 samples around each fragment
        for (int i = 0; i < 9; i++) {
            sample[i] = vec3(texture(scene, texCoords + offsets[i]));
        }
    } else {
        color = texture(scene, texCoords);
    }
    
    if (chaos) {
        // convoluting the samples with the edge detection kernel
        for (int i = 0; i < 9; i++) {
            color += vec4(sample[i] * edge_kernel[i], 0.0f);
        }
        color.a = 1.0f;
    }
    if (shake) {
        // convoluting the samples with the blue kernel
        for (int i = 0; i < 9; i++) {
            color += vec4(sample[i] * blur_kernel[i], 0.0f);
        }
        if (chaos) {
            // normalizing the output after both effects are applied
            color /= 2.0f;
        }
        color.a = 1.0f;
    }
    if (confuse) {
        // inverting the colors
        color = vec4(1.0f - color.rgb, 1.0f);
    }
    
    fragColor = color;
}
