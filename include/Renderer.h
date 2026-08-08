#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();
    void drawCube(const Shader& shader, const glm::mat4& model, const glm::vec3& color, bool useTexture = false, unsigned int textureId = 0) const;
    void drawCylinder(const Shader& shader, const glm::mat4& model, const glm::vec3& color, bool useTexture = false, unsigned int textureId = 0) const;
    void cleanup();
    
    unsigned int loadTexture(const std::string& path);

private:
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    unsigned int cylVAO, cylVBO, cylEBO;
    int cylIndexCount;

    void setupCubeMesh();
    void setupCylinderMesh();
};

#endif // RENDERER_H
