#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;
    
private:
    GLuint m_RendererID;
};
