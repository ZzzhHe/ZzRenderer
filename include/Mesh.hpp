#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

#include "Texture.hpp"
#include "Shader.hpp"

struct Vertex{
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    // glm::vec3 Tangent;
    // glm::vec3 Bitangent;
};

class Mesh {
public:
    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures = {});
    virtual ~Mesh();

    virtual void Render(Shader *shader);

protected:
    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    IndexBuffer *m_IBO;
    std::vector<std::shared_ptr<Texture>> textures;
};
