#include "Entities/Ball.h"

Ball::Ball(float x, float y, float radius)
    : GameObject(x, y, radius * 2.0f, radius * 2.0f), 
      velocity(100.0f, -350.0f), initialPosition(x, y), 
      initialVelocity(100.0f, -350.0f), stuck(false) {
    color = glm::vec3(1.0f, 1.0f, 1.0f); // White color
}

void Ball::Update(float deltaTime) {
    if (!stuck) {
        // Position update will be handled by Physics module
        // This is just a placeholder for the interface
        position += velocity * deltaTime;
    }
}

void Ball::Reset() {
    position = initialPosition;
    velocity = initialVelocity;
    stuck = false;
}