#pragma once
#include "opengl/OpenGLConfig.h"
#include "opengl/OpenGLShaderProgram.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/Scene.h"
#include "core/assets/Camera.h"
#include "core/scene/RenderCommand.h"
#include "core/bvh/BVHTree.h"
#include "core/scene/LightManager.h"
#include <unordered_map>

static constexpr uint32_t MAX_RENDER_OBJECTS = 1024;
static constexpr GLuint CAMERA_UBO_BINDING = 0;
static constexpr GLuint TRANSFORM_UBO_BINDING = 1;

static constexpr uint32_t CLUSTERS_X = 16;
static constexpr uint32_t CLUSTERS_Y = 9;
static constexpr uint32_t CLUSTERS_Z = 24;
static constexpr uint32_t TOTAL_CLUSTERS = CLUSTERS_X * CLUSTERS_Y * CLUSTERS_Z;


struct CameraUBOData
{
    float view[16];           // 0
    float projection[16];     // 64
    float viewProjection[16]; // 128
    float inverseView[16];    // 192
    float inverseProjection[16]; // 256

    float cameraPosition[3];  // 320
    float zNear;              // 332 (Riempie il 4° slot di cameraPosition)

    float zFar;               // 336
    float _pad0[3];           // 340 <--- PADDING per allineare gridRes a 352
    
    uint32_t gridRes[4];      // 352
    float screenRes[2];       // 368
    
    float _padding;           // 376
    float _pad1;              // 380 <--- PADDING finale per arrivare a 384
};

static_assert(sizeof(CameraUBOData) == 384, 
    "CameraUBOData size mismatch — check padding");

struct GBuffer
{
    GLuint fbo;

    GLuint normal;    // encoded normal
    GLuint albedo;    // base color
    GLuint material;  // roughness/metallic/AO/emissive

    GLuint depth;

    uint32_t width;
    uint32_t height;
};

class OpenGLRenderer
{
public:
    OpenGLRenderer(ResourceManager& resources,
        const std::string& geometryVert,
        const std::string& geometryFrag,
        const std::string& lightingVert,
        const std::string& lightingFrag,
        const std::string& clusterComputeShader);
	~OpenGLRenderer();

    void init();
    void shutdown();
    void onResize(unsigned int width, unsigned int height);
    void render(const Scene& scene,
                            const LightManager& lightManager, 
                            const std::vector<RenderObject>& staticObjects,
                            const std::vector<RenderObject>& quasiStaticObjects,
                            const std::vector<RenderObject>& dynamicSlowObjects,
                            const Camera& camera,
                            const BVHTree& staticBVH,
                            const BVHTree& quasiStaticBVH,
                            const BVHTree& dynamicBVH,
                            const std::vector<RenderObject>& dynamicFastObjects);

private:
    void geometryPass(const Scene& scene, 
                        const std::unordered_map<MeshHandle, 
                        std::vector<std::pair<uint32_t, MaterialHandle>>>& groups);

    void lightingPass();

    void setupGBuffer();


    ResourceManager& m_resources;

    // OpenGLShaderProgram m_shader;
    OpenGLShaderProgram m_geometryShader;
    OpenGLShaderProgram m_lightingShader;
    OpenGLShaderProgram m_clusterComputeShader;

    bool m_clustersDirty = true;

    GBuffer m_gBuffer;

    unsigned int        m_width = 1280;
    unsigned int        m_height = 720;

	GLuint m_cameraUBO;
	GLuint m_transformUBO;

    GLuint m_clusterAABBSSBO;
    GLuint m_lightGridSSBO;
    GLuint m_lightIndexListSSBO;

	mat4 m_transformStagingBuffer[MAX_RENDER_OBJECTS];
};