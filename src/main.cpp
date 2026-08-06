#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game.h"
#include "InputHandler.h"
#include "TextRenderer.h"

int screenWidth = 800;
int screenHeight = 600;
TextRenderer* globalTextRenderer = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->setAspectRatio((float)width / (float)(height ? height : 1));
    }
    if (globalTextRenderer) {
        globalTextRenderer->updateScreenSize(width, height);
    }
}



int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "3D Tower Stacker", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable VSync

    InputHandler::init(window);

    Camera camera;
    glfwSetWindowUserPointer(window, &camera);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    Shader blockShader("assets/shaders/block.vert", "assets/shaders/block.frag");
    Renderer renderer;
    renderer.init();

    Shader textShader("assets/shaders/text.vert", "assets/shaders/text.frag");
    TextRenderer textRenderer;
    textRenderer.init(screenWidth, screenHeight);
    globalTextRenderer = &textRenderer;

    Shader bgShader("assets/shaders/bg.vert", "assets/shaders/bg.frag");
    unsigned int dummyVAO;
    glGenVertexArrays(1, &dummyVAO);

    Game game;
    game.setCamera(&camera);
    // Note: the game state starts at START_SCREEN but does not auto-initialize blocks until startGame()
    // It is safe to just let it run. However, the first block is added in init(). Wait, game.init() is called on start.

    while (!glfwWindowShouldClose(window)) {
        float dt = calculateDeltaTime();
        
        if (InputHandler::isQuitPressed()) {
            glfwSetWindowShouldClose(window, true);
        }

        game.handleInput();
        game.update(dt);
        camera.update(dt);

        glClearColor(0.88f, 0.90f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render Background (disable depth writing)
        glDepthMask(GL_FALSE);
        bgShader.use();
        bgShader.setFloat("screenHeight", (float)screenHeight);
        glBindVertexArray(dummyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDepthMask(GL_TRUE);

        blockShader.use();

        blockShader.setMat4("projection", camera.getProjectionMatrix());
        blockShader.setMat4("view", camera.getViewMatrix());

        game.render(renderer, blockShader, textRenderer, textShader, screenWidth, screenHeight);

        GL_CHECK_ERROR();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &dummyVAO);
    renderer.cleanup();
    glfwTerminate();
    return 0;
}
