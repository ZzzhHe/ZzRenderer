#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "Mesh.hpp"

class Framebuffer {
public:
    Framebuffer(const unsigned int width, const unsigned int height, const std::string name);
    ~Framebuffer();

    void attachTexture(TextureType type = TextureType::FRAMEBUFFER);
    void debug_setTexture(std::shared_ptr<Texture> texture);

    void attachRenderBuffer();

    void setShader(std::shared_ptr<Shader> shader);

    void bind();
    void unbind();

    void render();

    bool checkStatus();

	std::shared_ptr<Texture> getTexture() const;

private:
    GLuint m_fbo;
    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
	GLuint m_rbo;
	std::shared_ptr<Texture> m_texture;
    unsigned int m_width, m_height;
    std::shared_ptr<Shader> m_shader;
    const std::string m_name;
};
