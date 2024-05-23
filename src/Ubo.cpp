#include "Ubo.hpp"

/* --- Ubo --- */
Ubo::Ubo(const std::string& uboName, const size_t size) 
    : m_uboName(uboName), m_offset(0), m_size(size) {
    glGenBuffers(1, &m_uboID);
    allocate(size);
}

Ubo::~Ubo() {
    glDeleteBuffers(1, &m_uboID);
}

void Ubo::uniformBlockBindingPoint(const Shader& shader, const GLuint bindingPoint) {
    GLuint uniformBlockIndex = glGetUniformBlockIndex(shader.id(), m_uboName.c_str());
    glUniformBlockBinding(shader.id(),  uniformBlockIndex, bindingPoint);
}

void Ubo::bindBufferToBindingPoint(const GLuint bindingPoint) {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_uboID);
}

void Ubo::bind() {
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboID);
}

void Ubo::unbind() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Ubo::allocate(const size_t size) {
    bind();
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    unbind();
}

void Ubo::flush() {
    m_offset = 0;
}
/* --- --- ---  --- */
