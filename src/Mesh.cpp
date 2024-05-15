#include "Mesh.hpp"
#include "GLDebug.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::shared_ptr<Material> material) 
    : m_material(material){

    this->m_VAO = new VertexArray();

    this->m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
    
    this->m_IBO = new IndexBuffer(&indices[0], indices.size());

    VertexBufferLayout layout;
    layout.push<float>(3); // Position
    layout.push<float>(2); // TexCoords
    layout.push<float>(3); // Normal

    this->m_VAO->addBuffer(*this->m_VBO, layout);
}

Mesh::~Mesh() {
    delete this->m_VAO;
    delete this->m_VBO;
    delete this->m_IBO;
}

void Mesh::render(const SharedUniform& uniform) {
    m_material->apply(uniform);
    this->m_VAO->bind();
    this->m_IBO->bind();
    GLCall(glDrawElements(GL_TRIANGLES, this->m_IBO->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Mesh::setShader(std::shared_ptr<Shader> shader) {
    m_material->shader = shader;
}

void Mesh::setMaterial(std::shared_ptr<Material> material) {
    m_material = material;
}
