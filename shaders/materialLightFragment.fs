/*
See http://devernay.free.fr/cours/opengl/materials.html for a table
with different materials and their values

See http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
for values of light attenuation
And table here https://learnopengl.com/Lighting/Light-casters
*/

#version 330 core

// struct Material {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// };

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct MaterialColors {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform PointLight movingLight;
uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform Spotlight flashlight;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 getLight(vec3 lightDir, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, MaterialColors materialColors);
vec3 calcDirectionalLight(DirectionalLight light, MaterialColors materialColors);
vec3 calcPointLight(PointLight light, MaterialColors materialColors);
vec3 calcSpotlight(Spotlight light, MaterialColors materialColors);

void main() {
    MaterialColors materialColors;
    materialColors.diffuse = vec3(texture(material.diffuse, TexCoords));
    materialColors.ambient = materialColors.diffuse;
    materialColors.specular = vec3(texture(material.specular, TexCoords));
    materialColors.shininess = material.shininess;

    vec3 result = vec3(0.0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], materialColors);
    }

    result += calcPointLight(movingLight, materialColors);

    result += calcDirectionalLight(directionalLight, materialColors);

    result += calcSpotlight(flashlight, materialColors);

    FragColor = vec4(result, 1.0);
}

vec3 calcPointLight(PointLight light, MaterialColors materialColors) {
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    return attenuation * getLight(lightDir, light.ambient, light.diffuse, light.specular, materialColors);
}

vec3 calcDirectionalLight(DirectionalLight light, MaterialColors materialColors) {
    return getLight(light.direction, light.ambient, light.diffuse, light.specular, materialColors);
}

vec3 calcSpotlight(Spotlight light, MaterialColors materialColors) {
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 baseLight = getLight(lightDir, light.ambient, light.diffuse, light.specular, materialColors);
    return intensity * attenuation * baseLight;
}

vec3 getLight(vec3 lightDir, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, MaterialColors materialColors) {
    // ambient
    vec3 ambient = lightAmbient * materialColors.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * materialColors.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialColors.shininess);
    vec3 specular = lightSpecular * spec * materialColors.specular;

    return ambient + diffuse + specular;
}