#include "CameraController.hpp"

#include <stdexcept>

void CameraController::processKeyboard(GLFWwindow* window, float deltaTime) {
	auto cameraSharedPtr = m_camera.lock();
	if (!cameraSharedPtr) {
		throw std::runtime_error("Camera is no longer available.");
	}
	if (auto* camera = dynamic_cast<MoveCamera*>(cameraSharedPtr.get())) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->processKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->processKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->processKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->processKeyboard(RIGHT, deltaTime);
	}
}

void CameraController::processMouseMovement(float xpos, float ypos) {
    if (m_firstMouse) {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;

    m_lastX = xpos;
    m_lastY = ypos;
		
	auto cameraSharedPtr = m_camera.lock();
	if (!cameraSharedPtr) {
		throw std::runtime_error("Camera is no longer available.");
	}

    if (auto* camera = dynamic_cast<MoveCamera*>(cameraSharedPtr.get())) {
		camera->processMouseMovement(xoffset, yoffset);
    } else if (auto* camera = dynamic_cast<OrbitCamera*>(cameraSharedPtr.get())) {
		camera->processMouseMovement(xoffset, yoffset);
    }
}

void CameraController::processMouseScroll(float yoffset) {
	auto cameraSharedPtr = m_camera.lock();
	if (!cameraSharedPtr) {
		throw std::runtime_error("Camera is no longer available.");
	}
		
    if (auto* camera = dynamic_cast<OrbitCamera*>(cameraSharedPtr.get())) {
		camera->processMouseScroll(yoffset);
    }
}
