#include <glm/gtc/type_ptr.hpp>

template<typename T>
void Ubo::addData(const T& data) {
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, m_offset, sizeof(T), glm::value_ptr(data));
    unbind();
    m_offset = updateOffset<T>();
    /* TODO: there is a little trick here, I set the m_size to sizeof(a struct of data) directly, 
    not follow the std140 layout, so the size and the final layout may different, be careful! 
    And the offset may overflow the size, when vec3/int/float (all not 16) at the end of the struct,
    so the offset should be checked in the future. Or should be careful when setting the struct, need to handle the memory alignment
    */
    // if (m_offset > m_size) {
    //     throw std::runtime_error("Ubo buffer overflow");
    // }
}

template<typename T>
size_t Ubo::getStrideOfType() const {
    if constexpr (std::is_same<T, float>::value || std::is_same<T, unsigned int>::value) {
        return 4;
    } else if constexpr (std::is_same<T, glm::vec3>::value || std::is_same<T, glm::vec4>::value) {
        return 16;
    } else if constexpr (std::is_same<T, glm::mat3>::value) {
        return 16 * 3;  
    } else if constexpr (std::is_same<T, glm::mat4>::value) {
        return 16 * 4;
    } else {
        throw std::runtime_error("Unsupported type");
    }
}

template<typename T>
size_t Ubo::updateOffset() {
    m_offset += getStrideOfType<T>();
    if (m_offset % 16 != 0) {
        m_offset += 16 - (m_offset % 16);
    }
    return m_offset;
}