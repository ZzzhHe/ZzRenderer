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
    m_models[m_current_id - 1]->setShader(shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -3.0f, -10.0f)); // For testing, set to identity or a simple view
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!m_window.shouldClose()) {
        m_window.pollEvents();
        m_window.clear();
        shader->use();
        shader->setMat4("model", model);
        shader->setMat4("view", viewMatrix);
        shader->setMat4("projection", projectionMatrix);
        render();
        m_window.swapBuffers();
    }
}

void Application::loadRenderObjects() {
    auto model = std::make_shared<Model>("models/grass_cube/Grass_Block.obj");
    m_models.emplace(m_current_id, model);
    m_current_id++;
}

void Application::render() {
    for (auto &kv : m_models) {
        kv.second->render();
    }
}