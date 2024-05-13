#include "GLDebug.hpp"
#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>  
#include <iostream>

Shader::Shader(const std::string& filepath)
    :m_RendererID(0), m_FilePath(filepath) {

    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    // Open file
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };


    std::string line;
    // Create an array of string streams for vertex and fragment shaders
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    // read line by line
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            // If the line doesn't contain "#shader", add it to the string stream of the current shader type
            ss[(int)type] << line << '\n';
        }
    }
    // Return the shader source code for vertex and fragment shaders
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    // create vertex shader object
    GLCall(GLuint id = glCreateShader(type));
    const char* src = source.c_str();
    // attach shader source code to shader object
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // compile shader
    glCompileShader(id);  

    int success;
    char infoLog[1024];
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(id, 1024, nullptr, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        GLCall(glDeleteShader(id));
    }
    return id;
}


GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(GLuint program = glCreateProgram());
    GLCall(GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));
    
    // attach and link reviously compiled shaders to the program object
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // delete shader objects after using
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteProgram(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    m_UniformLocationCache[name] = location;
    return location;
}


void Shader::use() const {
    GLCall(glUseProgram(m_RendererID)); 
}

void Shader::unUse() const {
    GLCall(glUseProgram(0));
}

// Don't use bool
void Shader::setBool(const std::string &name, bool value) {
    GLCall(glUniform1i(GetUniformLocation(name), (int)value));
}

void Shader::setInt(const std::string &name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));     
}

void Shader::setUInt(const std::string &name, unsigned int value) {
    GLCall(glUniform1ui(GetUniformLocation(name), value)); 
}

void Shader::setFloat(const std::string &name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value)); 
}

void Shader::setMat4(const std::string &name, glm::mat4 &value) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::setVec3(const std::string &name, glm::vec3 &value) {
    GLCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::setVec2(const std::string &name, glm::vec2 &value) {
	GLCall(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::setVec4(const std::string &name, glm::vec4 &value) {
	GLCall(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

