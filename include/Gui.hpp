#pragma once

#include "Window.hpp"
#include "Light.hpp"

struct GuiData{
    DirectLight& directLight;
    PointLight& pointLight;
    const std::vector<std::string>& framebufferType;
    std::string& currentFramebuffer;
};

class Gui{
public:
    Gui(Window& window);
    ~Gui();

    void newFrame();
    void updateGUI(GuiData &guiData);
    void render();
};