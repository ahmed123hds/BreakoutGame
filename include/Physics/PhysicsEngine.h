#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Ball;
class Paddle;
class Brick;

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    // Main physics update function
    void Update(float deltaTime);

    // Collision detection methods
    bool CheckCollision(const Ball& ball, const Paddle& paddle);
    bool CheckCollision(const Ball& ball, const Brick& brick);
    bool CheckBoundaryCollision(const Ball& ball, int screenWidth, int screenHeight);

    // Collision response methods
    void HandlePaddleCollision(Ball& ball, const Paddle& paddle);
    void HandleBrickCollision(Ball& ball, Brick& brick);
    void HandleBoundaryCollision(Ball& ball, int screenWidth, int screenHeight);

    // Physics properties
    void SetGravity(const glm::vec2& gravity) { this->gravity = gravity; }
    glm::vec2 GetGravity() const { return gravity; }

private:
    glm::vec2 gravity;
    
    // Helper methods for collision detection
    bool AABB(const glm::vec2& pos1, const glm::vec2& size1, 
              const glm::vec2& pos2, const glm::vec2& size2);
    bool CircleRect(const glm::vec2& center, float radius,
                   const glm::vec2& rectPos, const glm::vec2& rectSize);
    
    // Helper methods for collision response
    glm::vec2 CalculateReflection(const glm::vec2& velocity, const glm::vec2& normal);
    float CalculateDistance(const glm::vec2& point1, const glm::vec2& point2);
};