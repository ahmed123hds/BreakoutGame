#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Entities/Brick.h"
#include <iostream>
#include <string>

const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
}
)";

const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

Renderer::Renderer(int width, int height) 
    : screenWidth(width), screenHeight(height), quadVAO(0), quadVBO(0) {
    projectionMatrix = glm::ortho(0.0f, static_cast<float>(width), 
                                 static_cast<float>(height), 0.0f, 
                                 -1.0f, 1.0f);
}

Renderer::~Renderer() {
    if (quadVAO != 0) {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }
}

bool Renderer::Initialize() {
    shader = std::make_unique<Shader>();
    if (!shader->LoadFromSource(vertexShaderSource, fragmentShaderSource)) {
        std::cout << "Failed to load shader" << std::endl;
        return false;
    }

    InitializeQuad();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return true;
}

void Renderer::BeginFrame() {
    Clear(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::EndFrame() {
    // Frame ending operations if needed
}

void Renderer::RenderPaddle(const Paddle& paddle) {
    RenderQuad(paddle.GetPosition(), paddle.GetSize(), paddle.GetColor());
}

void Renderer::RenderBall(const Ball& ball) {
    RenderCircle(ball.GetPosition(), ball.GetRadius(), ball.GetColor());
}

void Renderer::RenderBrick(const Brick& brick) {
    RenderQuad(brick.GetPosition(), brick.GetSize(), brick.GetColor());
}

void Renderer::SetViewport(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    projectionMatrix = glm::ortho(0.0f, static_cast<float>(width), 
                                 static_cast<float>(height), 0.0f, 
                                 -1.0f, 1.0f);
}

void Renderer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::InitializeQuad() {
    float vertices[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color) {
    shader->Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    shader->SetMat4("model", model);
    shader->SetMat4("projection", projectionMatrix);
    shader->SetVec3("color", color);
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::RenderCircle(const glm::vec2& position, float radius, const glm::vec3& color) {
    // For simplicity, render circle as a square for now
    // You can implement proper circle rendering later
    glm::vec2 size(radius * 2.0f, radius * 2.0f);
    glm::vec2 adjustedPos(position.x - radius, position.y - radius);
    RenderQuad(adjustedPos, size, color);
}

void Renderer::RenderScore(int score, int gameState) {
    // Make score much more visible with larger, brighter text
    glm::vec3 scoreColor(1.0f, 1.0f, 0.0f); // Bright yellow for score
    glm::vec3 labelColor(0.8f, 0.8f, 0.8f); // Light gray for labels
    
    // Render score with background for better visibility
    glm::vec2 scorePos(10, 10);
    float scoreSize = 30;
    
    // Background rectangle for score
    RenderQuad(glm::vec2(5, 5), glm::vec2(200, 40), glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Render "SCORE:" label - smaller and to the left
    RenderText("SCORE:", scorePos, 25, labelColor);
    
    // Render score value - larger and more prominent
    std::string scoreStr = std::to_string(score);
    RenderText(scoreStr, glm::vec2(scorePos.x + 120, scorePos.y), scoreSize, scoreColor);
    
    // Render game state messages with backgrounds
    if (gameState == 3) { // GAME_OVER
        glm::vec2 gameOverPos(screenWidth/2 - 120, screenHeight/2 - 20);
        // Background for game over message
        RenderQuad(glm::vec2(gameOverPos.x - 10, gameOverPos.y - 10), glm::vec2(240, 80), glm::vec3(0.0f, 0.0f, 0.0f));
        
        RenderText("GAME OVER", gameOverPos, 40, glm::vec3(1.0f, 0.2f, 0.2f));
        RenderText("PRESS R TO RESTART", glm::vec2(gameOverPos.x, gameOverPos.y + 50), 25, labelColor);
    } else if (gameState == 4) { // WIN
        glm::vec2 winPos(screenWidth/2 - 80, screenHeight/2 - 20);
        // Background for win message
        RenderQuad(glm::vec2(winPos.x - 10, winPos.y - 10), glm::vec2(160, 80), glm::vec3(0.0f, 0.0f, 0.0f));
        
        RenderText("YOU WIN!", winPos, 40, glm::vec3(0.2f, 1.0f, 0.2f));
        RenderText("PRESS R TO RESTART", glm::vec2(winPos.x - 40, winPos.y + 50), 25, labelColor);
    }
}

void Renderer::RenderDigit(int digit, const glm::vec2& position, float size, const glm::vec3& color) {
    // Simple 7-segment display style digit rendering using rectangles
    float segmentWidth = size * 0.8f;
    float segmentHeight = size * 0.3f; // Make segments taller
    float thickness = size * 0.15f; // Make segments thicker
    
    // Define which segments are on for each digit (7-segment display)
    bool segments[10][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };
    
    if (digit < 0 || digit > 9) return;
    
    // Render segments
    if (segments[digit][0]) // top
        RenderQuad(glm::vec2(position.x, position.y), glm::vec2(segmentWidth, thickness), color);
    if (segments[digit][1]) // top right
        RenderQuad(glm::vec2(position.x + segmentWidth - thickness, position.y), glm::vec2(thickness, segmentHeight), color);
    if (segments[digit][2]) // bottom right
        RenderQuad(glm::vec2(position.x + segmentWidth - thickness, position.y + segmentHeight), glm::vec2(thickness, segmentHeight), color);
    if (segments[digit][3]) // bottom
        RenderQuad(glm::vec2(position.x, position.y + size - thickness), glm::vec2(segmentWidth, thickness), color);
    if (segments[digit][4]) // bottom left
        RenderQuad(glm::vec2(position.x, position.y + segmentHeight), glm::vec2(thickness, segmentHeight), color);
    if (segments[digit][5]) // top left
        RenderQuad(glm::vec2(position.x, position.y), glm::vec2(thickness, segmentHeight), color);
    if (segments[digit][6]) // middle
        RenderQuad(glm::vec2(position.x, position.y + segmentHeight), glm::vec2(segmentWidth, thickness), color);
}

void Renderer::RenderText(const std::string& text, const glm::vec2& position, float size, const glm::vec3& color) {
    float charSpacing = size * 0.8f;
    glm::vec2 currentPos = position;
    
    for (char c : text) {
        if (c >= '0' && c <= '9') {
            RenderDigit(c - '0', currentPos, size, color);
        } else if (c == ' ') {
            // Space character
        } else {
            // For letters, render as simple rectangles (very basic)
            RenderQuad(currentPos, glm::vec2(size * 0.6f, size), color * 0.7f);
        }
        currentPos.x += charSpacing;
    }
}