#pragma once

#include "Light.hpp"
#include "Window.hpp"

#include <unordered_map>

struct GuiData{
    std::unordered_map<std::string, std::shared_ptr<Light>>& lights;
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