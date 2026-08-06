#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();
    void drawCube(const Shader& shader, const glm::mat4& model, const glm::vec3& color) const;
    void cleanup();
private:
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    void setupCubeMesh();
};

#endif // RENDERER_H
