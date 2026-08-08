#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Material
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;
uniform sampler2D texture1;
uniform bool useTexture; // To toggle texture if needed

// Light
uniform vec3 lightDir;
uniform vec3 lightColor;

// Camera
uniform vec3 viewPos;

void main() {
    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 dir = normalize(-lightDir);
    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(dir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 texColor = useTexture ? texture(texture1, TexCoords) : vec4(1.0);
    vec3 result = (ambient + diffuse + specular) * objectColor * texColor.rgb;
    FragColor = vec4(result, 1.0);
}
