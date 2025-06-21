#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>

class Ball : public GameObject {
public:
    Ball(float x = 400.0f, float y = 300.0f, float radius = 12.5f);

    void Update(float deltaTime) override;
    void Reset() override;

    // Physics interface methods (to be called by Physics module)
    glm::vec2 GetVelocity() const { return velocity; }
    void SetVelocity(const glm::vec2& newVelocity) { velocity = newVelocity; }
    void SetVelocity(float vx, float vy) { velocity = glm::vec2(vx, vy); }

    float GetRadius() const { return size.x / 2.0f; }
    void SetRadius(float radius) { size = glm::vec2(radius * 2.0f); }

    bool IsStuck() const { return stuck; }
    void SetStuck(bool isStuck) { stuck = isStuck; }

private:
    glm::vec2 velocity;
    glm::vec2 initialPosition;
    glm::vec2 initialVelocity;
    bool stuck;
};