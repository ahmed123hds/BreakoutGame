#include "Physics/PhysicsEngine.h"
#include "Entities/Ball.h"
#include "Entities/Paddle.h"
#include "Entities/Brick.h"
#include <algorithm>
#include <cmath>

PhysicsEngine::PhysicsEngine() : gravity(0.0f, 0.0f) {
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::Update(float deltaTime) {
    // Main physics update - this will be called by the Game class
    // Individual collision checks and responses are handled by specific methods
}

bool PhysicsEngine::CheckCollision(const Ball& ball, const Paddle& paddle) {
    glm::vec2 ballCenter = ball.GetPosition(); // Ball position is already center
    return CircleRect(ballCenter, ball.GetRadius(), paddle.GetPosition(), paddle.GetSize());
}

bool PhysicsEngine::CheckCollision(const Ball& ball, const Brick& brick) {
    if (!brick.IsActive()) {
        return false;
    }
    
    glm::vec2 ballCenter = ball.GetPosition(); // Ball position is already center
    return CircleRect(ballCenter, ball.GetRadius(), brick.GetPosition(), brick.GetSize());
}

bool PhysicsEngine::CheckBoundaryCollision(const Ball& ball, int screenWidth, int screenHeight) {
    glm::vec2 ballPos = ball.GetPosition(); // Ball position is center
    float radius = ball.GetRadius();
    
    // Check only left, right, and top walls - NOT the bottom (floor)
    return (ballPos.x <= radius || ballPos.x >= screenWidth - radius ||
            ballPos.y <= radius);
}

void PhysicsEngine::HandlePaddleCollision(Ball& ball, const Paddle& paddle) {
    glm::vec2 ballCenter = ball.GetPosition(); // Ball position is already center
    glm::vec2 paddleCenter = paddle.GetPosition() + paddle.GetSize() * 0.5f;
    
    // Calculate relative position of ball to paddle center
    float relativeIntersectX = ballCenter.x - paddleCenter.x;
    float normalizedIntersectX = relativeIntersectX / (paddle.GetWidth() * 0.5f);
    
    // Clamp to prevent extreme angles
    normalizedIntersectX = std::max(-1.0f, std::min(1.0f, normalizedIntersectX));
    
    // Calculate bounce angle (max 45 degrees)
    float bounceAngle = normalizedIntersectX * glm::radians(45.0f);
    
    // Get current speed and apply new direction
    float speed = glm::length(ball.GetVelocity());
    glm::vec2 newVelocity;
    newVelocity.x = speed * sin(bounceAngle);
    newVelocity.y = -abs(speed * cos(bounceAngle)); // Always bounce upward
    
    ball.SetVelocity(newVelocity);
    
    // Move ball out of paddle to prevent sticking
    glm::vec2 ballPos = ball.GetPosition();
    ballPos.y = paddle.GetY() - ball.GetRadius() * 2.0f;
    ball.SetPosition(ballPos);
}

void PhysicsEngine::HandleBrickCollision(Ball& ball, Brick& brick) {
    if (!brick.IsActive()) return;
    
    glm::vec2 ballCenter = ball.GetPosition(); // Ball position is already center
    glm::vec2 brickCenter = brick.GetPosition() + brick.GetSize() * 0.5f;
    
    // Calculate collision normal based on closest point
    glm::vec2 closest = glm::vec2(
        std::max(brick.GetX(), std::min(ballCenter.x, brick.GetX() + brick.GetWidth())),
        std::max(brick.GetY(), std::min(ballCenter.y, brick.GetY() + brick.GetHeight()))
    );
    
    glm::vec2 difference = ballCenter - closest;
    glm::vec2 normal = glm::normalize(difference);
    
    // Reflect velocity
    glm::vec2 velocity = ball.GetVelocity();
    glm::vec2 newVelocity = velocity - 2.0f * glm::dot(velocity, normal) * normal;
    ball.SetVelocity(newVelocity);
    
    // Move ball out of brick to prevent multiple collisions
    float penetration = ball.GetRadius() - glm::length(difference);
    if (penetration > 0) {
        glm::vec2 ballPos = ball.GetPosition();
        ballPos += normal * penetration;
        ball.SetPosition(ballPos);
    }
    
    // Damage the brick
    brick.Hit();
}

void PhysicsEngine::HandleBoundaryCollision(Ball& ball, int screenWidth, int screenHeight) {
    glm::vec2 ballPos = ball.GetPosition(); // Ball position is center
    glm::vec2 velocity = ball.GetVelocity();
    float radius = ball.GetRadius();
    bool collided = false;
    
    // Left wall collision - ball center hits left edge + radius
    if (ballPos.x <= radius) {
        velocity.x = abs(velocity.x); // Bounce right
        ballPos.x = radius; // Keep ball inside bounds
        collided = true;
    }
    
    // Right wall collision - ball center hits right edge - radius
    if (ballPos.x >= screenWidth - radius) {
        velocity.x = -abs(velocity.x); // Bounce left
        ballPos.x = screenWidth - radius; // Keep ball inside bounds
        collided = true;
    }
    
    // Top wall collision - ball center hits top edge + radius
    if (ballPos.y <= radius) {
        velocity.y = abs(velocity.y); // Bounce down
        ballPos.y = radius; // Keep ball inside bounds
        collided = true;
    }
    
    // Bottom wall collision - DO NOT bounce, let the ball fall through
    // Game logic will handle game over when ball goes below screen
    
    if (collided) {
        ball.SetVelocity(velocity);
        ball.SetPosition(ballPos);
    }
}

bool PhysicsEngine::AABB(const glm::vec2& pos1, const glm::vec2& size1, 
                        const glm::vec2& pos2, const glm::vec2& size2) {
    return (pos1.x < pos2.x + size2.x &&
            pos1.x + size1.x > pos2.x &&
            pos1.y < pos2.y + size2.y &&
            pos1.y + size1.y > pos2.y);
}

bool PhysicsEngine::CircleRect(const glm::vec2& center, float radius,
                              const glm::vec2& rectPos, const glm::vec2& rectSize) {
    glm::vec2 closest = glm::vec2(
        std::max(rectPos.x, std::min(center.x, rectPos.x + rectSize.x)),
        std::max(rectPos.y, std::min(center.y, rectPos.y + rectSize.y))
    );
    
    glm::vec2 difference = center - closest;
    return glm::length(difference) < radius;
}

glm::vec2 PhysicsEngine::CalculateReflection(const glm::vec2& velocity, const glm::vec2& normal) {
    return velocity - 2.0f * glm::dot(velocity, normal) * normal;
}

float PhysicsEngine::CalculateDistance(const glm::vec2& point1, const glm::vec2& point2) {
    return glm::length(point2 - point1);
}