#include "Camera.h"
#include <algorithm>

Camera::Camera(float fov, float aspect, float nearP, float farP)
    : fov(fov), aspectRatio(aspect), nearPlane(nearP), farPlane(farP),
      position(8.0f, 10.0f, 8.0f),
      lookOffset(-8.0f, -10.0f, -8.0f),
      currentY(0.0f),
      targetY(0.0f),
      lerpSpeed(5.0f)
{
}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 currentPos = glm::vec3(position.x, position.y + currentY, position.z);
    glm::vec3 lookAtPoint = glm::vec3(0.0f, currentY, 0.0f);
    return glm::lookAt(currentPos, lookAtPoint, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getPosition() const {
    return glm::vec3(position.x, position.y + currentY, position.z);
}

void Camera::setTargetY(float y) {
    targetY = y;
}

void Camera::update(float deltaTime) {
    currentY += (targetY - currentY) * lerpSpeed * deltaTime;
}

void Camera::setAspectRatio(float aspect) {
    aspectRatio = aspect;
}
