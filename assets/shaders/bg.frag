#version 330 core
out vec4 FragColor;
uniform float screenHeight;

void main() {
    vec3 botColor = vec3(0.85, 0.90, 0.96); // Soft bright sky blue
    vec3 topColor = vec3(0.95, 0.88, 0.95); // Soft lavender pink
    float y = gl_FragCoord.y / screenHeight;
    FragColor = vec4(mix(botColor, topColor, y), 1.0);
}
