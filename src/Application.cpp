// TODO: add Light class, add Camera class

#include "Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application::model_id_t Application::m_current_id = 0;

Application::Application() {
    std::cout << "Creating application..." << std::endl;
}

Application::~Application() {
}   

void Application::run() {
    std::cout << "Running application..." << std::endl;
    loadRenderObjects();
    auto shader = std::make_shared<Shader>("shader/simple.vert", "shader/simple.frag");
    m_models[m_current_id - 1]->setShader(shader); // TODO: set shader for each mesh?

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -3.0f, -10.0f)); // For testing, set to identity or a simple view
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    SharedUniform uniform;

    while (!m_window.shouldClose()) {
        m_window.pollEvents();
        m_renderer.clearColor(0.8f, 0.8f, 0.8f, 1.0f);
        m_renderer.clear();
        uniform = {modelMatrix, viewMatrix, projectionMatrix};
        for (const auto& model : m_models) {
            m_renderer.render(model.second, uniform); // TODO: set modelMatrix for each model?
        }
        m_window.swapBuffers();
    }
}

void Application::loadRenderObjects() {
    auto model = std::make_shared<Model>("models/grass_cube/Grass_Block.obj");
    m_models.emplace(m_current_id, model);
    m_current_id++;
}