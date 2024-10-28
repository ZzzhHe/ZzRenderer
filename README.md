# Renderer Project

The **Renderer Project** is a learning-focused exploration into advanced real-time 3D rendering techniques using C++ and OpenGL. Developed to deepen understanding of modern graphics programming, this project demonstrates high-performance rendering methods and a variety of sophisticated visual effects. While not intended as a fully-featured game engine, it showcases important rendering capabilities that form the foundation of real-time graphics engines and applications.

![Example](assets/ZzRenderer.gif)

## Features

### Physically Based Rendering (PBR)
This engine is designed with **Physically Based Rendering (PBR)** at its core, using principles that replicate real-world light and material interaction. PBR enhances realism by accurately simulating surface properties, including:

- **Metalness and Roughness**: Enables detailed surface textures, like polished metals or matte materials, through precise PBR parameters.
- **Albedo and Ambient Occlusion Maps**: Provides rich color and depth to materials, enhancing the perception of realism.
- **Dynamic Lighting Interaction**: Objects respond naturally to lighting changes, ensuring consistent material appearance across different environments.

PBR is foundational to the renderer’s realism, producing high-quality visuals across various lighting conditions with accurate reflection, refraction, and shadowing.

### Advanced Lighting
The renderer supports multiple light types, providing flexibility for creating complex scenes with nuanced lighting. The lighting system includes:

- **Directional Lights**: Simulates sunlight with adjustable direction and intensity.
- **Point Lights**: Models localized light sources, ideal for effects such as lamps or other small lights.
- **Shadow Mapping**: Cascaded Shadow Maps (CSM) allow for realistic, high-resolution shadows that enhance spatial depth in scenes.

### Shadow Mapping and Cascaded Shadows
With **Cascaded Shadow Maps (CSM)**, the engine divides the view frustum into multiple sections, each with its own shadow map. This technique reduces shadow aliasing issues, delivering clear, detailed shadows at varying distances. Shadows adjust dynamically based on the camera’s position and orientation, optimizing both quality and performance.

### Bloom Effect with HDR Support
The bloom effect creates a soft glow on bright areas, mimicking real-world lighting behavior. **High Dynamic Range (HDR)** is used to accurately capture and enhance brightness in the scene. The bloom process includes:

- **Brightness Extraction**: Identifies bright areas where bloom will be applied.
- **Gaussian Blur**: Softens bright areas, creating the glow effect.
- **Tone Mapping and Gamma Correction**: Ensures scene brightness is visually balanced, maintaining image quality.

### Skybox Rendering
The engine includes **Skybox Rendering** with HDR support, allowing scenes to be enclosed in immersive cubemaps. Skybox rendering provides dynamic lighting cues and a visually appealing background, contributing to the overall atmosphere of the scene.

### Flexible Shader System
A modular shader system simplifies customization and extension of visual effects. Shaders are organized by function in the `shader/` directory, including:

- **PBR Core Shaders (`main.frag`, `main.vert`)**: Handles physically based material rendering.
- **Shadow Shaders (`shadow.vert`, `shadow.frag`)**: Computes shadow depth.
- **Bloom Shaders (`bloom.frag`, `blur.frag`)**: Implements the bloom effect.
- **Night Vision Shader (`nightvision.frag`)**: Demonstrates a stylized rendering effect.

### Real-Time Camera and Scene Navigation
The camera system supports **FPS** and **Orbit** modes, enabling flexible scene exploration. The `CameraController` class manages camera movement, rotation, and zoom, providing seamless navigation for both broad and detailed views.

### GUI Integration with ImGui
The project integrates **ImGui** to allow real-time adjustments to rendering parameters. This GUI enables toggling effects, modifying lighting, and adjusting camera settings dynamically, allowing rapid experimentation and fine-tuning.

### Framebuffer Management and Post-Processing
The renderer utilizes framebuffers extensively, applying post-processing effects in separate passes for control over each stage of the pipeline:

- **Kernel Effects**: Allows image processing using kernel-based filters.
- **Night Vision**: A unique effect with green tint and noise, showcasing custom effect implementation.
- **Pass-Through Effects**: Simple effects for demonstrating chaining and layering of custom filters.

### Material and Texture Handling with Assimp
Materials can use various texture maps (albedo, metallic, roughness, normal, ambient occlusion, emission, and shadow), enhancing material appearance. **Assimp** is used for model loading, simplifying the process of adding new 3D assets by importing textures and material data in formats like OBJ. This allows for efficient model integration, ideal for asset-rich applications.

## Dependencies

The renderer relies on the following libraries, ensuring compatibility and performance:

- **C++17**: Modern C++ standards for efficient memory management and functionality.
- **OpenGL 4.1**: Advanced rendering support with shader compatibility.
- **GLFW**: Window creation and input handling.
- **GLAD**: OpenGL function loading for consistent API support.
- **GLM**: Mathematics library tailored for 3D graphics.
- **ImGui**: Lightweight GUI for easy parameter adjustment.

To install dependencies on Ubuntu:

`` sudo apt-get update sudo apt-get install cmake glfw3 libglfw3-dev ``

## Setup

### Clone the Repository

`` git clone https://github.com/yourusername/renderer_project.git cd renderer_project ``

### Build Instructions

To build the project:

`` mkdir build cd build cmake .. make ``

### Running the Application

After building, run the executable:

`` ./renderer_app ``

## Usage

### Controls

- **W, A, S, D**: Camera movement
- **Mouse**: Rotate the camera
- **GUI**: Toggle to adjust render settings dynamically.

### Adding Models

Place OBJ format models in the `resources/models` directory. Update `loadRenderObjects()` in `Application.cpp` to initialize new models with shaders and materials.

## Rendering Pipeline

The renderer’s pipeline maximizes visual quality while optimizing for performance:

1. **Shadow Pass**: Calculates shadow depth for each light source using cascaded shadow maps.
2. **Main Pass**: Renders models with deferred shading, leveraging PBR materials.
3. **Post-Processing**:
   - **Bloom**: Adds glow to bright areas with multi-pass blurring.
   - **Special Effects**: Kernel effects and night vision shaders demonstrate customization.
4. **Final Composition**: Combines all elements into the final rendered output.

This structured pipeline supports real-time performance, ensuring each component efficiently contributes to high-quality visual output.

---

The **Renderer Project** is a comprehensive demonstration of modern rendering techniques and optimization practices. This engine showcases skills in graphics programming and serves as a robust foundation for further development in real-time 3D rendering.
