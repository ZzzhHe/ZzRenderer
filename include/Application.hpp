#pragma once

#include "Window.hpp"

class Application {
    public:
        static const int WIDTH = 800;
        static const int HEIGHT = 600;

        Application();
        ~Application();
        void run();
        
    private:
        Window m_window{WIDTH, HEIGHT, "OpenGL"};
};