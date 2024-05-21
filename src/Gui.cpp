#include "Gui.hpp"

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
    
    if(ImGui::TreeNode("Direct Light")) {
        ImGui::SliderFloat3("Direction", &guiData.directLight.direction[0], -10.0f, 10.0f, "X: %.1f Y: %.1f Z: %.1f");
        
        ImGui::ColorEdit3("Color", &guiData.directLight.color[0], ImGuiColorEditFlags_Float);
        
        // Slider for light intensity using the alpha channel of the color
        ImGui::SliderFloat("Intensity", &guiData.directLight.color.w, 0.0f, 1.0f, "%.2f");
        
        // Color picker for ambient color (ignores alpha)
        ImGui::ColorEdit3("Ambient Color", &guiData.directLight.ambientColor[0], ImGuiColorEditFlags_Float);
        
        // Slider for ambient intensity using the alpha channel of the ambient color
        ImGui::SliderFloat("Ambient Intensity", &guiData.directLight.ambientColor.w, 0.0f, 1.0f, "%.2f");
        
        ImGui::TreePop();
    }
    
    if(ImGui::TreeNode("Point Light")) {
        ImGui::SliderFloat3("Position", &guiData.pointLight.position[0], -10.0f, 10.0f, "X: %.1f Y: %.1f Z: %.1f");
        
        ImGui::ColorEdit3("Color", &guiData.pointLight.color[0], ImGuiColorEditFlags_Float);
        
        // Slider for light intensity using the alpha channel of the color
        ImGui::SliderFloat("Intensity", &guiData.pointLight.color.w, 0.0f, 1.0f, "%.2f");
        
        // Color picker for ambient color (ignores alpha)
        ImGui::ColorEdit3("Ambient Color", &guiData.pointLight.ambientColor[0], ImGuiColorEditFlags_Float);
        
        // Slider for ambient intensity using the alpha channel of the ambient color
        ImGui::SliderFloat("Ambient Intensity", &guiData.pointLight.ambientColor.w, 0.0f, 1.0f, "%.2f");
            
        ImGui::TreePop();
    }

    ImGui::End();
}

void Gui::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}