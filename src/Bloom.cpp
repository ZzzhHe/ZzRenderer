#include "Bloom.hpp"

Bloom::Bloom(unsigned int width, unsigned int height)
    : m_width(width), m_height(height) {
        init();
    }

Bloom::~Bloom() {}

void Bloom::init() {
    // Initialize shaders
    m_shader["BrightShader"] = std::make_shared<Shader>("shader/bright.vert", "shader/bright.frag");
    m_shader["BlurShader"] = std::make_shared<Shader>("shader/blur.vert", "shader/blur.frag");

    m_shader["BloomShader"] = std::make_shared<Shader>("shader/bloom.vert", "shader/bloom.frag");

    // Initialize framebuffers
    m_fbos["BrightFBO"] = std::make_shared<Framebuffer>(m_width, m_height, "BrightMap");
    m_fbos["BlurFBO1"] = std::make_shared<Framebuffer>(m_width, m_height, "BlurMap");
    m_fbos["BlurFBO2"] = std::make_shared<Framebuffer>(m_width, m_height, "BlurMap");
    // this framebuffer will be used to render to screen
    m_fbos["BloomFBO"] = std::make_shared<Framebuffer>(m_width, m_height, "BloomBlurMap");

    m_fbos["BrightFBO"]->attachTexture();
    m_fbos["BrightFBO"]->setShader(m_shader["BrightShader"]);
    m_fbos["BrightFBO"]->checkStatus();

    m_fbos["BlurFBO1"]->attachTexture();
    m_fbos["BlurFBO1"]->setShader(m_shader["BlurShader"]);
    m_fbos["BlurFBO1"]->checkStatus();

    m_fbos["BlurFBO2"]->attachTexture();
    m_fbos["BlurFBO2"]->setShader(m_shader["BlurShader"]);
    m_fbos["BlurFBO2"]->checkStatus();

    m_fbos["BloomFBO"]->attachTexture();
    m_fbos["BloomFBO"]->setShader(m_shader["BloomShader"]);
    m_fbos["BloomFBO"]->checkStatus();
}

void Bloom::applyBloomEffect(std::shared_ptr<Framebuffer> sceneFBO) {
    extractBrightAreas(sceneFBO);
    applyBlur();
    combineTextures();
}

void Bloom::extractBrightAreas(std::shared_ptr<Framebuffer> sceneFBO) {
    m_fbos["BrightFBO"]->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sceneFBO->render();
    m_fbos["BrightFBO"]->unbind();
}

void Bloom::applyBlur() {
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;

    for (unsigned int i = 0; i < amount; i ++) {
        if (horizontal) {
            m_fbos["BlurFBO1"]->bind();
        } else {
            m_fbos["BlurFBO2"]->bind();
        }

        m_shader["BlurShader"]->use();
        m_shader["BlurShader"]->setInt("horizontal", horizontal);   

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (first_iteration) {
            m_fbos["BrightFBO"]->render();
            first_iteration = false;
        } else {
            if (horizontal) {
                m_fbos["BlurFBO2"]->render();
            } else {
                m_fbos["BlurFBO1"]->render();
            }
        }

        if (horizontal) {
            m_fbos["BlurFBO1"]->unbind();
        } else {
            m_fbos["BlurFBO2"]->unbind();
        }

        horizontal = !horizontal;
    }
    // the final time will be in BlurFBO1
}

void Bloom::combineTextures() {
    // TODO: need to double check the uniform (sample2D) location
    m_fbos["BloomFBO"]->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_fbos["BlurFBO1"]->render();
    m_fbos["BloomFBO"]->unbind();
}

std::shared_ptr<Framebuffer> Bloom::getBloomFBO() const {
    return m_fbos.at("BloomFBO");
}

void Bloom::renderBloomFBO(std::shared_ptr<Framebuffer> sceneFBO) {
    m_shader["BloomShader"]->use();
    sceneFBO->getTexture()->bind(1);
    m_shader["BloomShader"]->setInt("scene", 1);
    m_fbos["BloomFBO"]->render();
}