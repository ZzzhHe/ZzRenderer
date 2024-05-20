#pragma once

#include "Model.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
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
	void renderImGui();

private:
    Window m_window{WIDTH, HEIGHT, "OpenGL"};
    Renderer m_renderer{};
	
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<CameraController> m_cameraController;
	
	SharedUniform m_uniform;
	
	std::shared_ptr<Shader> m_shader;
	
    model_map_t m_models; // will be replaced by a scene graph(render object)
    static model_id_t m_current_id;
};
