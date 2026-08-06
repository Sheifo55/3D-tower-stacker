#include "InputHandler.h"

bool InputHandler::dropRequested = false;
bool InputHandler::restartRequested = false;
bool InputHandler::quitRequested = false;

void InputHandler::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

bool InputHandler::isDropPressed() {
    if (dropRequested) {
        dropRequested = false;
        return true;
    }
    return false;
}

bool InputHandler::isRestartPressed() {
    if (restartRequested) {
        restartRequested = false;
        return true;
    }
    return false;
}

bool InputHandler::isQuitPressed() {
    if (quitRequested) {
        quitRequested = false;
        return true;
    }
    return false;
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE) {
            dropRequested = true;
        } else if (key == GLFW_KEY_R) {
            restartRequested = true;
        } else if (key == GLFW_KEY_ESCAPE) {
            quitRequested = true;
        }
    }
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        dropRequested = true;
    }
}
