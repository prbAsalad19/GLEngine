#pragma once
#include "core/CoreConfig.h"

#ifdef ENGINE_DEBUG_UI

#include <imgui-docking/imgui.h>
#include <imgui-docking/backends/imgui_impl_glfw.h>
#include <imgui-docking/backends/imgui_impl_opengl3.h>

#include "core/scene/LightManager.h"
#include "core/assets/Camera.h"
#include "core/bvh/BVHTree.h"
#include "core/AudioPipeline/AudioEngine.h"
#include "core/scene/RenderObject.h"
#include "opengl/OpenGLRenderer.h"

#include <GLFW/glfw3.h>

class DebugPanel
{
public:
    void init(GLFWwindow* window);
    void beginFrame();
    void endFrame();
    void shutdown();

    void render(Camera& camera,
                const double& now,
                LightManager& lightManager,
                AudioEngine& audioEngine,
                OpenGLRenderer& renderer,
                const BVHTree& staticBVH,
                const BVHTree& quasistaticBVH,
                const BVHTree& dynamicBVH);

    void onResize(uint32_t width, uint32_t height)
    {
        m_viewportWidth  = width;
        m_viewportHeight = height;
    }

private:
    void drawCameraPanel(Camera& camera);
    void drawLightPanel(LightManager& lightManager);
    void drawBVHPanel(const BVHTree& staticBVH, const BVHTree& quasiStaticBVH, const BVHTree& dynamicBVH, const size_t visibleIndicesSize);
    void drawAudioPanel(AudioEngine& audioEngine);
    void drawRendererPanel(OpenGLRenderer& renderer, const double& now);
    void drawMatrix4x4(const mat4& m);

    uint32_t m_viewportWidth  = 1280;
    uint32_t m_viewportHeight = 720;
};

#endif