#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    bool LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    void Use() const;
    void Delete();

    // Uniform setting methods
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

    GLuint GetID() const { return ID; }

private:
    GLuint ID;
    
    bool CompileShader(GLuint shader, const std::string& source);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader);
    void CheckCompileErrors(GLuint shader, const std::string& type);
    std::string ReadFile(const std::string& filePath);
};