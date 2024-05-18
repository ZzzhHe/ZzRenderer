#include "CameraController.hpp"

void CameraController::processKeyboard(GLFWwindow* window, float deltaTime) {
    if (m_camera) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera->processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera->processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera->processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera->processKeyboard(RIGHT, deltaTime);
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

    if (m_camera) {
        m_camera->processMouseMovement(xoffset, yoffset);
    } else if (m_orbitCamera) {
        m_orbitCamera->processMouseMovement(xoffset, yoffset);
    }
}

void CameraController::processMouseScroll(float yoffset) {
    if (m_orbitCamera) {
        m_orbitCamera->processMouseScroll(yoffset);
    }
}
