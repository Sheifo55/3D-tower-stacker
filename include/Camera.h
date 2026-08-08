#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov = 45.0f, float aspect = 4.0f/3.0f,
           float near = 0.1f, float far = 200.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;

    void setTargetY(float y);
    void update(float deltaTime);
    void setAspectRatio(float aspect);

private:
    glm::vec3 position;
    glm::vec3 lookOffset;
    float currentY;
    float targetY;
    float lerpSpeed;
    float orbitAngle;
    float orbitSpeed;

    float fov, aspectRatio, nearPlane, farPlane;
};
