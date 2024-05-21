#include <glad/glad.h>

#include "Texture.hpp"
#include "GLDebug.hpp"
#include <iostream>

#include "stb_image/stb_image.h"
#include <cassert>


Texture::Texture(const std::string& path, const TextureType type)
    : m_RendererID(0), m_FilePath(path), m_Type(type), m_Width(0), m_Height(0), m_BPP(4) {
    // flip texture(top left 0) upside down to fit OpenGL's bottom left 0
    stbi_set_flip_vertically_on_load(false);

    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // set wrapping
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    // set filtering
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D,
                    0, 
                    GL_SRGB_ALPHA, // sRGB color space, gamma correction
                    m_Width, 
                    m_Height, 
                    0, 
                    GL_RGBA, 
                    GL_UNSIGNED_BYTE, 
                    m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    glBindTexture(GL_TEXTURE_2D, 0);
    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::Texture(const std::vector<std::string>& paths, TextureType type)
    : m_RendererID(0), m_Type(type), m_Width(0), m_Height(0), m_BPP(0) {
    assert(type == TextureType::CUBE_MAP);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

    for (unsigned int i = 0; i < paths.size(); i++) {
        unsigned char* data = stbi_load(paths[i].c_str(), &m_Width, &m_Height, &m_BPP, 0);
        if (data) {
            GLCall(glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, 
                GL_RGB, 
                m_Width, 
                m_Height, 
                0, 
                GL_RGB, 
                GL_UNSIGNED_BYTE, 
                data));
            stbi_image_free(data);
        } else {
            std::cout << "Failed to load cube map texture at path: " << paths[i] << std::endl;
        }
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(const unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unBind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}