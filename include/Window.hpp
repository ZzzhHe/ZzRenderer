#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char *title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

    GLFWwindow *getGLFWWindow() const;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

private:
    void initGLFW();
    void createWindow();
    void initGLAD();
    void setWindowHints();
    void setGlobalOpenGLState();

private:
    int m_width, m_height;
    const char *m_title;
    GLFWwindow *m_window;
};