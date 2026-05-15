#include "ShadowMapping.h"
#include "core/scene/Frustum.h"

void ShadowEngine::update(Frustum frustum, LightManager& lightManager, Camera camera)
{
    uint32_t i = 0;
    for (auto& light : lightManager.getGPULights())
    {
        if (light.type == static_cast<uint32_t>(LightType::Point)) //point
        {
            if (frustum.intersectsSphere( {light.position[0], light.position[1], light.position[2] },
                                             light.radius))
            {
                float distance = Vector3::dist({light.position[0], light.position[1], light.position[2] }, camera.position);
                uint32_t res = getTileSize(getScreenImportance(light.radius, distance, 1.0f));
            }
        }
        else if (light.type == static_cast<uint32_t>(LightType::Spot)) //spot
        {
            if (frustum.intersectsSphere( {light.position[0], light.position[1], light.position[2] },
                                             light.radius))
            {
                float distance = Vector3::dist({light.position[0], light.position[1], light.position[2] }, camera.position);
                uint32_t res = getTileSize(getScreenImportance(light.radius, distance, 1.0f - light.outerAngleCos));

            }
        }
        else if (light.type == static_cast<uint32_t>(LightType::Directional))
        {
            if (lightManager.isPrimary(i))
            {
                uint32_t res = m_shadowAtlas.m_resolutions[3]; //highest for the primary, no shadow for the non primary
            }
        }
        i++;
    }
}

float ShadowEngine::getScreenImportance(float radius, float distance, float spotFactor)
{
    return (radius / distance) * spotFactor;
}

uint32_t ShadowEngine::getTileSize(float screenImportance)
{
    float importance = std::clamp(screenImportance, 0.0f, 1.0f);

    uint32_t res;

    if (importance > 0.6f) res = m_shadowAtlas.m_resolutions[2];
    else if (importance > 0.3f) res = m_shadowAtlas.m_resolutions[1];
    else res = m_shadowAtlas.m_resolutions[0];
    return res;
}
