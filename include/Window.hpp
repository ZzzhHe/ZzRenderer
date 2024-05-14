#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char *title);
    ~Window();

    
    void swapBuffers();
    void pollEvents();

    bool shouldClose() const;

    GLFWwindow *getGLFWWindow() const;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

private:
    void initGLFW();
    void createWindow();
    void initGLAD();
    void setWindowHints();

private:
    int m_width, m_height;
    const char *m_title;
    GLFWwindow *m_window;
};