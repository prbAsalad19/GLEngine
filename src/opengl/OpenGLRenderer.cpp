#include "OpenGLRenderer.h"
#include "core/assets/Material.h"
#include "core/scene/RenderObject.h"
#include <vector>


OpenGLRenderer::OpenGLRenderer(ResourceManager& resources,
    const std::string& vertPath,
    const std::string& fragPath)
    : m_resources(resources)
    , m_shader(vertPath, fragPath)
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
	//-- OpenGL state setup (depth test, blending, culling) ───────────────────────────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEBUG_OUTPUT);

	//-- UBO setup ────────────────────────────────────────────────────────────────────────────────────────────────
	glCreateBuffers(1, &m_cameraUBO);
	glNamedBufferStorage(m_cameraUBO, sizeof(CameraUBOData), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_UBO_BINDING, m_cameraUBO);

    glCreateBuffers(1, &m_transformUBO);
    glNamedBufferStorage(m_transformUBO, sizeof(mat4) * MAX_RENDER_OBJECTS, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM_UBO_BINDING, m_transformUBO);


    m_shader.bind();
	m_shader.setUniformBlockBinding("CameraData", CAMERA_UBO_BINDING);
	m_shader.setUniformBlockBinding("TransformData", TRANSFORM_UBO_BINDING);

    m_shader.setInt("material", 0);
    m_shader.setInt("mask", 1);
    m_shader.setInt("useMaskAlpha", 0);
	m_shader.unbind();
}

void OpenGLRenderer::shutdown()
{
    glDeleteBuffers(1, &m_cameraUBO);
	glDeleteBuffers(1, &m_transformUBO);
}

void OpenGLRenderer::onResize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}

void OpenGLRenderer::render(const Scene& scene,
                            const std::vector<RenderObject>& staticObjects,
                            const std::vector<RenderObject>& quasiStaticObjects,
                            const std::vector<RenderObject>& dynamicSlowObjects, 
                            const Camera& camera,
                            const BVHTree& staticBVH,
                            const BVHTree& quasiStaticBVH,
                            const BVHTree& dynamicBVH,
                            const std::vector<RenderObject>& dynamicFastObjects)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ripristina sempre lo stato 3D
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    if (scene.objects.empty()) return;

    // estrai frustum
    mat4 view       = camera.getViewMatrix();
    mat4 projection = camera.getProjectionMatrix(
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

    std::cout << "visibili: " << visibleIndices.size() 
          << " / " << scene.objects.size() << "\n";

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

    // Upload transforms — tutti in un colpo solo
    if (index > 0)
    {
        glNamedBufferSubData(m_transformUBO, 0,
            sizeof(mat4) * index,
            m_transformStagingBuffer);
    }

    // Upload camera UBO
    CameraUBOData cameraData;
    memcpy(cameraData.view, view.entries, sizeof(float) * 16);
    memcpy(cameraData.projection, projection.entries, sizeof(float) * 16);
    memcpy(cameraData.viewProjection, vp.entries, sizeof(float) * 16);
    cameraData.cameraPosition[0] = camera.position.entries[0];
    cameraData.cameraPosition[1] = camera.position.entries[1];
    cameraData.cameraPosition[2] = camera.position.entries[2];
    cameraData._padding = 0.0f;
    glNamedBufferSubData(m_cameraUBO, 0, sizeof(CameraUBOData), &cameraData);

    // bind shader — sempre, anche se groups è vuoto
    // così lo stato è sempre pulito per l'UI che viene dopo
    m_shader.bind();

    for (auto& [meshHandle, instances] : groups)
    {
        // se visibleIndices era vuoto, groups è vuoto e questo loop non esegue
        OpenGLMesh* mesh = m_resources.getMesh(meshHandle);
        if (!mesh) continue;

        MaterialHandle matHandle = instances[0].second;
        Material* material = m_resources.getMaterial(matHandle);
        if (material)
        {
            auto it = material->textures.find("albedo");
            if (it != material->textures.end())
            {
                OpenGLTexture* tex = m_resources.getTexture(it->second);
                if (tex) tex->use(0);
            }
        }

        std::vector<mat4> groupTransforms;
        groupTransforms.reserve(instances.size());
        for (auto& [tIdx, _] : instances)
            groupTransforms.push_back(m_transformStagingBuffer[tIdx]);

        glNamedBufferSubData(m_transformUBO, 0,
            sizeof(mat4) * groupTransforms.size(),
            groupTransforms.data());

        mesh->drawInstanced(static_cast<uint32_t>(instances.size()));
    }

    // unbind sempre — garantisce stato pulito per l'UI
    m_shader.unbind();
}
