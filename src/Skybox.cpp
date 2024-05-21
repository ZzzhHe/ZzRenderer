#include "Skybox.hpp"

Skybox::Skybox(const std::vector<std::string>& faceTextures, std::shared_ptr<Shader> shader) {
    std::vector<float> skyboxVertices =  {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    this->m_VAO = new VertexArray();

    this->m_VBO = new VertexBuffer(&skyboxVertices[0], skyboxVertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(3); // Position

    this->m_VAO->addBuffer(*this->m_VBO, layout);

    loadTextures(faceTextures);
    m_shader = shader;
}

Skybox::~Skybox() {
    delete this->m_VAO;
    delete this->m_VBO;
}

void Skybox::loadTextures(const std::vector<std::string>& faceTextures) {
    m_texture = std::make_shared<Texture>(faceTextures, TextureType::CUBE_MAP);
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
    m_shader->use();
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    m_texture->bind(0);
    m_shader->setInt("skybox", 0);

    this->m_VAO->bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}
