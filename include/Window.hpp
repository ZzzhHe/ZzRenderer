#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include "CameraController.hpp"

class Window {
public:
    Window(int width, int height, const char *title);
    ~Window();

    void swapBuffers();
    void pollEvents();

    bool shouldClose() const;

    GLFWwindow *getGLFWWindow() const;
	
	void setCameraController(std::weak_ptr<CameraController> cameraController);
	void processKeyboard(float deltaTime);
	void toggleGuiMode();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
	static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow *window, double xpos, double ypos);
	
	void setupCallbacks();
	void setInputMode();

private:
    void initGLFW();
    void createWindow();
    void initGLAD();
    void setWindowHints();

private:
    int m_width, m_height;
    const char *m_title;
    GLFWwindow *m_window;
	std::weak_ptr<CameraController> m_cameraController;
	
	bool m_guiMode = false;
};
