#pragma once

#include <Framebuffer.hpp>

class Bloom {
public:
    Bloom(const unsigned int width, const unsigned int height);
    ~Bloom();

    void init();
    void applyBloomEffect(std::shared_ptr<Framebuffer> sceneFBO);
    std::shared_ptr<Framebuffer> getBloomFBO() const;   
    void renderBloomFBO(std::shared_ptr<Framebuffer> sceneFBO);

private:
    unsigned int m_width, m_height;
    std::unordered_map<std::string, std::shared_ptr<Framebuffer>> m_fbos;
    std::unordered_map<std::string,  std::shared_ptr<Shader>> m_shader;

    void extractBrightAreas(std::shared_ptr<Framebuffer> sceneFBO);
    void applyBlur();
    void combineTextures();
};  
