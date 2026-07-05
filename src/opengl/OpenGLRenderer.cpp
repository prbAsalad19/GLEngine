#include "OpenGLRenderer.h"
#include "core/assets/Material.h"
#include "core/scene/RenderObject.h"
#include "opengl/ShadowMapping/ShadowMapping.h"
#include <unordered_map>
#include <vector>


OpenGLRenderer::OpenGLRenderer(ResourceManager& resources,
    const std::string& geometryVert,
    const std::string& geometryFrag,
    const std::string& lightingVert,
    const std::string& lightingFrag,
    const std::string& clusterComputeShader,
    const std::string& lightCullingShader)
    : m_resources(resources)
    , m_geometryShader(geometryVert, geometryFrag)
    , m_lightingShader(lightingVert, lightingFrag)
    , m_clusterComputeShader(clusterComputeShader, OpenGLShaderProgram::ShaderType::Compute)
    , m_lightCullingShader(lightCullingShader, OpenGLShaderProgram::ShaderType::Compute)
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

    // LightSSBO: header 16 byte (count + padding) + array GPULight
    glCreateBuffers(1, &m_lightSSBO);
    glNamedBufferStorage(m_lightSSBO, 16 + sizeof(GPULight) * MAX_LIGHTS, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_lightSSBO);

    // GlobalCounter: singolo uint
    glCreateBuffers(1, &m_globalCounterSSBO);
    glNamedBufferStorage(m_globalCounterSSBO, sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_globalCounterSSBO);

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

    m_clusterComputeShader.bind();
    m_clusterComputeShader.setUniformBlockBinding("CameraData", CAMERA_UBO_BINDING);
    m_clusterComputeShader.unbind();

    m_lightCullingShader.bind();
    m_lightCullingShader.setUniformBlockBinding("CameraData", CAMERA_UBO_BINDING);
    m_lightCullingShader.unbind();

    glCreateVertexArrays(1, &m_fullscreenVAO);
    //-- GBUFFER SETUP --------------------------------------------------------
    setupGBuffer();

        //-- FBO CHECK ------------------------------------------------------------
    if (glCheckNamedFramebufferStatus(m_gBuffer.fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[OpenGLRenderer] GBuffer not complete!\n";
    }

    m_shadowEngine.init();

#ifdef ENGINE_DEBUG_UI
    glGenQueries(1, &m_primitivesQuery);
#endif
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
    glDeleteVertexArrays(1, &m_fullscreenVAO);

#ifdef ENGINE_DEBUG_UI
    glDeleteQueries(1, &m_primitivesQuery);
#endif

    m_shadowEngine.shutdown();
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

void OpenGLRenderer::render(LightManager& lightManager,
                            RenderBuckets& buckets, 
                            const Camera& camera,
                            const BVHTree& staticBVH,
                            const BVHTree& quasiStaticBVH,
                            const BVHTree& dynamicBVH)
{
    // ripristina sempre lo stato 3D
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    size_t numObjects = buckets.staticObjects.size() + buckets.quasiStaticObjects.size() + 
        buckets.dynamicSlowObjects.size() + buckets.dynamicFastObjects.size();
    if (numObjects == 0) return;

    // ── matrici camera ────────────────────────────────────────────────────────
    mat4 view              = camera.getViewMatrix();
    mat4 projection        = camera.getProjectionMatrix(
        static_cast<float>(m_width) / static_cast<float>(m_height));
    mat4 inverseView       = camera.getInverseViewMatrix();
    mat4 inverseProjection = camera.getInverseProjectionMatrix(
        static_cast<float>(m_width) / static_cast<float>(m_height));
    mat4 vp = projection * view;

    // ── frustum culling ───────────────────────────────────────────────────────
    Frustum frustum;
    frustum.extractFromMatrix(vp);

    m_totalObjectsThisFrame = static_cast<uint32_t>(numObjects);

    std::unordered_map<MeshHandle, std::vector<std::pair<uint32_t, MaterialHandle>>> groups;

    uint32_t globalTransformIndex = 0;

    auto processVisibleObjects = [&](const std::vector<RenderObject>& bucket,
                                  const std::vector<uint32_t>& visibleIndices) 
    {
        for (uint32_t idx : visibleIndices)
        {
            if (globalTransformIndex >= MAX_RENDER_OBJECTS) break;
            const RenderObject& obj = bucket[idx];
            m_transformStagingBuffer[globalTransformIndex] = obj.transform.getMatrix();
            groups[obj.mesh].push_back({ globalTransformIndex, obj.material });
            ++globalTransformIndex;
        }
    };

    std::vector<uint32_t> staticVisibleIndices;
    staticBVH.query(frustum, buckets.staticObjects, staticVisibleIndices);
    processVisibleObjects(buckets.staticObjects, staticVisibleIndices);

    std::vector<uint32_t> quasiVisibleIndices;
    quasiStaticBVH.query(frustum, buckets.quasiStaticObjects, quasiVisibleIndices);
    processVisibleObjects(buckets.quasiStaticObjects, staticVisibleIndices);

    std::vector<uint32_t> dynamicSlowVisibleIndices;
    dynamicBVH.query(frustum, buckets.dynamicSlowObjects, dynamicSlowVisibleIndices);
    processVisibleObjects(buckets.dynamicSlowObjects, staticVisibleIndices);

    std::vector<uint32_t> dynamicFastVisibleIndices;
    for (uint32_t i = 0; i < buckets.dynamicFastObjects.size(); ++i)
    {
        const RenderObject& obj = buckets.dynamicFastObjects[i];
        AABB world = AABB::transform(m_resources.getMeshAABB(obj.mesh), obj.transform.getMatrix());
        if (frustum.intersectsAABB(world))
            dynamicFastVisibleIndices.push_back(i);
    }
    processVisibleObjects(buckets.dynamicFastObjects, dynamicFastVisibleIndices);

    _visibleIndices = globalTransformIndex;

    // ── upload camera UBO ─────────────────────────────────────────────────────
    CameraUBOData cameraData;
    memcpy(cameraData.view,               view.entries,              sizeof(float) * 16);
    memcpy(cameraData.projection,         projection.entries,        sizeof(float) * 16);
    memcpy(cameraData.viewProjection,     vp.entries,                sizeof(float) * 16);
    memcpy(cameraData.inverseView,        inverseView.entries,       sizeof(float) * 16);
    memcpy(cameraData.inverseProjection,  inverseProjection.entries, sizeof(float) * 16);
    cameraData.cameraPosition[0] = camera.position.entries[0];
    cameraData.cameraPosition[1] = camera.position.entries[1];
    cameraData.cameraPosition[2] = camera.position.entries[2];
    cameraData.zNear        = camera.nearPlane;
    cameraData.zFar         = camera.farPlane;
    cameraData.gridRes[0]   = CLUSTERS_X;
    cameraData.gridRes[1]   = CLUSTERS_Y;
    cameraData.gridRes[2]   = CLUSTERS_Z;
    cameraData.gridRes[3]   = 0;
    cameraData.screenRes[0] = static_cast<float>(m_width);
    cameraData.screenRes[1] = static_cast<float>(m_height);
    glNamedBufferSubData(m_cameraUBO, 0, sizeof(CameraUBOData), &cameraData);

    // ── cluster build (solo se dirty) ─────────────────────────────────────────
    if (m_clustersDirty)
    {
        m_clusterComputeShader.bind();
        uint32_t numGroups = (TOTAL_CLUSTERS + 63) / 64;
        m_clusterComputeShader.dispatch(numGroups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        m_clusterComputeShader.unbind();
        m_clustersDirty = false;
    }

    // ── upload light SSBO (solo se dirty) ────────────────────────────────────
    if (lightManager.isDirty())
    {
        const auto& lights = lightManager.getGPULights();
        uint32_t count     = lightManager.count();

        // header: lightCount (uint) + 3 float padding + array di GPULight
        glNamedBufferSubData(m_lightSSBO, 0,
            sizeof(uint32_t), &count);
        glNamedBufferSubData(m_lightSSBO, 16,
            sizeof(GPULight) * count, lights.data());

        lightManager.clearDirty();
    }

    // ── light culling — ogni frame ────────────────────────────────────────────
    // reset contatore atomico
    uint32_t zero = 0;
    glNamedBufferSubData(m_globalCounterSSBO, 0, sizeof(uint32_t), &zero);

    m_lightCullingShader.bind();
    uint32_t numGroups = (TOTAL_CLUSTERS + 63) / 64;
    m_lightCullingShader.dispatch(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    m_lightCullingShader.unbind();

    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    
    Frustum lightFrustum;
    lightFrustum.extractFromMatrix(vp);
    m_shadowEngine.update(lightFrustum, lightManager, camera);
    //m_shadowEngine.renderShadowPass(buckets, m_resources);

#ifdef ENGINE_DEBUG_UI
    m_drawCalls = 0;
    m_drawnTriangles = 0;

    // ── geometry e lighting pass ──────────────────────────────────────────────
    glBeginQuery(GL_PRIMITIVES_GENERATED, m_primitivesQuery);
#endif
    geometryPass(groups);

#ifdef ENGINE_DEBUG_UI
    glEndQuery(GL_PRIMITIVES_GENERATED);
    glGetQueryObjectuiv(m_primitivesQuery, GL_QUERY_RESULT, &m_drawnTriangles);
#endif
    lightingPass();
}

void OpenGLRenderer::geometryPass(const std::unordered_map<MeshHandle, std::vector<std::pair<uint32_t, MaterialHandle>>>& groups)
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

        ++m_drawCalls;
    }

    m_geometryShader.unbind();

    //-- IMPORTANT: restore state for next pass ------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::lightingPass()
{
    // 1. Output al framebuffer di default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
    
    // In un deferred, il lighting pass non deve pulire il depth (già scritto nel geom pass)
    // ma solo il colore.
    glClear(GL_COLOR_BUFFER_BIT); 

    // 2. Stato della GPU: Disabilita scrittura depth e blending (solitamente)
    glDepthMask(GL_FALSE); 
    glDisable(GL_DEPTH_TEST); // Non serve testare la depth per un quad fullscreen
    glDisable(GL_BLEND);

    m_lightingShader.bind();
    
    if (m_debugModeLast != m_debugMode)
    {
        m_lightingShader.setInt("u_debugMode", m_debugMode);
        m_debugModeLast = m_debugMode;
    }

    // 3. Bind delle Texture dal GBuffer (usando i tuoi membri m_gBuffer)
    // Slot 0: Normals (RG16F)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer.normal);
    m_lightingShader.setInt("u_gNormal", 0);

    // Slot 1: Albedo (RGBA8)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer.albedo);
    m_lightingShader.setInt("u_gAlbedo", 1);

    // Slot 2: Material (Roughness, Metallic, AO)
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer.material);
    m_lightingShader.setInt("u_gMaterial", 2);

    // Slot 3: Depth (Fondamentale per ricostruire la posizione nel fragment shader)
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer.depth);
    m_lightingShader.setInt("u_gDepth", 3);


    m_shadowEngine.bindForLightingPass(4);
    m_lightingShader.setInt("u_shadowAtlas", 6);

    // 4. Binding degli SSBO per il Clustered Lighting
    // Usiamo i binding index definiti nella tua init()
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_lightGridSSBO);      // Light Grid
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_lightIndexListSSBO); // Index List
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_lightSSBO);          // Lights Data
    
    glBindVertexArray(m_fullscreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // 6. Ripristino stato
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    m_lightingShader.unbind();
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

void OpenGLRenderer::setDebugMode(int mode)
{
    m_debugMode = mode;
}
