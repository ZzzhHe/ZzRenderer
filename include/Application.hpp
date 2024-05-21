#pragma once

#include "Model.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Gui.hpp"

#include <unordered_map>
#include <memory>

class Application {
public:
    static const int WIDTH = 800;
    static const int HEIGHT = 600;

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
	
	SharedUniform m_uniform;
	
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Shader> m_skyboxShader;
    
    model_map_t m_models; // will be replaced by a scene graph(render object)
    static model_id_t m_current_id;
};
