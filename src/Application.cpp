#include "Application.hpp"
#include "CameraController.hpp"
#include "Skybox.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <iostream>

const int Application::WIDTH = 800;
const int Application::HEIGHT = 600;

Application::model_id_t Application::m_current_id = 0;

Application::Application() {
    std::cout << "Creating application..." << std::endl;
	
	// Camera
	m_camera = std::make_shared<OrbitCamera>();
	m_cameraController = std::make_shared<CameraController>(m_camera);
	
	// shader
	m_shaders["simple"] = std::make_shared<Shader>("shader/simple.vert", "shader/simple.frag");
	m_shaders["skybox"] = std::make_shared<Shader>("shader/skybox.vert", "shader/skybox.frag");
	m_shaders["kernel"] = std::make_shared<Shader>("shader/kernel.vert", "shader/kernel.frag");


	// uniform buffer objects
	m_ubos["UboCamera"] = std::make_shared<Ubo>("UboCamera", sizeof(UboCamera));
	m_ubos["UboCamera"]->uniformBlockBindingPoint(*m_shaders["simple"], 0);
	m_ubos["UboCamera"]->bindBufferToBindingPoint(0);

	// model and skybox
	loadRenderObjects();
	m_models[m_current_id - 1]->setShader(m_shaders["simple"]); // TODO: set shader for each mesh?
	
	std::vector<std::string> skybox_faces = {
		"resource/skybox/iceberg/right.jpg",
		"resource/skybox/iceberg/left.jpg",
		"resource/skybox/iceberg/top.jpg",
		"resource/skybox/iceberg/bottom.jpg",
		"resource/skybox/iceberg/front.jpg",
		"resource/skybox/iceberg/back.jpg"
	};
	m_skybox = std::make_shared<Skybox>(skybox_faces, m_shaders["skybox"]);
	
	// window callback
	m_window.setCameraController(m_cameraController); // TODO: unique_ptr to point
	m_window.setupCallbacks();
	m_window.setInputMode();

	// Framebuffer
	m_framebuffers["KernalEffect"] = std::make_shared<Framebuffer>(WIDTH * 2, HEIGHT * 2, "KernelEffectTexture");
	m_framebuffers["KernalEffect"]->setShader(m_shaders["kernel"]);
	m_framebuffers["KernalEffect"]->bind();
	m_framebuffers["KernalEffect"]->attachTexture();
	m_framebuffers["KernalEffect"]->attachRenderBuffer();
	if (!m_framebuffers["KernalEffect"]->checkStatus()) {
		throw std::runtime_error("Framebuffer is not complete!");
	}
	m_framebuffers["KernalEffect"]->unbind();
	// m_framebuffers["KernalEffect"]->apply();
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

		m_gui.newFrame();
		GuiData guiData = {directLight, pointLight};
		m_gui.updateGUI(guiData);
		
		m_window.processKeyboard(deltaTime);


		viewMatrix = m_camera->getViewMatrix();
		projectionMatrix = m_camera->getProjectionMatrix(static_cast<float>(Application::WIDTH) / Application::HEIGHT);
		
		m_ubos["UboCamera"]->addData(viewMatrix);
		m_ubos["UboCamera"]->addData(projectionMatrix);
		m_ubos["UboCamera"]->addData(m_camera->getCameraPos());
		m_ubos["UboCamera"]->flush();

        m_uniform = {modelMatrix, directLight, pointLight};

		m_framebuffers["KernalEffect"]->bind();
		
		m_renderer.enable(GL_DEPTH_TEST);
		m_renderer.clearColor(0.47f, 0.53f, 0.6f, 1.0f);
        m_renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& kv : m_models) {
            auto model = kv.second;
            m_renderer.render(model, m_uniform); // TODO: set modelMatrix for each model?
        }
		
		m_renderer.setDepthFunc(GL_LEQUAL);
		m_skybox->render(viewMatrix, projectionMatrix);
		m_renderer.setDepthFunc(GL_LESS);

		m_framebuffers["KernalEffect"]->unbind();
		
		m_renderer.disable(GL_DEPTH_TEST);
		m_renderer.clearColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_framebuffers["KernalEffect"]->render();

		m_gui.render();
		
        m_window.swapBuffers();
		m_window.pollEvents();
    }
}

void Application::loadRenderObjects() {
//	auto model = std::make_shared<Model>("resource/model/yellow_car/Pony_cartoon.obj");
    auto model = std::make_shared<Model>("resource/model/nuka_cup/nuka_cup.obj");
//	auto model = std::make_shared<Model>("resource/model/grass_cube/Grass_Block.obj");
    m_models.emplace(m_current_id, model);
    m_current_id++;
}
