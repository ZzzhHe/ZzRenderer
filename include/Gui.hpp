#pragma once

#include "Window.hpp"
#include "Light.hpp"

struct GuiData{
    DirectLight& directLight;
    PointLight& pointLight;
};

class Gui{
public:
    Gui(Window& window);
    ~Gui();

    void newFrame();
    void updateGUI(GuiData &guiData);
    void render();
};