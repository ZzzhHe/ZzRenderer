#pragma once

#include "Camera.hpp"

#include <memory>

class CameraController {
public:
    CameraController(const std::weak_ptr<Camera>& camera)
        : m_camera(camera), m_firstMouse(true), m_lastX(0.0f), m_lastY(0.0f) {}

    void processKeyboard(GLFWwindow* window, float deltaTime);

    void processMouseMovement(float xpos, float ypos);

    void processMouseScroll(float yoffset);

private:
    std::weak_ptr<Camera> m_camera;
    bool m_firstMouse;
    float m_lastX, m_lastY;
};
