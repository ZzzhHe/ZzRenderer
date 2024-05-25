#include "Gui.hpp"

#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Gui::Gui(Window& window) {
    // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_None;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

Gui::~Gui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::updateGUI(GuiData& guiData) {
    ImGui::Begin("Settings");
		
    ImGui::CollapsingHeader("Lights");
    

    for (auto& [name, light] : guiData.lights) {
        if(ImGui::TreeNode(name.c_str())) {
            if (light->type == LightType::Point) {
                auto pointLight = std::dynamic_pointer_cast<PointLight>(light);
                ImGui::SliderFloat3("Position", &pointLight->position[0], -10.0f, 10.0f, "X: %.1f Y: %.1f Z: %.1f");
            } else {
                auto directLight = std::dynamic_pointer_cast<DirectLight>(light);
                ImGui::SliderFloat3("Direction", &directLight->direction[0], -10.0f, 10.0f, "X: %.1f Y: %.1f Z: %.1f");
            }

            // Slider for light intensity using the alpha channel of the color
            ImGui::SliderFloat("Intensity", &light->color.w, 0.0f, 1.0f, "%.2f");
            
            // Color picker for ambient color (ignores alpha)
            ImGui::ColorEdit3("Ambient Color", &light->ambientColor[0], ImGuiColorEditFlags_Float);
            
            // Slider for ambient intensity using the alpha channel of the ambient color
            ImGui::SliderFloat("Ambient Intensity", &light->ambientColor.w, 0.0f, 1.0f, "%.2f");

            ImGui::TreePop();
        }
    }

    ImGui::CollapsingHeader("Post-processing");

        // Create a C-style array of strings for ImGui
    std::vector<const char*> items;
    for (const auto& name : guiData.framebufferType) {
        items.push_back(name.c_str());
    }

    // Variable to store the selected index
    static int selectedIndex = 0;
    // Find the index of the current framebuffer
    auto it = std::find(guiData.framebufferType.begin(), guiData.framebufferType.end(), guiData.currentFramebuffer);
    if (it != guiData.framebufferType.end()) {
        selectedIndex = std::distance(guiData.framebufferType.begin(), it);
    }

    // Create a combo box
    if (ImGui::Combo("Effect", &selectedIndex, items.data(), items.size())) {
        // Update the current framebuffer based on user selection
        guiData.currentFramebuffer = guiData.framebufferType[selectedIndex];
    }

    ImGui::End();
}

void Gui::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}