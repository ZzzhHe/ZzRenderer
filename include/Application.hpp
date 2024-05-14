#pragma once

#include "Model.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include <unordered_map>

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
    model_map_t m_models; // will be replaced by a scene graph(render object)
    static model_id_t m_current_id;
};