#include "Application.hpp"
#include "CameraController.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application::model_id_t Application::m_current_id = 0;

Application::Application() {
    std::cout << "Creating application..." << std::endl;
	
	loadRenderObjects();
	
	// set Camera
	m_camera = std::make_shared<OrbitCamera>();
	m_cameraController = std::make_shared<CameraController>(m_camera);
	
	m_shader = std::make_shared<Shader>("shader/simple.vert", "shader/simple.frag");
	m_models[m_current_id - 1]->setShader(m_shader); // TODO: set shader for each mesh?
	
	m_window.setCameraController(m_cameraController); // TODO: unique_ptr to point
	m_window.setupCallbacks();
	m_window.setInputMode();
	
}

Application::~Application() {
}

void Application::run() {
    std::cout << "Running application..." << std::endl;

	DirectLight directLight = {glm::vec3(5.0f, -5.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)}; // TODO: y and -y axis?

	PointLight pointLight = {glm::vec3(2.0f, 2.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)};

    glm::mat4 modelMatrix = glm::mat4(1.0f);
//	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!m_window.shouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_window.pollEvents();
		
		m_gui.newFrame();
		GuiData guiData = {directLight, pointLight};
		m_gui.updateGUI(guiData);
		
		m_window.processKeyboard(deltaTime);

		m_renderer.clearColor(0.47f, 0.53f, 0.6f, 1.0f);
        m_renderer.clear();
		m_renderer.setDepthFunc(GL_LESS);

		viewMatrix = m_camera->getViewMatrix();
		projectionMatrix = m_camera->getProjectionMatrix(static_cast<float>(Application::WIDTH) / Application::HEIGHT);
		
        m_uniform = {modelMatrix, viewMatrix, projectionMatrix, directLight, pointLight, m_camera->getCameraPos()};
        for (const auto& kv : m_models) {
            auto model = kv.second;
            m_renderer.render(model, m_uniform); // TODO: set modelMatrix for each model?
        }
		
		m_gui.render();
		
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
