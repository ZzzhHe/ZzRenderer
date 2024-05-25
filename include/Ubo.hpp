#pragma once

#include "Light.hpp"

#include <type_traits>

class Ubo {
public:
    Ubo(const std::string& uboName,  const size_t size);
    ~Ubo();

    void uniformBlockBindingPoint(const Shader& shader, const GLuint bindingPoint);
    void bindBufferToBindingPoint(const GLuint bindingPoint);
    template<typename T>
    void addData(const T& data);
    void flush();

private:
    template<typename T>
    size_t getStrideOfType() const;
    template<typename T>
    size_t updateOffset();
    void bind();
    void allocate(const size_t size);
    void unbind();
    
private:
    GLuint m_uboID;
    const std::string m_uboName;
    size_t m_offset;
    size_t m_size;
};

#include "Ubo.tpp" 