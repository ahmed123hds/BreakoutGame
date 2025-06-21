#include "Core/Game.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Game::Game(int width, int height) 
    : screenWidth(width), screenHeight(height), state(GameState::MENU),
      score(0), gameTime(0.0f), speedMultiplier(1.0f),
      spacePressed(false), rPressed(false),
      renderer(nullptr), inputManager(nullptr), physicsEngine(nullptr),
      paddle(nullptr), ball(nullptr) {
    // Explicitly initialize all pointers to nullptr
    bricks.clear();
}

Game::~Game() {
    Cleanup();
}

void Game::Initialize() {
    // Only initialize renderer and input manager once
    if (!renderer) {
        renderer = std::make_unique<Renderer>(screenWidth, screenHeight);
        renderer->Initialize();
        inputManager = std::make_unique<InputManager>();
    }
    
    // Always recreate game objects and physics
    physicsEngine = std::make_unique<PhysicsEngine>();
    //paddle = std::make_unique<Paddle>(screenWidth / 2.0f - 50.0f, screenHeight - 50.0f);
    //ball = std::make_unique<Ball>(screenWidth / 2.0f, screenHeight / 2.0f);
    
    // (Re)create core game objects each time we initialize
    paddle = std::make_unique<Paddle>(screenWidth / 2.0f - 50.0f,
                                      screenHeight - 50.0f);
    ball = std::make_unique<Ball>(screenWidth / 2.0f,
                                  screenHeight / 2.0f);

    paddle->Reset();
    ball->Reset();
    
    // Reset ALL game state completely
    score = 0;
    gameTime = 0.0f;
    speedMultiplier = 1.0f;
    spacePressed = false;
    rPressed = false;
    
    // Clear and recreate level
    bricks.clear();
    InitializeLevel();
    
    state = GameState::ACTIVE;
}

void Game::Update(float deltaTime) {
    if (state == GameState::ACTIVE) {
        UpdateGameLogic(deltaTime);
        CheckCollisions();
    }
    // Don't update anything when game is over or won - wait for restart
}

void Game::Render() {
    renderer->BeginFrame();
    
    if (state == GameState::ACTIVE || state == GameState::PAUSED) {
        renderer->RenderPaddle(*paddle);
        renderer->RenderBall(*ball);
        
        for (const auto& brick : bricks) {
            if (brick->IsActive()) {
                renderer->RenderBrick(*brick);
            }
        }
    }
    
    // Always render score and game state messages
    renderer->RenderScore(score, static_cast<int>(state));
    
    renderer->EndFrame();
}

void Game::ProcessInput() {
    // Get current GLFW window context for input checking
    GLFWwindow* window = glfwGetCurrentContext();
    
    if (state == GameState::ACTIVE) {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            paddle->MoveLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            paddle->MoveRight();
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
        spacePressed = true;
        if (state == GameState::MENU) {
            state = GameState::ACTIVE;
        } else if (state == GameState::ACTIVE) {
            state = GameState::PAUSED;
        } else if (state == GameState::PAUSED) {
            state = GameState::ACTIVE;
        }
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        spacePressed = false;
    }
    
    // Restart functionality - SIMPLE APPROACH
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed) {
        rPressed = true;
        if (state == GameState::GAME_OVER || state == GameState::WIN) {
            // Just reinitialize everything
            Initialize();
            // Clear input states after restart
            spacePressed = false;
            rPressed = false;
        }
    } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        rPressed = false;
    }
}

bool Game::ShouldClose() const {
    return glfwWindowShouldClose(glfwGetCurrentContext());
}

void Game::InitializeLevel() {
    bricks.clear();
    
    const int rows = 8;
    const int cols = 12;
    const float brickWidth = 60.0f;
    const float brickHeight = 20.0f;
    const float spacing = 5.0f;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = col * (brickWidth + spacing) + spacing;
            float y = row * (brickHeight + spacing) + spacing + 50;
            
            // Create different brick types based on row
            BrickType type = BrickType::NORMAL;
            if (row == 0 || row == 1) {
                type = BrickType::HARD; // Top 2 rows are hard bricks
            } else if (row == 7 && (col == 0 || col == 11)) {
                type = BrickType::UNBREAKABLE; // Bottom corners are unbreakable
            }
            
            auto brick = std::make_unique<Brick>(x, y, brickWidth, brickHeight, type);
            brick->Reset();
            bricks.push_back(std::move(brick));
        }
    }
}

void Game::CheckCollisions() {
    // Check boundary collisions
    if (physicsEngine->CheckBoundaryCollision(*ball, screenWidth, screenHeight)) {
        physicsEngine->HandleBoundaryCollision(*ball, screenWidth, screenHeight);
    }
    
    // Check paddle collision
    if (physicsEngine->CheckCollision(*ball, *paddle)) {
        physicsEngine->HandlePaddleCollision(*ball, *paddle);
    }
    
    // Check brick collisions
    for (auto& brick : bricks) {
        if (brick->IsActive() && physicsEngine->CheckCollision(*ball, *brick)) {
            // Check if brick was active before collision
            bool wasActive = brick->IsActive();
            physicsEngine->HandleBrickCollision(*ball, *brick);
            
            // Award points if brick was destroyed
            if (wasActive && !brick->IsActive()) {
                // Different points based on brick type
                switch (brick->GetType()) {
                    case BrickType::NORMAL:
                        AddScore(10);
                        break;
                    case BrickType::HARD:
                        AddScore(20);
                        break;
                    case BrickType::UNBREAKABLE:
                        AddScore(50); // Bonus points for hitting unbreakable bricks
                        break;
                }
            } else if (wasActive && brick->IsActive()) {
                // Brick was hit but not destroyed (hard brick)
                AddScore(5);
            }
            
            break; // Only handle one collision per frame to prevent issues
        }
    }
}

void Game::UpdateGameLogic(float deltaTime) {
    // Update game time and speed multiplier - much more gradual increase
    gameTime += deltaTime;
    float oldSpeedMultiplier = speedMultiplier;
    speedMultiplier = 1.0f + (gameTime * 0.02f); // Increase speed by 2% every second
    
    // Cap maximum speed increase
    if (speedMultiplier > 1.5f) {
        speedMultiplier = 1.5f; // Maximum 50% speed increase
    }
    
    // Apply speed increase to ball if multiplier changed significantly
    if (abs(speedMultiplier - oldSpeedMultiplier) > 0.01f) {
        glm::vec2 currentVelocity = ball->GetVelocity();
        float speedIncrease = speedMultiplier / oldSpeedMultiplier;
        ball->SetVelocity(currentVelocity * speedIncrease);
    }
    
    paddle->Update(deltaTime);
    ball->Update(deltaTime);
    
    // Keep paddle within screen bounds
    paddle->ConstrainToScreen(screenWidth);
    
    // Check win condition
    bool allBricksDestroyed = true;
    for (const auto& brick : bricks) {
        if (brick->IsActive()) {
            allBricksDestroyed = false;
            break;
        }
    }
    
    if (allBricksDestroyed) {
        state = GameState::WIN;
    }
    
    // Check lose condition (ball below screen)
    if (ball->GetY() > screenHeight) {
        state = GameState::GAME_OVER;
    }
}


void Game::Cleanup() {
    // Cleanup will be handled by smart pointers automatically
}
