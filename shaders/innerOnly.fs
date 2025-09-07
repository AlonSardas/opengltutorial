#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

void main() {
    float borderWidth = 0.05;
    if (TexCoords.x < borderWidth || TexCoords.x > 1.0 - borderWidth || TexCoords.y < borderWidth ||
        TexCoords.y > 1.0 - borderWidth) {
        // float dist = distance(TexCoords, vec2(0.5, 0.5));
        // if (dist > 0.5 - 0.1) {
        discard;
    } else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
