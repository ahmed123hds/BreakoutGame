#include "Input/InputManager.h"

InputManager::InputManager() : window(nullptr) {
}

InputManager::~InputManager() {
}

void InputManager::Initialize(GLFWwindow* glfwWindow) {
    window = glfwWindow;
}

void InputManager::Update() {
    if (!window) return;
    
    UpdateKeyStates();
    UpdateMouseStates();
}

bool InputManager::IsKeyPressed(int key) const {
    auto it = currentKeyStates.find(key);
    return it != currentKeyStates.end() && it->second;
}

bool InputManager::IsKeyJustPressed(int key) const {
    auto currentIt = currentKeyStates.find(key);
    auto previousIt = previousKeyStates.find(key);
    
    bool currentPressed = (currentIt != currentKeyStates.end() && currentIt->second);
    bool previousPressed = (previousIt != previousKeyStates.end() && previousIt->second);
    
    return currentPressed && !previousPressed;
}

bool InputManager::IsKeyReleased(int key) const {
    auto currentIt = currentKeyStates.find(key);
    auto previousIt = previousKeyStates.find(key);
    
    bool currentPressed = (currentIt != currentKeyStates.end() && currentIt->second);
    bool previousPressed = (previousIt != previousKeyStates.end() && previousIt->second);
    
    return !currentPressed && previousPressed;
}

bool InputManager::IsMouseButtonPressed(int button) const {
    auto it = currentMouseStates.find(button);
    return it != currentMouseStates.end() && it->second;
}

void InputManager::GetMousePosition(double& x, double& y) const {
    if (window) {
        glfwGetCursorPos(window, &x, &y);
    } else {
        x = y = 0.0;
    }
}

void InputManager::UpdateKeyStates() {
    previousKeyStates = currentKeyStates;
    
    // Check common keys used in the game
    int keysToCheck[] = {
        GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
        GLFW_KEY_SPACE, GLFW_KEY_ENTER, GLFW_KEY_ESCAPE,
        GLFW_KEY_P, GLFW_KEY_R
    };
    
    for (int key : keysToCheck) {
        currentKeyStates[key] = (glfwGetKey(window, key) == GLFW_PRESS);
    }
}

void InputManager::UpdateMouseStates() {
    previousMouseStates = currentMouseStates;
    
    int buttonsToCheck[] = {
        GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE
    };
    
    for (int button : buttonsToCheck) {
        currentMouseStates[button] = (glfwGetMouseButton(window, button) == GLFW_PRESS);
    }
}