#pragma once
#include <GLFW/glfw3.h>
#include <string>

enum struct TextureType {
    ALBEDO,
    NORMAL,
    METALLIC,
    ROUGHNESS,
	AO,
	EMISSION,
    CUBE_MAP,
    FRAMEBUFFER,
    SHADOW_MAP,
    Array_2D,
};

class Texture {
public:
    Texture(const std::vector<std::string>& paths, TextureType type); // for cube maps
    Texture(const std::string& path, const TextureType type);
    Texture(const unsigned int width, const unsigned int height, const TextureType type);
    Texture(const unsigned int width, const unsigned int height, const unsigned int shadowCascadeLevelCount, const TextureType type);

    ~Texture();

    void bind(const unsigned int slot = 0) const;
    void unBind() const;

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
    inline GLuint id() const { return m_RendererID; }
    inline std::string getFilePath() const { return m_FilePath; }
    inline TextureType getType() const { return m_Type; }

private:
    GLuint m_RendererID;
    const std::string m_FilePath;
    const TextureType m_Type;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
};
