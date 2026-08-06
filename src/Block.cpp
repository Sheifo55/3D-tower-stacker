#include "Block.h"

glm::mat4 Block::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, size);
    return model;
}

void Block::update(float deltaTime) {
    if (flashTimer > 0.0f) {
        flashTimer -= deltaTime;
    }
}

glm::mat4 ScrapPiece::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
    model = glm::scale(model, size);
    return model;
}

void ScrapPiece::update(float deltaTime) {
    velocity.y -= 9.8f * deltaTime; // Gravity
    position += velocity * deltaTime;
    rotationAngle += rotationSpeed * deltaTime;
}

bool ScrapPiece::isOffScreen() const {
    return position.y < -30.0f; // clean up when far below
}
