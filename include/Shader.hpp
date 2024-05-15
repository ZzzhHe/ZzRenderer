#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const std::string &vertpath, const std::string &fragpath);
    ~Shader();

    void use() const;
    void unUse() const;

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setUInt(const std::string &name, unsigned int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, const glm::mat4 &value);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec2(const std::string &name, glm::vec2 &value);
    void setVec4(const std::string &name, const glm::vec4 &value);

private:
    ShaderProgramSource ParseShader(const std::string &vertpath, const std::string &fragpath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    GLuint CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    int GetUniformLocation(const std::string &name);

private:
    GLuint m_rendererID;
    std::string m_vertPath;
    std::string m_fragPath;
    std::unordered_map<std::string, int> m_uniformLocationCache;
};
