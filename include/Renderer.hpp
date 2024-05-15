#pragma once
#include "Model.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void clearColor(float r, float g, float b, float a);
    void clear();

    void setViewport(int width, int height);

    void render(std::shared_ptr<Model> model, const SharedUniform& uniform);

private:
    void setGlobalOpenGLState();
};