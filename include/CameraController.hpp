#pragma once

#include "Camera.hpp"

class CameraController {
public:
    CameraController(Camera& camera)
        : m_camera(&camera), m_orbitCamera(nullptr), m_firstMouse(true), m_lastX(0.0f), m_lastY(0.0f) {}

    CameraController(OrbitCamera& orbitCamera)
        : m_camera(nullptr), m_orbitCamera(&orbitCamera), m_firstMouse(true), m_lastX(0.0f), m_lastY(0.0f) {}

    void processKeyboard(GLFWwindow* window, float deltaTime);

    void processMouseMovement(float xpos, float ypos);

    void processMouseScroll(float yoffset);

    Camera* m_camera;
    OrbitCamera* m_orbitCamera;
    bool m_firstMouse;
    float m_lastX, m_lastY;
};