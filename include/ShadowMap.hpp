#pragma once

#include "Light.hpp"
#include "Model.hpp"
#include "Camera.hpp"

class ShadowMap {
public:
    ShadowMap(int width, int height, LightType lightType);
    ShadowMap(const unsigned int width, const unsigned int height, const unsigned int shadowCascadeLevelCount, const std::vector<float>& shadowCascadeLevels, LightType lightType); // Cascade shadow map
    ~ShadowMap();

    void setupDirectLight(const std::shared_ptr<DirectLight>& directLight);
    void setup(const std::shared_ptr<Camera>& camera, const std::shared_ptr<DirectLight>& directLight);
	
	void attachTexture();
    void attachTexture(const unsigned int shadowCascadeLevelCount); // Cascade shadow map
	
    void bindFramebuffer();
    void unbindFramebuffer();

    void setShader(std::shared_ptr<Shader> shader);

    void render(std::unordered_map<uint32_t, std::shared_ptr<Model>>& models, ShadowUniform& uniformData);
    void render_cascade(std::unordered_map<uint32_t, std::shared_ptr<Model>>& models, ShadowUniform& uniformData);
    
    
	std::shared_ptr<Texture> getDepthMapTexture() const;
    const std::vector<glm::mat4>& getLightSpaceMatrices() const;
	void getLightSpaceMatrices_Cascade();
	
	
    std::shared_ptr<Shader> getShader() const { return m_shader; }
	int getCascadeLevelsCount() const {return m_shadowCascadeLevelCount; }
	std::vector<float> getCascadeLevels() {return m_shadowCascadeLevels; }

private:
    glm::mat4 getLightSpaceMatrix(float nearPlane, float farPlane) const ;
    std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) const;
private:
	GLuint m_shadowFBO;
	unsigned int m_width, m_height;
    std::shared_ptr<Texture> m_texture;
    GLuint m_depthMapId;
    LightType m_lightType;
    std::vector<glm::mat4> m_lightSpaceMatrices;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<DirectLight> m_directLight;
    std::shared_ptr<Camera> m_camera;

    // Cascade shadow map
    unsigned int m_shadowCascadeLevelCount;
    std::vector<float> m_shadowCascadeLevels;
};
