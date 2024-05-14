/*
#TODO:
1. think of how to set shader to one model and each meshes in the model 
2. seperate shader to vertex and fragment shader
*/

#include "Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

void Application::loadRenderObjects() {
    auto model = Model("models/grass_cube/Grass_Block.obj");
    m_models.emplace(0, model);


}

void Application::render() {
    
}