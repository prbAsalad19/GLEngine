#pragma once
#include "opengl/OpenGLConfig.h"
#include "opengl/OpenGLShaderProgram.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/Scene.h"
#include "core/scene/Camera.h"

class OpenGLRenderer
{
public:
    OpenGLRenderer(ResourceManager& resources,
        const std::string& vertPath,
        const std::string& fragPath);

    void init();
    void shutdown();
    void onResize(unsigned int width, unsigned int height);
    void render(const Scene& scene, const Camera& camera);

private:
    ResourceManager& m_resources;
    OpenGLShaderProgram m_shader;
    unsigned int        m_width = 1280;
    unsigned int        m_height = 720;
};