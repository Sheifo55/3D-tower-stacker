#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Block {
    glm::vec3 position;     // Center of the block
    glm::vec3 size;         // Width (X), Height (Y), Depth (Z)
    glm::vec3 color;        // RGB color
    float flashTimer = 0.0f; // For perfect placement flash

    glm::mat4 getModelMatrix() const;
    void update(float deltaTime);
    // Returns: translate(position) * scale(size)
};

struct ScrapPiece {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 color;
    glm::vec3 velocity;     // Falling velocity
    glm::vec3 rotationAxis;
    float rotationAngle;    // Accumulates over time
    float rotationSpeed;    // Degrees per second

    glm::mat4 getModelMatrix() const;
    void update(float deltaTime);
    bool isOffScreen() const; // position.y < -20.0f
};
