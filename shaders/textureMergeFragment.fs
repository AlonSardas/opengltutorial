#version 330 core

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixRatio;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixRatio);
    // Flip texture2
    // vec2 flippedCoor = vec2(1-TexCoord.x, TexCoord.y);
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, flippedCoor), mixRatio);
}
