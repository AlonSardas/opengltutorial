#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main() {
    // vec2 flipped = vec2(TexCoord.x, 1 - TexCoord.y);
    // FragColor = texture(texture_diffuse1, flipped);
    // FragColor = vec4(TexCoords.x, TexCoords.y, 0.0f, 1.0f);
    FragColor = texture(texture_diffuse1, TexCoords);
    // FragColor = vec4(0.8f, 0.8f, 0.0f, 1.0f);
}
