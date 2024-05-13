#pragma once

#include "Model.hpp"
#include "Window.hpp"
#include <unordered_map>

class Application {
public:
    static const int WIDTH = 800;
    static const int HEIGHT = 600;

    Application();
    ~Application();
    void run();

private:
    void loadRenderObjects();
    void render();

private:
    Window m_window{WIDTH, HEIGHT, "OpenGL"};
    std::unordered_map<uint32_t, Model> m_models; // will be replaced by a scene graph(render object)
};