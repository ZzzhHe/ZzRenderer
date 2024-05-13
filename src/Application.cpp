#include "Application.hpp"
#include <iostream>

Application::Application() {
    std::cout << "Creating application..." << std::endl;
}

Application::~Application() {
}   

void Application::run() {
    std::cout << "Running application..." << std::endl;
    while (!m_window.shouldClose()) {
        m_window.pollEvents();
        m_window.swapBuffers();
    }
}