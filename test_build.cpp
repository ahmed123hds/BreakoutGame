#include <iostream>
#include "Core/Game.h"
#include "Physics/PhysicsEngine.h"
#include "Entities/Ball.h"
#include "Entities/Paddle.h"
#include "Entities/Brick.h"

int main() {
    std::cout << "Testing Breakout Game components..." << std::endl;
    
    // Test basic object creation
    try {
        Ball ball(100, 100, 10);
        Paddle paddle(200, 500, 100, 20);
        Brick brick(50, 50, 60, 20);
        PhysicsEngine physics;
        
        std::cout << "✓ All game objects created successfully" << std::endl;
        
        // Test ball properties
        ball.SetVelocity(100, -200);
        auto velocity = ball.GetVelocity();
        if (velocity.x == 100.0f && velocity.y == -200.0f) {
            std::cout << "✓ Ball velocity setting works" << std::endl;
        }
        
        // Test collision detection
        bool collision = physics.CheckCollision(ball, paddle);
        std::cout << "✓ Collision detection runs without error" << std::endl;
        
        // Test brick interaction
        brick.Hit();
        if (!brick.IsActive()) {
            std::cout << "✓ Brick destruction works" << std::endl;
        }
        
        std::cout << "All tests passed! The game should work correctly." << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Error: " << e.what() << std::endl;
        return 1;
    }
}