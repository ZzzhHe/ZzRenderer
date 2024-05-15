#include "Renderer.hpp"

Renderer::Renderer() {
    setGlobalOpenGLState();
}

Renderer::~Renderer() {   
}

void Renderer::clearColor(float r, float g, float b, float a) {
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::setGlobalOpenGLState() {
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));
}

void Renderer::setViewport(int width, int height) {
    GLCall(glViewport(0, 0, width, height));
}

void Renderer::render(std::shared_ptr<Model> model, const SharedUniform& uniform) {
    model->render(uniform);
}