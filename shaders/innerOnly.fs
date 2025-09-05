#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

void main() {
    float borderWidth = 0.05;
    if (TexCoords.x < borderWidth || TexCoords.x > 1.0 - borderWidth || TexCoords.y < borderWidth ||
        TexCoords.y > 1.0 - borderWidth) {
        discard;
    } else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
