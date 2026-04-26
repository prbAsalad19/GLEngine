#pragma once
#include "opengl/OpenGLConfig.h"
#include "opengl/OpenGLShaderProgram.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/Scene.h"
#include "core/assets/Camera.h"
#include "core/scene/RenderCommand.h"
#include "core/bvh/BVHTree.h"

static constexpr uint32_t MAX_RENDER_OBJECTS = 1024;
static constexpr GLuint CAMERA_UBO_BINDING = 0;
static constexpr GLuint TRANSFORM_UBO_BINDING = 1;

struct CameraUBOData
{
    float view[16];
	float projection[16];
	float viewProjection[16];
    float cameraPosition[3];
    float _padding;
};

class OpenGLRenderer
{
public:
    OpenGLRenderer(ResourceManager& resources,
        const std::string& vertPath,
        const std::string& fragPath);
	~OpenGLRenderer();

    void init();
    void shutdown();
    void onResize(unsigned int width, unsigned int height);
    void render(const Scene& scene, 
                            const std::vector<RenderObject>& staticObjects,
                            const std::vector<RenderObject>& quasiStaticObjects,
                            const std::vector<RenderObject>& dynamicSlowObjects,
                            const Camera& camera,
                            const BVHTree& staticBVH,
                            const BVHTree& quasiStaticBVH,
                            const BVHTree& dynamicBVH,
                            const std::vector<RenderObject>& dynamicFastObjects);

private:
    ResourceManager& m_resources;
    OpenGLShaderProgram m_shader;
    unsigned int        m_width = 1280;
    unsigned int        m_height = 720;

	GLuint m_cameraUBO;
	GLuint m_transformUBO;

	mat4 m_transformStagingBuffer[MAX_RENDER_OBJECTS];
};