#pragma once

#include <glm/glm.hpp>

class GameObject {
public:
    GameObject(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f);
    virtual ~GameObject() = default;

    virtual void Update(float deltaTime) {}
    virtual void Reset() {}

    // Getters
    float GetX() const { return position.x; }
    float GetY() const { return position.y; }
    float GetWidth() const { return size.x; }
    float GetHeight() const { return size.y; }
    glm::vec2 GetPosition() const { return position; }
    glm::vec2 GetSize() const { return size; }
    glm::vec3 GetColor() const { return color; }

    // Setters
    void SetPosition(float x, float y) { position = glm::vec2(x, y); }
    void SetPosition(const glm::vec2& pos) { position = pos; }
    void SetSize(float width, float height) { size = glm::vec2(width, height); }
    void SetSize(const glm::vec2& newSize) { size = newSize; }
    void SetColor(const glm::vec3& newColor) { color = newColor; }

protected:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
};