// TODO: 1. add Light class, add Camera class
// TODO: 2. set how manage the texture materials 

#include "Application.hpp"
#include "CameraController.hpp"

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
	
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	
	m_window.setCameraController(camera);
	m_window.setupCallbacks();
	m_window.setInputMode();
    
    loadRenderObjects();
    
    auto shader = std::make_shared<Shader>("shader/simple.vert", "shader/simple.frag");
    m_models[m_current_id - 1]->setShader(shader); // TODO: set shader for each mesh?


    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    SharedUniform uniform;

    DirectLight light = {glm::vec3(1.0f, 2.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f)};
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!m_window.shouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_window.processKeyboard(deltaTime);

        m_window.pollEvents();
        m_renderer.clearColor(0.8f, 0.8f, 0.8f, 1.0f);
        m_renderer.clear();

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = camera.GetProjectionMatrix(static_cast<float>(Application::WIDTH) / Application::HEIGHT);

        uniform = {modelMatrix, viewMatrix, projectionMatrix, light};
        for (const auto& kv : m_models) {
            auto model = kv.second;
            m_renderer.render(model, uniform); // TODO: set modelMatrix for each model?
        }
        m_window.swapBuffers();
    }
}

void Application::loadRenderObjects() {
    auto model = std::make_shared<Model>("models/grass_cube/Grass_Block.obj");
    m_models.emplace(m_current_id, model);
    m_current_id++;
}
