#pragma once

#include <memory>
#include <vector>
#include "Rendering/Renderer.h"
#include "Input/InputManager.h"
#include "Physics/PhysicsEngine.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Entities/Brick.h"

enum class GameState {
    MENU,
    ACTIVE,
    PAUSED,
    GAME_OVER,
    WIN
};

class Game {
public:
    Game(int width, int height);
    ~Game();

    void Initialize();
    void Update(float deltaTime);
    void Render();
    void ProcessInput();
    void Cleanup();

    bool ShouldClose() const;
    GameState GetState() const { return state; }
    void SetState(GameState newState) { state = newState; }
    
    // Scoring system
    int GetScore() const { return score; }
    void AddScore(int points) { score += points; }

private:
    int screenWidth;
    int screenHeight;
    GameState state;
    
    // Game stats
    int score;
    float gameTime;
    float speedMultiplier;
    
    // Input state tracking (moved from static to member variables)
    bool spacePressed;
    bool rPressed;

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<PhysicsEngine> physicsEngine;
    std::unique_ptr<Paddle> paddle;
    std::unique_ptr<Ball> ball;
    std::vector<std::unique_ptr<Brick>> bricks;

    void InitializeLevel();
    void CheckCollisions();
    void UpdateGameLogic(float deltaTime);
    void ResetLevel();
};