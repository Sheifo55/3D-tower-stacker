#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputHandler {
public:
    static void init(GLFWwindow* window);

    static bool isDropPressed();   // Consumes the event (returns true once)
    static bool isRestartPressed();
    static bool isQuitPressed();

private:
    static bool dropRequested;
    static bool restartRequested;
    static bool quitRequested;

    static void keyCallback(GLFWwindow* window, int key,
                           int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button,
                                    int action, int mods);
};
