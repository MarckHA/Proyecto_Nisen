#version 330 core

// Entradas
in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// Salidas
out vec4 FragColor;

// Uniformes
uniform sampler2D map_kd;    // Textura difusa
uniform sampler2D map_ks;    // Textura especular
uniform sampler2D map_normal; // Textura de normales
uniform float shininess;     // Brillo especular

// Estructuras de luz
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct SpotLight {
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

// Máximo número de luces puntuales
#define NR_POINT_LIGHTS 4

// Uniformes para las luces
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;

// Función de cálculo de luz direccional
vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 ambient = light.ambient * texture(map_kd, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(map_kd, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(map_ks, TexCoords).rgb;
    return ambient + diffuse + specular;
}

// Función de cálculo de luces puntuales
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * texture(map_kd, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(map_kd, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(map_ks, TexCoords).rgb;
    return (ambient + diffuse + specular) * attenuation;
}

// Función de cálculo de spotlight
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * texture(map_kd, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(map_kd, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(map_ks, TexCoords).rgb;
    return (ambient + diffuse + specular) * attenuation * intensity;
}

void main() {
    vec3 normal = normalize(texture(map_normal, TexCoords).rgb * 2.0 - 1.0); // Convertir a [-1,1]
    vec3 viewDir = normalize(viewPos - FragPos);

    // Iluminación total
    vec3 result = vec3(0.0);

    // Luz direccional
    result += calculateDirLight(dirLight, normal, viewDir);

    // Luces puntuales
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calculatePointLight(pointLights[i], normal, FragPos, viewDir);
    }

    // Spotlight
    result += calculateSpotLight(spotLight, normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}
