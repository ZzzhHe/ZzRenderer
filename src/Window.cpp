#include <glad/glad.h>

#include "Window.hpp"

#include <iostream>

Window::Window(int width, int height, const char* title) 
    : m_width(width), m_height(height), m_title(title) {
    
    std::cout << "Creating window..." << std::endl;

    initGLFW();

    setWindowHints();

    createWindow();

    glfwMakeContextCurrent(m_window);

    initGLAD();
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Window::createWindow() {
    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

GLFWwindow* Window::getGLFWWindow() const {
    return m_window;
}

void Window::setWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4); // MSAA
}

void Window::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Window::setCameraController(std::weak_ptr<CameraController> cameraController ) {
	m_cameraController = cameraController;
	glfwSetWindowUserPointer(m_window, this);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	// TODO: need to make sure the Camera Controller be the same,
	// may true the queue system
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	if (!win) {
		throw std::runtime_error("Window pointer is null!");
	}
	
	auto cameraControllerSharedPtr = win->m_cameraController.lock();
	if (!cameraControllerSharedPtr) {
		throw std::runtime_error("CameraController is not available");
	}
	cameraControllerSharedPtr->processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Window::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	if (!win) {
		throw std::runtime_error("Window pointer is null!");
	}
	
	auto cameraControllerSharedPtr = win->m_cameraController.lock();
	if (!cameraControllerSharedPtr) {
		throw std::runtime_error("CameraController is not available");
	}
	
	cameraControllerSharedPtr->processMouseScroll(static_cast<float>(yoffset));
}

void Window::setupCallbacks() {
	glfwSetFramebufferSizeCallback(m_window, Window::framebufferSizeCallback);
	glfwSetCursorPosCallback(m_window, Window::cursorPosCallback);
	glfwSetScrollCallback(m_window, Window::scrollCallback);
}

void Window::setInputMode() {
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::processKeyboard(float deltaTime) {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(m_window, true);
	}
	if (glfwGetKey(m_window, GLFW_KEY_G) == GLFW_PRESS) {
		toggleGuiMode();
	}
	
	auto cameraControllerSharedPtr = m_cameraController.lock();
	
	if (!cameraControllerSharedPtr) {
		throw std::runtime_error("CameraController is not available");
	}
	
	cameraControllerSharedPtr->processKeyboard(m_window, deltaTime);
}

void Window::toggleGuiMode() {
	m_guiMode = !m_guiMode;

	if (m_guiMode) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
