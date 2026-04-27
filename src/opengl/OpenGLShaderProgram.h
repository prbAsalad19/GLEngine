#pragma once
#include "opengl/OpenGLConfig.h"
#include "core/math/algebricOp.h"
#include <string>
#include <unordered_map>

class OpenGLShaderProgram
{
public:
    OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath);

    enum class ShaderType { Compute };
    OpenGLShaderProgram(const std::string& computePath, ShaderType type);
    ~OpenGLShaderProgram();

    OpenGLShaderProgram(const OpenGLShaderProgram&) = delete;
    OpenGLShaderProgram& operator=(const OpenGLShaderProgram&) = delete;

    void bind()   const;
    void unbind() const;

    void setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const;
    void setInt(const std::string& name, int value)          const;
    void setFloat(const std::string& name, float value)        const;
    void setMat4(const std::string& name, const mat4& matrix) const;
    void setVec2(const std::string& name, const Vector2& v)   const;
    void setVec3(const std::string& name, const Vector3& v)   const;
    void setUInt(const std::string& name, unsigned int value) const;
    GLuint getProgramID() const { return m_programID; }
    void dispatch(unsigned int groupX, unsigned int groupY, unsigned int groupZ) const;

private:
    GLuint m_programID = 0;
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

    GLint         getUniformLocation(const std::string& name) const;
    static GLuint compileModule(const std::string& filepath, GLenum type);
};