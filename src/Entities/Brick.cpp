#include "Entities/Brick.h"

Brick::Brick(float x, float y, float width, float height, BrickType brickType)
    : GameObject(x, y, width, height), active(true), type(brickType) {
    
    switch (type) {
        case BrickType::NORMAL:
            hitPoints = 1;
            maxHitPoints = 1;
            color = glm::vec3(0.8f, 0.2f, 0.2f); // Red
            break;
        case BrickType::HARD:
            hitPoints = 2;
            maxHitPoints = 2;
            color = glm::vec3(0.2f, 0.2f, 0.8f); // Blue
            break;
        case BrickType::UNBREAKABLE:
            hitPoints = -1; // Unbreakable
            maxHitPoints = -1;
            color = glm::vec3(0.5f, 0.5f, 0.5f); // Gray
            break;
    }
}

void Brick::Update(float deltaTime) {
    // Bricks are generally static, but this can be extended for special effects
}

void Brick::Reset() {
    active = true;
    hitPoints = maxHitPoints;
    UpdateColorBasedOnHealth();
}

void Brick::Hit() {
    if (type == BrickType::UNBREAKABLE) {
        return; // Cannot be destroyed
    }
    
    hitPoints--;
    if (hitPoints <= 0) {
        active = false;
    } else {
        UpdateColorBasedOnHealth();
    }
}

void Brick::UpdateColorBasedOnHealth() {
    if (!active) {
        return;
    }
    
    switch (type) {
        case BrickType::NORMAL:
            color = glm::vec3(0.8f, 0.2f, 0.2f); // Red
            break;
        case BrickType::HARD:
            if (hitPoints == 2) {
                color = glm::vec3(0.2f, 0.2f, 0.8f); // Blue
            } else {
                color = glm::vec3(0.4f, 0.4f, 0.8f); // Lighter blue
            }
            break;
        case BrickType::UNBREAKABLE:
            color = glm::vec3(0.5f, 0.5f, 0.5f); // Gray
            break;
    }
}