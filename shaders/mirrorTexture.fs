#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D mirrorTexture;

void main() {
    float borderWidth = 0.05;
    if (TexCoords.x < borderWidth || TexCoords.x > 1.0 - borderWidth || TexCoords.y < borderWidth ||
        TexCoords.y > 1.0 - borderWidth) {
        FragColor = vec4(0.7, 0.2, 0.2, 1.0);
    } else {
        vec4 color = texture(mirrorTexture, TexCoords);
        FragColor = color;
    }
}
