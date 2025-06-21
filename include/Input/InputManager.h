#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class InputManager {
public:
    InputManager();
    ~InputManager();

    void Initialize(GLFWwindow* window);
    void Update();

    bool IsKeyPressed(int key) const;
    bool IsKeyJustPressed(int key) const;
    bool IsKeyReleased(int key) const;

    // Mouse input (for future features)
    bool IsMouseButtonPressed(int button) const;
    void GetMousePosition(double& x, double& y) const;

private:
    GLFWwindow* window;
    std::unordered_map<int, bool> currentKeyStates;
    std::unordered_map<int, bool> previousKeyStates;
    std::unordered_map<int, bool> currentMouseStates;
    std::unordered_map<int, bool> previousMouseStates;

    void UpdateKeyStates();
    void UpdateMouseStates();
};