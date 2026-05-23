#pragma once
#include "core/scene/LightManager.h"
#include "opengl/OpenGLConfig.h"
#include "ShadowAtlas.h"
#include "core/scene/Frustum.h"
#include "core/assets/Camera.h"
#include "core/scene/Scene.h"
#include "core/resourcemanager/ResourceManager.h"
#include "opengl/OpenGLShaderProgram.h"

class ShadowEngine
{
public:
    void init();
    void update(Frustum frustum, LightManager& lightManager, Camera camera);
    void renderShadowPass(const Scene& scene, ResourceManager& resources);
    void bindForLightingPass(int atlasUnit) const;
    void shutdown();

    const GLuint getCasterSSBO() const { return m_casterSSBO; }
    uint32_t getCasterCount()    const { return static_cast<uint32_t>(m_casters.size()); }

private:
    float    getScreenImportance(float radius, float distance, float spotFactor);
    uint32_t getTileSize(float screenImportance);

    ShadowAtlas    m_shadowAtlas;

    std::unordered_map<LightHandle, uint32_t> m_lightToTile;
    std::vector<GPUShadowCaster>              m_casters;

    GLuint m_casterSSBO = 0;
    bool   m_dirty      = false;

    OpenGLShaderProgram* m_depthShader = nullptr;
};