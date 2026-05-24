#include "ShadowMapping.h"
#include "core/scene/Frustum.h"
#include "core/scene/LightManager.h"
#include <iostream>

void ShadowEngine::init()
{
    m_shadowAtlas.init();

    m_casters.reserve(MAX_LIGHTS);

    constexpr size_t SSBO_SIZE = 16 + sizeof(GPUShadowCaster) * MAX_LIGHTS;

    glCreateBuffers(1, &m_casterSSBO);
    glNamedBufferStorage(m_casterSSBO, SSBO_SIZE, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, m_casterSSBO);

    m_depthShader = std::make_unique<OpenGLShaderProgram>(
    "shaders/shadowPass.vert",
    "shaders/shadowPass.frag");
}

void ShadowEngine::update(Frustum frustum, LightManager& lightManager, Camera camera)
{
    const auto& handles   = lightManager.getActiveHandles();
    const auto& gpuLights = lightManager.getGPULights();

    // rimuovi luci che non sono più attive
    for (auto it = m_lightToTile.begin(); it != m_lightToTile.end(); )
    {
        bool stillActive = false;
        for (const auto& h : handles)
            if (h == it->first) { stillActive = true; break; }

        if (!stillActive)
        {
            m_shadowAtlas.freeTile(it->second);
            it = m_lightToTile.erase(it);
            m_dirty = true;
        }
        else ++it;
    }

    m_casters.clear();

    for (const auto& handle : handles)
    {
        const GPULight& light = gpuLights[lightManager.getSlot(handle).gpuIndex];

        // point lights — no shadow map per ora
        if (light.type == static_cast<uint32_t>(LightType::Point)) continue;

        Vector3 lightPos = { light.position[0], light.position[1], light.position[2] };

        // frustum check
        if (!frustum.intersectsSphere(lightPos, light.radius))
        {
            if (m_lightToTile.count(handle))
            {
                m_shadowAtlas.freeTile(m_lightToTile[handle]);
                m_lightToTile.erase(handle);
                m_dirty = true;
            }
            continue;
        }

        // importanza e risoluzione
        float distance   = Vector3::dist(lightPos, camera.position);
        float spotFactor = 1.0f - light.outerAngleCos;
        float importance = getScreenImportance(light.radius, distance, spotFactor);
        uint32_t res     = getTileSize(importance);

        // alloca tile se non esiste
        if (!m_lightToTile.count(handle))
        {
            uint32_t nodeIdx = m_shadowAtlas.addTile(res);
            if (nodeIdx == uint32_t(-1))
            {
                std::cout << "[ShadowEngine] Atlas full, skipping light\n";
                continue;
            }
            m_lightToTile[handle] = nodeIdx;
            m_dirty = true;
        }

        // tile rect in UV
        ShadowAtlas::Node node = m_shadowAtlas.getNode(m_lightToTile[handle]);
        float invAtlas = 1.0f / static_cast<float>(ATLAS_SIZE);

        GPUShadowCaster caster;
        caster.atlasRect[0] = node.tile.x    * invAtlas;
        caster.atlasRect[1] = node.tile.y    * invAtlas;
        caster.atlasRect[2] = node.tile.size * invAtlas;
        caster.atlasRect[3] = node.tile.size * invAtlas;
        caster.lightIndex   = lightManager.getSlot(handle).gpuIndex;
        caster.bias         = 0.005f;

        // lightSpaceMatrix
        Vector3 lightDir = { light.direction[0], light.direction[1], light.direction[2] };
        Vector3 target   = { lightPos.x + lightDir.x,
                             lightPos.y + lightDir.y,
                             lightPos.z + lightDir.z };

        float outerAngleDeg  = acosf(light.outerAngleCos) * 180.0f / PI;
        mat4 lightView        = mat4::create_look_at(lightPos, target);
        mat4 lightProjection  = mat4::create_prospective_projection(
                                    outerAngleDeg * 2.0f, 1.0f, 0.1f, light.radius);
        caster.lightSpaceMatrix = lightProjection * lightView;

        m_casters.push_back(caster);
    }

    if (m_dirty)
    {
        uint32_t count = static_cast<uint32_t>(m_casters.size());
        glNamedBufferSubData(m_casterSSBO, 0, sizeof(uint32_t), &count);
        glNamedBufferSubData(m_casterSSBO, 16, sizeof(GPUShadowCaster) * count, m_casters.data());
        m_dirty = false;
    }
}

void ShadowEngine::renderShadowPass(const Scene& scene, ResourceManager& resources)
{
    if (m_casters.empty()) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowAtlas.fbo);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT); // peter panning fix

    m_depthShader->bind();

    if (!m_depthShader || m_casters.empty()) return;

    for (const auto& caster : m_casters)
    {
        // converti atlasRect UV → pixel
        uint32_t x    = static_cast<uint32_t>(caster.atlasRect[0] * ATLAS_SIZE);
        uint32_t y    = static_cast<uint32_t>(caster.atlasRect[1] * ATLAS_SIZE);
        uint32_t size = static_cast<uint32_t>(caster.atlasRect[2] * ATLAS_SIZE);

        glViewport(x, y, size, size);

        m_depthShader->setMat4("lightSpaceMatrix", caster.lightSpaceMatrix);

        for (const auto& obj : scene.objects)
        {
            OpenGLMesh* mesh = resources.getMesh(obj.mesh);
            if (!mesh) continue;

            // upload transform — slot 0 del TransformUBO
            mat4 model = obj.transform.getMatrix();
            // usiamo un UBO dedicato o un semplice uniform
            m_depthShader->setMat4("model", model);

            mesh->draw();
        }
    }

    m_depthShader->unbind();
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowEngine::bindForLightingPass(int atlasUnit) const
{
    glActiveTexture(GL_TEXTURE0 + atlasUnit);
    glBindTexture(GL_TEXTURE_2D, m_shadowAtlas.texture);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, m_casterSSBO);
}
void ShadowEngine::shutdown()
{
    glDeleteBuffers(1, &m_casterSSBO);
    m_depthShader.reset();
}

float ShadowEngine::getScreenImportance(float radius, float distance, float spotFactor)
{
    if (distance < 0.0001f) return 1.0f;
    return (radius / distance) * spotFactor;
}

uint32_t ShadowEngine::getTileSize(float screenImportance)
{
    float importance = std::clamp(screenImportance, 0.0f, 1.0f);

    if (importance > 0.6f) return m_shadowAtlas.m_resolutions[2];
    if (importance > 0.3f) return m_shadowAtlas.m_resolutions[1];
    return m_shadowAtlas.m_resolutions[0];
}