#include "Framebuffer.hpp"
#include "GLDebug.hpp"

Framebuffer::Framebuffer(const unsigned int width, const unsigned int height, const std::string name) 
: m_width(width), m_height(height), m_name(name) {
    GLCall(glGenFramebuffers(1, &m_fbo));

    // set vbo
     std::vector<float> quadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    this->m_VAO = new VertexArray();

    this->m_VBO = new VertexBuffer(&quadVertices[0], quadVertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2); // Position
    layout.push<float>(2); // TexCoords

    this->m_VAO->addBuffer(*this->m_VBO, layout);

}

Framebuffer::~Framebuffer() {
    delete m_VAO;
    delete m_VBO;
    
	if (m_rbo != 0) {
		GLCall(glDeleteRenderbuffers(1, &m_rbo));
	}
	
    GLCall(glDeleteFramebuffers(1, &m_fbo));
}

void Framebuffer::bind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void Framebuffer::unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::setShader(std::shared_ptr<Shader> shader) {
    m_shader = shader;
}

void Framebuffer::attachTexture() {
    // generate Texture
	m_texture = std::make_shared<Texture>(m_width, m_height, TextureType::FRAMEBUFFER);
    // attach
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->id(), 0));
}

void Framebuffer::attachShadowTexture() {
    // generate Texture
    m_texture = std::make_shared<Texture>(m_width, m_height, TextureType::SHADOW_MAP);
    // attach
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture->id(), 0));
}

void Framebuffer::attachRenderBuffer() {
    // generate RenderBuffer
    GLCall(glGenRenderbuffers(1, &m_rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height));
    
	// attach
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));
}

void Framebuffer::attachDepthBuffer() {
    // generate RenderBuffer
    GLCall(glGenRenderbuffers(1, &m_rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height));
    
    // attach
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo));
    // Disable color buffer
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));
}

bool Framebuffer::checkStatus() {
    GLCall(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    return true;
}

void Framebuffer::render() {
    if (m_shader == nullptr) {
        throw std::runtime_error("Framebuffer shader is not set!");
    }
    m_shader->use();
    m_shader->setInt(m_name.c_str(), 0);

    if (m_texture->id() == 0) {
        throw std::runtime_error("Framebuffer texture is not attached!");
    }

	m_texture->bind();

    this->m_VAO->bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

std::shared_ptr<Texture> Framebuffer::getTexture() const {
    return m_texture;
}

