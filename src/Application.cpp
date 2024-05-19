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
	
//	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	OrbitCamera camera;
	CameraController cameraController(camera);
	
	m_window.setCameraController(&cameraController);
	m_window.setupCallbacks();
	m_window.setInputMode();
    
    loadRenderObjects();
    
    auto shader = std::make_shared<Shader>("shader/simple.vert", "shader/simple.frag");
    m_models[m_current_id - 1]->setShader(shader); // TODO: set shader for each mesh?

    glm::mat4 modelMatrix = glm::mat4(1.0f);
//	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
    
    SharedUniform uniform;
	
//	DirectLight directLight = {glm::vec3(5.0f, -5.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f)}; // TODO: y and -y axis?

    PointLight pointLight = {glm::vec3(10.0f, 10.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f)}; // TODO: y and -y axis?
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!m_window.shouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraController.processKeyboard(m_window.getGLFWWindow(), deltaTime);

        m_window.pollEvents();
//        m_renderer.clearColor(0.80f, 0.94f, 1.0f, 1.0f);
		m_renderer.clearColor(0.47f, 0.53f, 0.6f, 1.0f);
        m_renderer.clear();

		glm::mat4 viewMatrix = camera.getViewMatrix();
		glm::mat4 projectionMatrix = camera.getProjectionMatrix(static_cast<float>(Application::WIDTH) / Application::HEIGHT);
		
		
        uniform = {modelMatrix, viewMatrix, projectionMatrix, pointLight, camera.getCameraPos()};
        for (const auto& kv : m_models) {
            auto model = kv.second;
            m_renderer.render(model, uniform); // TODO: set modelMatrix for each model?
        }
        m_window.swapBuffers();
    }
}

void Application::loadRenderObjects() {
//	auto model = std::make_shared<Model>("models/yellow_car/Pony_cartoon.obj");
    auto model = std::make_shared<Model>("models/nuka_cup/nuka_cup.obj");
//	auto model = std::make_shared<Model>("models/grass_cube/Grass_Block.obj");
    m_models.emplace(m_current_id, model);
    m_current_id++;
}
