#pragma once

#include "Model.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Gui.hpp"
#include "Skybox.hpp"
#include "Ubo.hpp"
#include "Framebuffer.hpp"
#include "ShadowMap.hpp"
#include "Bloom.hpp"

#include <unordered_map>
#include <memory>

class Application {
public:
    static const int WIDTH;
    static const int HEIGHT;

    Application();
    ~Application();
    void run();

    using model_id_t = uint32_t;
    using model_map_t = std::unordered_map<model_id_t, std::shared_ptr<Model>>;

private:
    void loadRenderObjects();

private:
    Window m_window{WIDTH, HEIGHT, "OpenGL"};
    Renderer m_renderer{};
    Gui m_gui{m_window};
	
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<CameraController> m_cameraController;

    std::unordered_map<std::string, std::shared_ptr<Light>> m_lights;  
	
	SharedUniform m_uniform;
    std::unordered_map<std::string, std::shared_ptr<Ubo>> m_ubos;
	
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	
	std::shared_ptr<Skybox> m_skybox;
    
    model_map_t m_models; // will be replaced by a scene graph(render object)
    static model_id_t m_current_id;

    std::unordered_map<std::string, std::shared_ptr<Framebuffer>> m_framebuffers;
    std::unordered_map<std::string, std::shared_ptr<ShadowMap>> m_shadowmaps;
    std::shared_ptr<Bloom> m_bloom;
};
