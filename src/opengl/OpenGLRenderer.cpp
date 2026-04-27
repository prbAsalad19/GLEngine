#include "OpenGLRenderer.h"
#include "core/assets/Material.h"
#include "core/scene/RenderObject.h"
#include <vector>


OpenGLRenderer::OpenGLRenderer(ResourceManager& resources,
    const std::string& geometryVert,
    const std::string& geometryFrag,
    const std::string& lightingVert,
    const std::string& lightingFrag,
    const std::string& clusterComputeShader)
    : m_resources(resources)
    , m_geometryShader(geometryVert, geometryFrag)
    , m_lightingShader(lightingVert, lightingFrag)
    , m_clusterComputeShader(clusterComputeShader, OpenGLShaderProgram::ShaderType::Compute)
    , m_cameraUBO(0)
	, m_transformUBO(0)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
    glDeleteBuffers(1, &m_cameraUBO);
    glDeleteBuffers(1, &m_transformUBO);
}

void OpenGLRenderer::init()
{
    //-- OpenGL base state ----------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);


    // 1. Cluster AABB Buffer
    glCreateBuffers(1, &m_clusterAABBSSBO);
    glNamedBufferStorage(m_clusterAABBSSBO, TOTAL_CLUSTERS * sizeof(float) * 8, nullptr, GL_DYNAMIC_STORAGE_BIT);
    // Bindiamo al binding 2 come nello shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_clusterAABBSSBO);

    // 2. Light Grid (offset e count per ogni cluster)
    glCreateBuffers(1, &m_lightGridSSBO);
    glNamedBufferStorage(m_lightGridSSBO, TOTAL_CLUSTERS * sizeof(uint32_t) * 2, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_lightGridSSBO);

    // 3. Light Index List (Assumiamo una media di 100 indici per cluster come limite)
    glCreateBuffers(1, &m_lightIndexListSSBO);
    glNamedBufferStorage(m_lightIndexListSSBO, TOTAL_CLUSTERS * 100 * sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_lightIndexListSSBO);


    //-- UBO setup ------------------------------------------------------------
    glCreateBuffers(1, &m_cameraUBO);
    glNamedBufferStorage(m_cameraUBO, sizeof(CameraUBOData), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_UBO_BINDING, m_cameraUBO);

    glCreateBuffers(1, &m_transformUBO);
    glNamedBufferStorage(m_transformUBO, sizeof(mat4) * MAX_RENDER_OBJECTS, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM_UBO_BINDING, m_transformUBO);


    //-- Shader UBO bindings --------------------------------------------------
    m_geometryShader.bind();
    m_geometryShader.setUniformBlockBinding("CameraData", CAMERA_UBO_BINDING);
    m_geometryShader.setUniformBlockBinding("TransformData", TRANSFORM_UBO_BINDING);
    m_geometryShader.unbind();

    m_lightingShader.bind();
    m_lightingShader.setUniformBlockBinding("CameraData", CAMERA_UBO_BINDING);
    m_lightingShader.unbind();


    //-- GBUFFER SETUP --------------------------------------------------------
    setupGBuffer();

        //-- FBO CHECK ------------------------------------------------------------
    if (glCheckNamedFramebufferStatus(m_gBuffer.fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[OpenGLRenderer] GBuffer not complete!\n";
    }
}

void OpenGLRenderer::shutdown()
{
    glDeleteBuffers(1, &m_cameraUBO);
	glDeleteBuffers(1, &m_transformUBO);
    glDeleteFramebuffers(1, &m_gBuffer.fbo);
    glDeleteTextures(1, &m_gBuffer.normal);
    glDeleteTextures(1, &m_gBuffer.albedo);
    glDeleteTextures(1, &m_gBuffer.material);
    glDeleteTextures(1, &m_gBuffer.depth);  
    glDeleteBuffers(1, &m_clusterAABBSSBO);
    glDeleteBuffers(1, &m_lightGridSSBO);
    glDeleteBuffers(1, &m_lightIndexListSSBO);  
}

void OpenGLRenderer::onResize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
    glDeleteFramebuffers(1, &m_gBuffer.fbo);
    glDeleteTextures(1, &m_gBuffer.normal);
    glDeleteTextures(1, &m_gBuffer.albedo);
    glDeleteTextures(1, &m_gBuffer.material);
    glDeleteTextures(1, &m_gBuffer.depth);

    //-- GBUFFER RELOAD --------------------------------------------------------
    setupGBuffer();

    //-- FBO CHECK ------------------------------------------------------------
    if (glCheckNamedFramebufferStatus(m_gBuffer.fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[OpenGLRenderer] GBuffer not complete!\n";
    }  

    m_clustersDirty =  true;
}

void OpenGLRenderer::render(const Scene& scene,
                            const LightManager& lightManager,
                            const std::vector<RenderObject>& staticObjects,
                            const std::vector<RenderObject>& quasiStaticObjects,
                            const std::vector<RenderObject>& dynamicSlowObjects, 
                            const Camera& camera,
                            const BVHTree& staticBVH,
                            const BVHTree& quasiStaticBVH,
                            const BVHTree& dynamicBVH,
                            const std::vector<RenderObject>& dynamicFastObjects)
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ripristina sempre lo stato 3D
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    if (scene.objects.empty()) return;

    // estrai frustum
    mat4 view       = camera.getViewMatrix();
    mat4 projection = camera.getProjectionMatrix(
        static_cast<float>(m_width) / static_cast<float>(m_height));
    mat4 inverseView = camera.getInverseViewMatrix();
    mat4 inverseProjection = camera.getInverseProjectionMatrix(
        static_cast<float>(m_width) / static_cast<float>(m_height));

    mat4 vp = projection * view;

    Frustum frustum;
    frustum.extractFromMatrix(vp);

    // query BVH
    std::vector<uint32_t> visibleIndices;
    visibleIndices.reserve(scene.objects.size());
    
    staticBVH.query    (frustum, staticObjects,      visibleIndices);
    quasiStaticBVH.query(frustum, quasiStaticObjects, visibleIndices);
    dynamicBVH.query   (frustum, dynamicSlowObjects,  visibleIndices);

    for (uint32_t i = 0; i < dynamicFastObjects.size(); i++)
    {
        AABB world = AABB::transform(
            m_resources.getMeshAABB(dynamicFastObjects[i].mesh),
            dynamicFastObjects[i].transform.getMatrix());

        if (frustum.intersectsAABB(world))
            visibleIndices.push_back(i);
    }

    // const auto& rootNode = bvh.getNodes()[0];
    // std::cout << "=== ROOT AABB ===\n";
    // std::cout << "min: " << rootNode.aabb.bounds[0].x << " " 
    //         << rootNode.aabb.bounds[0].y << " " 
    //         << rootNode.aabb.bounds[0].z << "\n";
    // std::cout << "max: " << rootNode.aabb.bounds[1].x << " " 
    //         << rootNode.aabb.bounds[1].y << " " 
    //         << rootNode.aabb.bounds[1].z << "\n";

    // std::cout << "visibili: " << visibleIndices.size() 
    //       << " / " << scene.objects.size() << "\n";

    // Raggruppa per mesh — stessa mesh = stessa draw call
    // key:   MeshHandle
    // value: lista di { transformIndex, materialHandle }
    std::unordered_map<MeshHandle, std::vector<std::pair<uint32_t, MaterialHandle>>> groups;

    uint32_t index = 0;
    for (uint32_t objIdx : visibleIndices)
    {
        if (index >= MAX_RENDER_OBJECTS) break;
        const RenderObject& obj = scene.objects[objIdx];
        m_transformStagingBuffer[index] = obj.transform.getMatrix();
        groups[obj.mesh].push_back({ index, obj.material });
        ++index;
    }

    // Upload camera UBO
    CameraUBOData cameraData;
    memcpy(cameraData.view, view.entries, sizeof(float) * 16);
    memcpy(cameraData.projection, projection.entries, sizeof(float) * 16);
    memcpy(cameraData.viewProjection, vp.entries, sizeof(float) * 16);
    memcpy(cameraData.inverseView, inverseView.entries, sizeof(float) * 16);
    memcpy(cameraData.inverseProjection, inverseProjection.entries, sizeof(float) * 16);
    cameraData.cameraPosition[0] = camera.position.entries[0];
    cameraData.cameraPosition[1] = camera.position.entries[1];
    cameraData.cameraPosition[2] = camera.position.entries[2];
    cameraData.zNear = camera.nearPlane;
    cameraData.zFar  = camera.farPlane;

    cameraData.gridRes[0] = CLUSTERS_X;
    cameraData.gridRes[1] = CLUSTERS_Y;
    cameraData.gridRes[2] = CLUSTERS_Z;
    cameraData.gridRes[3] = 0; // padding

    cameraData.screenRes[0] = (float)m_width;
    cameraData.screenRes[1] = (float)m_height;

    glNamedBufferSubData(m_cameraUBO, 0, sizeof(CameraUBOData), &cameraData);

    if (m_clustersDirty)
    {
        m_clusterComputeShader.bind();
        uint32_t numGroups = (TOTAL_CLUSTERS + 63) / 64;
        m_clusterComputeShader.dispatch(numGroups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        m_clusterComputeShader.unbind();
        m_clustersDirty = false;
    }

    geometryPass(scene, groups);
}

void OpenGLRenderer::geometryPass(const Scene& scene, 
                                    const std::unordered_map<MeshHandle, std::vector<std::pair<uint32_t, MaterialHandle>>>& groups)
{

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer.fbo);

    GLenum attachments[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, attachments);

    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);

    m_geometryShader.bind();


    for (auto& [meshHandle, instances] : groups)
    {
        OpenGLMesh* mesh = m_resources.getMesh(meshHandle);
        if (!mesh) continue;

        //-- MATERIAL SETUP --------------------------------------------------
        MaterialHandle matHandle = instances[0].second;
        Material* material = m_resources.getMaterial(matHandle);

        if (material)
        {
            //-- ALBEDO (slot 0)
            if (auto it = material->textures.find("albedo"); it != material->textures.end())
            {
                if (OpenGLTexture* tex = m_resources.getTexture(it->second))
                    tex->use(0);
            }

            //-- ROUGHNESS (slot 1)
            if (auto it = material->textures.find("roughness"); it != material->textures.end())
            {
                if (OpenGLTexture* tex = m_resources.getTexture(it->second))
                    tex->use(1);
            }

            //-- METALLIC (slot 2)
            if (auto it = material->textures.find("metallic"); it != material->textures.end())
            {
                if (OpenGLTexture* tex = m_resources.getTexture(it->second))
                    tex->use(2);
            }

            //-- AO (slot 3)
            if (auto it = material->textures.find("ao"); it != material->textures.end())
            {
                if (OpenGLTexture* tex = m_resources.getTexture(it->second))
                    tex->use(3);
            }
        }

        //-- TRANSFORMS ------------------------------------------------------
        std::vector<mat4> groupTransforms;
        groupTransforms.reserve(instances.size());

        for (auto& [tIdx, _] : instances)
            groupTransforms.push_back(m_transformStagingBuffer[tIdx]);

        glNamedBufferSubData(
            m_transformUBO,
            0,
            sizeof(mat4) * groupTransforms.size(),
            groupTransforms.data()
        );

        //-- DRAW ------------------------------------------------------------
        mesh->drawInstanced(static_cast<uint32_t>(instances.size()));
    }

    m_geometryShader.unbind();

    //-- IMPORTANT: restore state for next pass ------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::setupGBuffer()
{
    // FBO
    glCreateFramebuffers(1, &m_gBuffer.fbo);

    // NORMAL (encoded, 2 channels but with RG16F)
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gBuffer.normal);
    glTextureStorage2D(m_gBuffer.normal, 1, GL_RG16F, m_width, m_height);

    // ALBEDO
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gBuffer.albedo);
    glTextureStorage2D(m_gBuffer.albedo, 1, GL_RGBA8, m_width, m_height);

    // MATERIAL (R=roughness, G=metallic, B=AO, A=emissiveMask)
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gBuffer.material);
    glTextureStorage2D(m_gBuffer.material, 1, GL_RGBA8, m_width, m_height);

    // DEPTH
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gBuffer.depth);
    glTextureStorage2D(m_gBuffer.depth, 1, GL_DEPTH_COMPONENT24, m_width, m_height);

    // set parameters
    glTextureParameteri(m_gBuffer.normal, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gBuffer.normal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gBuffer.albedo, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gBuffer.albedo, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gBuffer.material, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gBuffer.material, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // attach
    glNamedFramebufferTexture(m_gBuffer.fbo, GL_COLOR_ATTACHMENT0, m_gBuffer.normal, 0);
    glNamedFramebufferTexture(m_gBuffer.fbo, GL_COLOR_ATTACHMENT1, m_gBuffer.albedo, 0);
    glNamedFramebufferTexture(m_gBuffer.fbo, GL_COLOR_ATTACHMENT2, m_gBuffer.material, 0);
    glNamedFramebufferTexture(m_gBuffer.fbo, GL_DEPTH_ATTACHMENT, m_gBuffer.depth, 0);

    // draw buffers
    GLenum attachments[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glNamedFramebufferDrawBuffers(m_gBuffer.fbo, 3, attachments);
}
