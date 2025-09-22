

#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
}
fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform samplerCube depthCubeMap;

uniform vec3 viewPos;
uniform vec3 lightDir;
uniform vec3 lightPos;

uniform float farPlane;

float ShadowCalculationLightSpace(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0)
        return 0.0;

    if (dot(fs_in.Normal, lightDir) < 0.0)
        return 0.0;

    vec3 normal = normalize(fs_in.Normal);
    // float bias = max(0.000005 * (1.0 - dot(normal, lightDir)), 0.00001);
    // float bias = -0.00005 * (1.0 - dot(normal, lightDir));
    float bias = -0.0001;
    // float bias = 0; // Not necessary if rendering with back face culling
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // PCF
    float shadow = 0.0;
    float middleFactor = 5.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;

            float factor = x == 0 && y == 0 ? middleFactor : 1.0;
            shadow += currentDepth - bias > pcfDepth ? factor : 0.0;
        }
    }
    shadow /= middleFactor + 8.0;

    return shadow;
}

float ShadowCalculation(vec3 fragPos) {
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    vec3 lightDirNorm = normalize(fragToLight);

    float bias;
    // if (dot(fs_in.Normal, fragToLight) >= 0.0)
    //     return 1.0;

    // // bias = 0.005;
    // else {
    //     bias = -0.001;
    // }
    vec3 normal = normalize(fs_in.Normal);

    // ise the fragment to light vector to sample from the depth map
    float closestDepth = texture(depthCubeMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    bias = -max(0.001 * (1.0 - dot(normal, lightDirNorm)), 0.001);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

vec3 getLight(vec3 lightColor, vec3 lightDirection, float shadow) {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // ambient
    vec3 ambient = 0.2 * lightColor;
    // diffuse
    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    return lighting;
}

void main() {
    vec3 dirLighting = getLight(vec3(0.8), lightDir, ShadowCalculationLightSpace(fs_in.FragPosLightSpace));
    vec3 lightDirection = normalize(lightPos - fs_in.FragPos);
    vec3 pointLighting = getLight(vec3(0.8), lightDirection, ShadowCalculation(fs_in.FragPos));

    FragColor = vec4(dirLighting + pointLighting, 1.0);
}
