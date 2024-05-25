#pragma once

#include "Light.hpp"
#include "Framebuffer.hpp"
#include "Model.hpp"

class ShadowMap {
public:
    ShadowMap(int width, int height, LightType lightType);
    ~ShadowMap();

    void setupDirectLight(const std::shared_ptr<DirectLight>& directLight);

    void bindFramebuffer();
    void unbindFramebuffer();

    void setShader(std::shared_ptr<Shader> shader);

    void render(std::shared_ptr<Model> model, ShadowUniform& uniformData);

	std::shared_ptr<Texture> getDepthMapTexture() const;
    const std::vector<glm::mat4>& getLightSpaceMatrices() const;
    std::shared_ptr<Shader> getShader() const { return m_shader; }

private:
    std::shared_ptr<Framebuffer> m_framebuffer;
    unsigned int m_depthMap;
    LightType m_lightType;
    std::vector<glm::mat4> m_lightSpaceMatrices;
    std::shared_ptr<Shader> m_shader;
};
