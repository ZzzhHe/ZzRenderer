#include "GLDebug.hpp"
#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>  
#include <iostream>

Shader::Shader(const std::string &vertpath, const std::string &fragpath)
    :m_rendererID(0), m_vertPath(vertpath), m_fragPath(fragpath) {

    ShaderProgramSource source = ParseShader(vertpath, fragpath);
    m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string &vertpath, const std::string& geompath, const std::string &fragpath) 
: m_rendererID(0), m_vertPath(vertpath), m_geomPath(geompath), m_fragPath(fragpath) {
    ShaderProgramSource source = ParseShader(vertpath, geompath, fragpath);
    m_rendererID = CreateShader(source.VertexSource, source.GeometrySource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_rendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string &vertpath, const std::string &fragpath) {
    std::ifstream stream_vertex(vertpath);
    std::ifstream stream_fragment(fragpath);

    if (!stream_vertex.is_open() || !stream_fragment.is_open()) {
        throw std::runtime_error("Failed to open shader file");
    }

    std::string line;

    std::stringstream ss_vertex;
    std::stringstream ss_fragment;

    while (getline(stream_vertex, line)) {
        ss_vertex << line << '\n';
    }

    while (getline(stream_fragment, line)) {
        ss_fragment << line << '\n';
    }

    return {ss_vertex.str(), "", ss_fragment.str()};
}

ShaderProgramSource Shader::ParseShader(const std::string &vertpath, const std::string &geompath, const std::string &fragpath) {
    std::ifstream stream_vertex(vertpath);
    std::ifstream stream_geometry(geompath);
    std::ifstream stream_fragment(fragpath);

    if (!stream_vertex.is_open() || !stream_fragment.is_open() || !stream_geometry.is_open()) {
        throw std::runtime_error("Failed to open shader file");
    }

    std::string line;

    std::stringstream ss_vertex;
    std::stringstream ss_geometry;
    std::stringstream ss_fragment;

    while (getline(stream_vertex, line)) {
        ss_vertex << line << '\n';
    }

    while (getline(stream_geometry, line)) {
        ss_geometry << line << '\n';
    }

    while (getline(stream_fragment, line)) {
        ss_fragment << line << '\n';
    }

    return {ss_vertex.str(), ss_geometry.str(), ss_fragment.str()};
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

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& geometryShaders, const std::string& fragmentShader) {
    GLCall(GLuint program = glCreateProgram());
    GLCall(GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(GLuint gs = CompileShader(GL_GEOMETRY_SHADER, geometryShaders));
    GLCall(GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));
    
    // attach and link reviously compiled shaders to the program object
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, gs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // delete shader objects after using
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteProgram(gs));
    GLCall(glDeleteProgram(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    m_uniformLocationCache[name] = location;
    return location;
}


void Shader::use() const {
    GLCall(glUseProgram(m_rendererID)); 
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

void Shader::setFloat(const std::string &name, const float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value)); 
}

void Shader::setFloatArray(const std::string &name, const std::vector<float>& value){
	for (unsigned int i = 0; i < value.size(); ++i) {
		std::string newName = name + "[" + std::to_string(i) + "]";
		GLCall(glUniform1f(GetUniformLocation(newName), value[i]));
	}
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::setMat4Array(const std::string &name, const std::vector<glm::mat4> &value) {
    for (unsigned int i = 0; i < value.size(); ++i) {
        std::string newName = name + "[" + std::to_string(i) + "]";
        GLCall(glUniformMatrix4fv(GetUniformLocation(newName), 1, GL_FALSE, glm::value_ptr(value[i])));
    }
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    GLCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::setVec2(const std::string &name, glm::vec2 &value) {
	GLCall(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) {
	GLCall(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

