#include "Renderer.hpp"

Renderer::Renderer() {
    setGlobalOpenGLState();
}

Renderer::~Renderer() {   
}

void Renderer::clearColor(float r, float g, float b, float a) {
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::clear(const GLenum mask) {
    GLCall(glClear(mask));
}

void Renderer::setGlobalOpenGLState() {
    GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_FRAMEBUFFER_SRGB)); // gamma correction
    GLCall(glEnable(GL_CULL_FACE)); // backface culling
	GLCall(glEnable(GL_DEPTH_CLAMP));
}

void Renderer::setViewport(int width, int height) {
    GLCall(glViewport(0, 0, width, height));
}

void Renderer::setDepthFunc(const GLenum func) {
    GLCall(glDepthFunc(func));
}

void Renderer::render(std::shared_ptr<Model> model, const SharedUniform& uniform) {
    model->render(uniform);
}

void Renderer::enable(const GLenum cap) {
    GLCall(glEnable(cap));
}

void Renderer::disable(const GLenum cap) {
    GLCall(glDisable(cap));
}
