#pragma once

#include "Mesh.hpp"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faceTextures, std::shared_ptr<Shader> shader);
    ~Skybox();
    void render(const glm::mat4& view, const glm::mat4& projection);

    void loadTextures(const std::vector<std::string>& faceTextures);
private:
    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Shader> m_shader;
};