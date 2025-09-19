#version 330 core

out vec4 FragColor;

// in vec3 ourColor;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixRatio;

void main() {
    FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), mixRatio);
    // FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
