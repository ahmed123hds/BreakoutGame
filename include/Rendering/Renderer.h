#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Paddle;
class Ball;
class Brick;
class Shader;

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool Initialize();
    void BeginFrame();
    void EndFrame();

    void RenderPaddle(const Paddle& paddle);
    void RenderBall(const Ball& ball);
    void RenderBrick(const Brick& brick);
    void RenderScore(int score, int gameState);

    void SetViewport(int width, int height);
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

private:
    int screenWidth, screenHeight;
    glm::mat4 projectionMatrix;
    
    std::unique_ptr<Shader> shader;
    GLuint quadVAO, quadVBO;

    void InitializeQuad();
    void RenderQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
    void RenderCircle(const glm::vec2& position, float radius, const glm::vec3& color);
    void RenderDigit(int digit, const glm::vec2& position, float size, const glm::vec3& color);
    void RenderText(const std::string& text, const glm::vec2& position, float size, const glm::vec3& color);
};