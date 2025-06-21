#include "Entities/Paddle.h"

Paddle::Paddle(float x, float y, float width, float height)
    : GameObject(x, y, width, height), speed(500.0f), initialX(x), initialY(y),
      movingLeft(false), movingRight(false) {
    color = glm::vec3(0.2f, 0.8f, 0.2f); // Green color
}

void Paddle::Update(float deltaTime) {
    if (movingLeft) {
        position.x -= speed * deltaTime;
        movingLeft = false;
    }
    if (movingRight) {
        position.x += speed * deltaTime;
        movingRight = false;
    }
}

void Paddle::Reset() {
    position.x = initialX;
    position.y = initialY;
    movingLeft = false;
    movingRight = false;
}

void Paddle::MoveLeft() {
    movingLeft = true;
}

void Paddle::MoveRight() {
    movingRight = true;
}

void Paddle::ConstrainToScreen(int screenWidth) {
    if (position.x < 0) {
        position.x = 0;
    }
    if (position.x + size.x > screenWidth) {
        position.x = screenWidth - size.x;
    }
}