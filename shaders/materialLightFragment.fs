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

struct Light {
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
uniform Light light;
uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform Spotlight flashlight;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 getLight(vec3 lightDir, vec3 lightAmbient, vec3 materialAmbient, vec3 lightDiffuse, vec3 materialDiffuse,
              vec3 lightSpecular, vec3 materialSpecular);

void main() {
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 materialDiffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 materialSpecular = vec3(texture(material.specular, TexCoords));
    vec3 light1 = attenuation * getLight(lightDir, light.ambient, materialDiffuse, light.diffuse, materialDiffuse,
                                         light.specular, materialSpecular);

    vec3 light2 = getLight(directionalLight.direction, directionalLight.ambient, materialDiffuse,
                           directionalLight.diffuse, materialDiffuse, directionalLight.specular, materialSpecular);

    lightDir = normalize(flashlight.position - FragPos);
    float theta = dot(lightDir, normalize(flashlight.direction));
    float epsilon = flashlight.cutOff - flashlight.outerCutOff;
    float intensity = clamp((theta - flashlight.outerCutOff) / epsilon, 0.0, 1.0);
    distance = length(flashlight.position - FragPos);
    attenuation =
        1.0 / (flashlight.constant + flashlight.linear * distance + flashlight.quadratic * (distance * distance));
    vec3 light3 = getLight(flashlight.direction, flashlight.ambient, materialDiffuse, flashlight.diffuse,
                           materialDiffuse, flashlight.specular, materialSpecular);
    light3 *= intensity * attenuation;

    FragColor = vec4(light1 + light2 + light3, 1.0);
}

vec3 getLight(vec3 lightDir, vec3 lightAmbient, vec3 materialAmbient, vec3 lightDiffuse, vec3 materialDiffuse,
              vec3 lightSpecular, vec3 materialSpecular) {
    // ambient
    vec3 ambient = lightAmbient * materialAmbient;

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * materialDiffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightSpecular * spec * materialSpecular;

    return ambient + diffuse + specular;
}