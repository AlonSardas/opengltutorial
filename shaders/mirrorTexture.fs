#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D mirrorTexture;

void main() { FragColor = vec4(texture(mirrorTexture, TexCoords)); }
