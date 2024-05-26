#include "ShadowMap.hpp"


#include <glm/gtc/matrix_transform.hpp>

ShadowMap::ShadowMap(int width, int height, LightType lightType) 
    : m_width(width), m_height(height), m_lightType(lightType) {
    
	GLCall(glGenFramebuffers(1, &m_shadowFBO));
		
	
	bindFramebuffer();

    if(m_lightType == LightType::Direct) {
		attachTexture();
		GLCall(glDrawBuffer(GL_NONE));
		GLCall(glReadBuffer(GL_NONE));
    } else {
        // TODO point light shadow map
        throw std::runtime_error("Point light shadow map not implemented yet");
    }
		
	GLCall(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Framebuffer is not complete!");
	}

	unbindFramebuffer();
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::setupDirectLight(const std::shared_ptr<DirectLight>& directLight) {
    glm::vec3 lightPos = directLight->direction * -2.0f; // TODO : not sure about this
    glm::mat4 lightProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_lightSpaceMatrices.push_back(lightProjection * lightView);
}



void ShadowMap::render(std::shared_ptr<Model> model, ShadowUniform& uniformData) {
	bindFramebuffer();
    uniformData.lightSpaceMatrix = m_lightSpaceMatrices[0];
    model->setShader(m_shader);
    model->render(uniformData);
	unbindFramebuffer();
}

void ShadowMap::bindFramebuffer() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO));
}

void ShadowMap::unbindFramebuffer() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

std::shared_ptr<Texture> ShadowMap::getDepthMapTexture() const {
	return m_texture;
}

const std::vector<glm::mat4>& ShadowMap::getLightSpaceMatrices() const {
    return m_lightSpaceMatrices;
}

void ShadowMap::setShader(std::shared_ptr<Shader> shader) {
    m_shader = shader;
}

void ShadowMap::attachTexture() {
	m_texture = std::make_shared<Texture>(m_width, m_height, TextureType::SHADOW_MAP);
	m_depthMap = m_texture->id();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0));
}
