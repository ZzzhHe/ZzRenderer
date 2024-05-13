#include "Mesh.hpp"
#include "GLDebug.hpp"

#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures){

    this->m_VAO = new VertexArray();

    this->m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
    
    this->m_IBO = new IndexBuffer(&indices[0], indices.size());

    VertexBufferLayout layout;
    layout.Push<float>(3); // Position
    layout.Push<float>(2); // TexCoords
    layout.Push<float>(3); // Normal

    this->m_VAO->AddBuffer(*this->m_VBO, layout);
}

Mesh::~Mesh() {
    delete this->m_VAO;
    delete this->m_VBO;
    delete this->m_IBO;
}

void Mesh::Render(Shader* shader) {
    shader->use();
    this->m_VAO->Bind();
    this->m_IBO->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, this->m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
    shader->unUse();
}
