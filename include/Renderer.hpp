#pragma once
#include "Model.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void clearColor(float r, float g, float b, float a);
    void clear(const GLenum mask);
    void enable(const GLenum cap);
    void disable(const GLenum cap);

    void setViewport(int width, int height);

    void setDepthFunc(const GLenum func);

    void render(std::shared_ptr<Model> model, SharedUniform& uniform);

private:
    void setGlobalOpenGLState();
};
