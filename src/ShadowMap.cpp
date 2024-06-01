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

ShadowMap::ShadowMap(const unsigned int width, const unsigned int height, const unsigned int shadowCascadeLevelCount, const std::vector<float>& shadowCascadeLevels, LightType lightType)
	: m_width(width), m_height(height), m_lightType(LightType::Direct), m_shadowCascadeLevelCount(shadowCascadeLevelCount), m_shadowCascadeLevels(shadowCascadeLevels) {
	GLCall(glGenFramebuffers(1, &m_shadowFBO));

	bindFramebuffer();
	attachTexture(shadowCascadeLevelCount);
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));

	GLCall(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Framebuffer is not complete!");
	}

	unbindFramebuffer();
}

ShadowMap::~ShadowMap() {
	GLCall(glDeleteFramebuffers(1, &m_shadowFBO));
}

void ShadowMap::setupDirectLight(const std::shared_ptr<DirectLight>& directLight) {
    glm::vec3 lightPos = directLight->direction * -6.0f; // TODO : not sure about this
    glm::mat4 lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.1f, 100.0f);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (m_lightSpaceMatrices.size() == 0) {
		m_lightSpaceMatrices.push_back(lightProjection * lightView);
	} else {
		m_lightSpaceMatrices[0] = lightProjection * lightView;
	}
}

void ShadowMap::setup(const std::shared_ptr<Camera>& camera, const std::shared_ptr<DirectLight>& directLight) {
    m_camera = camera;
    m_directLight = directLight;
    m_lightSpaceMatrices = getLightSpaceMatrices_Cascade();
}


void ShadowMap::render(std::unordered_map<uint32_t, std::shared_ptr<Model>>& models, ShadowUniform& uniformData) {
	bindFramebuffer();
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	GLCall(glCullFace(GL_FRONT));
    uniformData.lightSpaceMatrices = m_lightSpaceMatrices;
	for (auto& kv: models) {
		auto& model = kv.second;
		model->setShader(m_shader);
		model->render(uniformData);
	}
	GLCall(glCullFace(GL_BACK));
	unbindFramebuffer();
}

// void ShadowMap::render_cascade(std::unordered_map<uint32_t, std::shared_ptr<Model>>& models, ShadowUniform& uniformData) {
// 	bindFramebuffer();
// 	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
// 	GLCall(glCullFace(GL_FRONT));
//     uniformData.lightSpaceMatrices = getLightSpaceMatrices_Cascade();
// 	for (auto& kv: models) {
// 		auto& model = kv.second;
// 		model->setShader(m_shader);
// 		model->render(uniformData);
// 	}
// 	GLCall(glCullFace(GL_BACK));
// 	unbindFramebuffer();
// }

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
	m_depthMapId = m_texture->id();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMapId, 0));
}

void ShadowMap::attachTexture(const unsigned int shadowCascadeLevelCount) {
	m_texture = std::make_shared<Texture>(m_width, m_height, m_shadowCascadeLevelCount, TextureType::Array_2D);
	m_depthMapId = m_texture->id();
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthMapId, 0));
}

std::vector<glm::vec4> ShadowMap::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) const {
	const auto inv = glm::inverse(proj * view);
    
    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x) {
        for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int z = 0; z < 2; ++z) {
                const glm::vec4 pt = 
                    inv * glm::vec4(
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
	}
}

// TODO: proj, view, near, far and light direction
glm::mat4 ShadowMap::getLightSpaceMatrix(float near, float far) const  {
    const auto projection = glm::perspective(ZOOM, m_camera->getAspectRatio(), near, far);
    
    const auto corners = getFrustumCornersWorldSpace(projection, m_camera->getViewMatrix());

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : corners) {
        center += glm::vec3(v);
    }
    center /= corners.size();

    const auto lightView = glm::lookAt(center + m_directLight->direction, center, glm::vec3(0.0f, 1.0f, 0.0f));

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : corners) {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 10.0f;
    if (minZ < 0) {
        minZ *= zMult;
    } else {
        minZ /= zMult;
    }

    if (maxZ < 0) {
        maxZ /= zMult;
    } else {
        maxZ *= zMult;
    }

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
    return lightProjection * lightView;
}


// TODO: shadowCascadeLevels save the near and far plane of each cascade

const std::vector<glm::mat4>& ShadowMap::getLightSpaceMatrices_Cascade() const {

    std::vector<glm::mat4> ret;
    for (size_t i = 0; i < m_shadowCascadeLevels.size() + 1; ++i) {
        if (i == 0) {
            ret.push_back(getLightSpaceMatrix(0.1f, m_shadowCascadeLevels[i]));
        } else if (i < m_shadowCascadeLevels.size()) {
            ret.push_back(getLightSpaceMatrix(m_shadowCascadeLevels[i - 1], m_shadowCascadeLevels[i]));
        } else {
            ret.push_back(getLightSpaceMatrix(m_shadowCascadeLevels[i - 1], 100.0f));
        }
    }
    return ret;
}
