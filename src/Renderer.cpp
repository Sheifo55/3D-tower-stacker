#include "Renderer.h"
#include <vector>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

Renderer::Renderer() : cubeVAO(0), cubeVBO(0), cubeEBO(0), cylVAO(0), cylVBO(0), cylEBO(0), cylIndexCount(0) {
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::init() {
    setupCubeMesh();
    setupCylinderMesh();
}

void Renderer::cleanup() {
    if (cubeVAO != 0) { glDeleteVertexArrays(1, &cubeVAO); cubeVAO = 0; }
    if (cubeVBO != 0) { glDeleteBuffers(1, &cubeVBO); cubeVBO = 0; }
    if (cubeEBO != 0) { glDeleteBuffers(1, &cubeEBO); cubeEBO = 0; }
    if (cylVAO != 0) { glDeleteVertexArrays(1, &cylVAO); cylVAO = 0; }
    if (cylVBO != 0) { glDeleteBuffers(1, &cylVBO); cylVBO = 0; }
    if (cylEBO != 0) { glDeleteBuffers(1, &cylEBO); cylEBO = 0; }
}

unsigned int Renderer::loadTexture(const std::string& path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;
        else format = GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

void Renderer::setupCubeMesh() {
    // 24 vertices for the 6 faces. Pos (3), Normal (3), TexCoords (2)
    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::setupCylinderMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    int segments = 32;
    float radius = 0.5f;
    float halfHeight = 0.5f;
    
    // Top center
    vertices.insert(vertices.end(), {0.0f, halfHeight, 0.0f,  0.0f, 1.0f, 0.0f,  0.5f, 0.5f});
    int topCenterIdx = 0;
    // Bottom center
    vertices.insert(vertices.end(), {0.0f, -halfHeight, 0.0f,  0.0f, -1.0f, 0.0f,  0.5f, 0.5f});
    int bottomCenterIdx = 1;
    
    int topStartIndex = vertices.size() / 8;
    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / segments * 2.0f * 3.14159265f;
        float c = cos(angle);
        float s = sin(angle);
        float u = (float)i / segments;
        
        // Top edge vertex (normal up)
        vertices.insert(vertices.end(), {c * radius, halfHeight, s * radius,  0.0f, 1.0f, 0.0f,  u, 1.0f});
        // Bottom edge vertex (normal down)
        vertices.insert(vertices.end(), {c * radius, -halfHeight, s * radius,  0.0f, -1.0f, 0.0f,  u, 0.0f});
        
        // Side top vertex (normal out)
        vertices.insert(vertices.end(), {c * radius, halfHeight, s * radius,  c, 0.0f, s,  u, 1.0f});
        // Side bottom vertex (normal out)
        vertices.insert(vertices.end(), {c * radius, -halfHeight, s * radius,  c, 0.0f, s,  u, 0.0f});
    }
    
    for (int i = 0; i < segments; ++i) {
        int base = topStartIndex + i * 4;
        
        // Top triangle
        indices.push_back(topCenterIdx);
        indices.push_back(base + 4);
        indices.push_back(base + 0);
        
        // Bottom triangle
        indices.push_back(bottomCenterIdx);
        indices.push_back(base + 1);
        indices.push_back(base + 5);
        
        // Side quad
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 7);
        indices.push_back(base + 7);
        indices.push_back(base + 6);
        indices.push_back(base + 2);
    }
    
    cylIndexCount = indices.size();
    
    glGenVertexArrays(1, &cylVAO);
    glGenBuffers(1, &cylVBO);
    glGenBuffers(1, &cylEBO);

    glBindVertexArray(cylVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cylVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Renderer::drawCube(const Shader& shader, const glm::mat4& model, const glm::vec3& color, bool useTexture, unsigned int textureId) const {
    shader.use();
    shader.setMat4("model", model);
    shader.setVec3("objectColor", color);
    shader.setInt("useTexture", useTexture ? 1 : 0);
    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader.setInt("texture1", 0);
    }
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawCylinder(const Shader& shader, const glm::mat4& model, const glm::vec3& color, bool useTexture, unsigned int textureId) const {
    shader.use();
    shader.setMat4("model", model);
    shader.setVec3("objectColor", color);
    shader.setInt("useTexture", useTexture ? 1 : 0);
    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader.setInt("texture1", 0);
    }
    glBindVertexArray(cylVAO);
    glDrawElements(GL_TRIANGLES, cylIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
