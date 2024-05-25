#include "ShadowMap.hpp"


#include <glm/gtc/matrix_transform.hpp>

ShadowMap::ShadowMap(int width, int height, LightType lightType) 
    : m_lightType(lightType) {
    m_framebuffer = std::make_shared<Framebuffer>(width, height, "ShadowMap");
    m_framebuffer->bind();

    if(m_lightType == LightType::Direct) {
        m_framebuffer->attachShadowTexture();
    } else {
        // TODO point light shadow map
        throw std::runtime_error("Point light shadow map not implemented yet");
    }

    if (!m_framebuffer->checkStatus()) {
		throw std::runtime_error("Framebuffer is not complete!");
	}

	m_framebuffer->unbind();
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::setupDirectLight(const std::shared_ptr<DirectLight>& directLight) {
    glm::vec3 lightPos = directLight->direction * -5.0f; // TODO : not sure about this
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_lightSpaceMatrices.push_back(lightProjection * lightView);
}

void ShadowMap::render(std::shared_ptr<Model> model, ShadowUniform& uniformData) {
    m_framebuffer->bind(); // TODO: not use framebuffer class 
    uniformData.lightSpaceMatrix = m_lightSpaceMatrices[0];
    model->setShader(m_shader);
    model->render(uniformData);
    m_framebuffer->unbind();
}

void ShadowMap::bindFramebuffer() {
    m_framebuffer->bind();
}

void ShadowMap::unbindFramebuffer() {
    m_framebuffer->unbind();
}

GLuint ShadowMap::getDepthMap() const {
    return m_framebuffer->getTextureId();
}

const std::vector<glm::mat4>& ShadowMap::getLightSpaceMatrices() const {
    return m_lightSpaceMatrices;
}

void ShadowMap::setShader(std::shared_ptr<Shader> shader) {
    m_shader = shader;
}