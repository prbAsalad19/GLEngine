#include "OpenGLShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertPath,
    const std::string& fragPath)
{
    GLuint vs = compileModule(vertPath, GL_VERTEX_SHADER);
    GLuint fs = compileModule(fragPath, GL_FRAGMENT_SHADER);

    if (!vs || !fs)
    {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        std::cout << "[ShaderProgram] Compilation failed, program not created.\n";
        return;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vs);
    glAttachShader(m_programID, fs);
    glLinkProgram(m_programID);

    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(m_programID, sizeof(log), nullptr, log);
        std::cout << "[ShaderProgram] Link error:\n" << log << "\n";
        glDeleteProgram(m_programID);
        m_programID = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    std::cout << "[ShaderProgram] Program created, ID = " << m_programID << "\n";
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
    if (m_programID)
        glDeleteProgram(m_programID);
}

void OpenGLShaderProgram::bind() const
{
    glUseProgram(m_programID);
}

void OpenGLShaderProgram::unbind() const
{
    glUseProgram(0);
}

void OpenGLShaderProgram::setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const
{
    GLuint index = glGetUniformBlockIndex(m_programID, blockName.c_str());
    if (index != GL_INVALID_INDEX)
        glUniformBlockBinding(m_programID, index, bindingPoint);
    else
        std::cout << "[ShaderProgram] Uniform block '" << blockName << "' not found.\n";
}

void OpenGLShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(getUniformLocation(name), value);
}

void OpenGLShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(getUniformLocation(name), value);
}

void OpenGLShaderProgram::setMat4(const std::string& name, const mat4& matrix) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.entries);
}

void OpenGLShaderProgram::setVec3(const std::string& name, const Vector3& v) const
{
    glUniform3fv(getUniformLocation(name), 1, v.entries);
}

void OpenGLShaderProgram::setUInt(const std::string& name, unsigned int value) const
{
    glUniform1ui(getUniformLocation(name), value);
}

GLint OpenGLShaderProgram::getUniformLocation(const std::string& name) const
{
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end())
        return it->second;

    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location == -1)
        std::cout << "[ShaderProgram] Warning: uniform '" << name << "' not found.\n";

    m_uniformLocationCache[name] = location;
    return location;
}

GLuint OpenGLShaderProgram::compileModule(const std::string& filepath, GLenum type)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cout << "[ShaderProgram] Failed to open: " << filepath << "\n";
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string src = ss.str();
    const char* csrc = src.c_str();

    //  << "[Shader] " << filepath << " first chars: "
    //    << src.substr(0, 50) << "\n";

    GLuint module = glCreateShader(type);
    glShaderSource(module, 1, &csrc, nullptr);
    glCompileShader(module);

    int success;
    glGetShaderiv(module, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(module, sizeof(log), nullptr, log);
        std::cout << "[ShaderProgram] Compile error (" << filepath << "):\n" << log << "\n";
        glDeleteShader(module);
        return 0;
    }

    return module;
}