#pragma once
#include "opengl/OpenGLConfig.h"
#include "ShadowAtlas.h"
#include "core/scene/LightManager.h"
#include "core/scene/Frustum.h"
#include "core/assets/Camera.h"

class ShadowEngine
{
public:

    void update(Frustum frustum, LightManager& lightManager, Camera camera);

private:
    float getScreenImportance(float radius, float distance, float spotFactor); // spot factor has to be 1 if not spot

    uint32_t getTileSize(float screenImportance);

    ShadowAtlas m_shadowAtlas;
    
};
