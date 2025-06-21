#pragma once

#include "GameObject.h"

enum class BrickType {
    NORMAL,
    HARD,
    UNBREAKABLE
};

class Brick : public GameObject {
public:
    Brick(float x, float y, float width = 60.0f, float height = 20.0f, 
          BrickType type = BrickType::NORMAL);

    void Update(float deltaTime) override;
    void Reset() override;

    // Brick-specific methods
    bool IsActive() const { return active; }
    void SetActive(bool isActive) { active = isActive; }
    void Hit();
    
    BrickType GetType() const { return type; }
    int GetHitPoints() const { return hitPoints; }
    bool IsDestroyed() const { return !active; }

private:
    bool active;
    BrickType type;
    int hitPoints;
    int maxHitPoints;
    
    void UpdateColorBasedOnHealth();
};