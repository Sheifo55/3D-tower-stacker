#include "Renderer.h"
#include <vector>

Renderer::Renderer() : cubeVAO(0), cubeVBO(0), cubeEBO(0) {
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::init() {
    setupCubeMesh();
}

void Renderer::cleanup() {
    if (cubeVAO != 0) {
        glDeleteVertexArrays(1, &cubeVAO);
        cubeVAO = 0;
    }
    if (cubeVBO != 0) {
        glDeleteBuffers(1, &cubeVBO);
        cubeVBO = 0;
    }
    if (cubeEBO != 0) {
        glDeleteBuffers(1, &cubeEBO);
        cubeEBO = 0;
    }
}

void Renderer::setupCubeMesh() {
    // 24 vertices for the 6 faces (4 vertices per face to have proper normals)
    // Each vertex: pos (3), normal (3)
    float vertices[] = {
        // Back face (z = -0.5)
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Top-left

        // Front face (z = 0.5)
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Bottom-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Top-left

        // Left face (x = -0.5)
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Top-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Bottom-right

        // Right face (x = 0.5)
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Top-left
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Bottom-right
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Top-right

        // Bottom face (y = -0.5)
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Top-right
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Top-left
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Bottom-right

        // Top face (y = 0.5)
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Top-left
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Top-right
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // Bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f  // Bottom-left
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,       // Back
        4, 5, 6, 6, 7, 4,       // Front
        8, 9, 10, 10, 11, 8,    // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Bottom
        20, 21, 22, 22, 23, 20  // Top
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Renderer::drawCube(const Shader& shader, const glm::mat4& model, const glm::vec3& color) const {
    shader.use();
    shader.setMat4("model", model);
    shader.setVec3("objectColor", color);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
