#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include "Shader.h"

struct Character {
    float u1, v1; // Top-left texture coordinate
    float u2, v2; // Bottom-right texture coordinate
    float width;
    float height;
};

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    void init(int screenWidth, int screenHeight);
    void renderText(Shader& shader, const std::string& text, float x, float y, float scale, const glm::vec3& color);
    void updateScreenSize(int width, int height);

private:
    unsigned int VAO, VBO;
    unsigned int fontTexture;
    int screenWidth, screenHeight;
    glm::mat4 projection;

    std::map<char, Character> characters;
    void generateFontAtlas();
};
