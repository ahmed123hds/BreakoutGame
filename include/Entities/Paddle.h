#pragma once

#include "GameObject.h"

class Paddle : public GameObject {
public:
    Paddle(float x = 400.0f, float y = 550.0f, float width = 100.0f, float height = 20.0f);

    void Update(float deltaTime) override;
    void Reset() override;

    void MoveLeft();
    void MoveRight();
    void ConstrainToScreen(int screenWidth);

    float GetSpeed() const { return speed; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }

private:
    float speed;
    float initialX, initialY;
    bool movingLeft;
    bool movingRight;
};