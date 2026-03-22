#pragma once
#include "OpenGLConfig.h"
#include "core/ui/UIQuadBatch.h"
#include "core/resourcemanager/ResourceManager.h"
#include "opengl/OpenGLShaderProgram.h"

static constexpr uint32_t MAX_UI_VERTICES = 4096;  // 1024 quad
static constexpr uint32_t MAX_UI_INDICES = 6144;  // 1024 quad * 6 indici

class OpenGLUIRenderer {
public:
    OpenGLUIRenderer(ResourceManager& m_resources, unsigned int width, unsigned int height, const std::string& vertPath, const std::string& fragPath);
    ~OpenGLUIRenderer();
    void init();
    void render(const UICanvas& canvas);
    void shutdown();

private:
    ResourceManager& m_resources;
    OpenGLShaderProgram m_shader;
    unsigned int        m_width = 1280;
    unsigned int        m_height = 720;

    GLuint m_VAO, m_VBO, m_EBO;
	UIQuadBatch m_batch;
};
