#include "DebugPanel.h"
#include "core/AudioPipeline/AudioEngine.h"
#include "core/resourcemanager/ResourceHandle.h"
#include "core/scene/LightManager.h"
#include "imgui.h"
#include <string>
#include <vector>

void DebugPanel::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);  // true = installa i callback automaticamente
    ImGui_ImplOpenGL3_Init("#version 460");
}

void DebugPanel::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugPanel::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugPanel::render(Camera& camera,
                LightManager& lightManager,
                AudioEngine& audioEngine,
                OpenGLRenderer& renderer,
                const BVHTree& staticBVH,
                const BVHTree& dynamicBVH)
{
    ImGui::Begin("Debug Panel");
    
    if (ImGui::BeginTabBar("MainTabBar"))
    {
        if (ImGui::BeginTabItem("Camera"))
        {
            drawCameraPanel(camera);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Lights"))
        {
            drawLightPanel(lightManager);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("BVH"))
        {
            drawBVHPanel(staticBVH, dynamicBVH);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Audio"))
        {
            drawAudioPanel(audioEngine);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Renderer"))
        {
            drawRendererPanel(renderer);
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void DebugPanel::drawCameraPanel(Camera& camera)
{
    ImGui::SeparatorText("Transform");
    ImGui::DragFloat3("Position", camera.position.entries, 0.1f);
    ImGui::DragFloat3("Target", camera.target.entries, 0.1f);

    ImGui::SeparatorText("Projections");
    ImGui::SliderFloat("FOV", &camera.fovDegrees, 10.0f, 120.0f);
    ImGui::DragFloat("Near Plane", &camera.nearPlane,  0.001f, 0.001f, 10.0f);
    ImGui::DragFloat("Far Plane",  &camera.farPlane,   1.0f,   1.0f,   10000.0f);

    // matrici — read only, solo visualizzazione
    ImGui::SeparatorText("View Matrix");
    drawMatrix4x4(camera.getViewMatrix());

    ImGui::SeparatorText("Projection Matrix");
    drawMatrix4x4(camera.getProjectionMatrix(
        static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight)));
}

void DebugPanel::drawLightPanel(LightManager& lightManager)
{
    const auto& handles  = lightManager.getActiveHandles();
    const auto& cpuDescs = lightManager.getCPUDescs();
    //const auto& slots    = lightManager.getSl;

    if (handles.empty())
    {
        ImGui::TextDisabled("No active lights");
        return;
    }

    for (uint32_t i = 0; i < handles.size(); i++)
    {
        const LightHandle& handle = handles[i];
        uint32_t gpuIdx = lightManager.getSlot(handle).gpuIndex;
        
        // label univoca per ogni luce
        std::string label = "Light " + std::to_string(i) 
                          + " (slot " + std::to_string(handle.slot) + ")";
        
        if (ImGui::CollapsingHeader(label.c_str()))
        {
            // lavoriamo su una copia locale
            LightDesc desc = cpuDescs[gpuIdx];
            bool changed = false;

            // tipo luce — read only per ora, cambiarlo richiede remove+add
            const char* typeNames[] = { "Point", "Spot", "Directional" };
            ImGui::LabelText("Type", "%s", typeNames[(int)desc.type]);

            ImGui::SeparatorText("Transform");
            changed |= ImGui::DragFloat3("Position##l",  desc.position.entries,  0.1f);
            
            if (desc.type == LightType::Spot || desc.type == LightType::Directional)
                changed |= ImGui::DragFloat3("Direction##l", desc.direction.entries, 0.01f);

            ImGui::SeparatorText("Photometry");
            changed |= ImGui::ColorEdit3("Color##l",      desc.color.entries);
            changed |= ImGui::DragFloat("Intensity##l",  &desc.intensity,  0.1f, 0.0f, 1000.0f);
            changed |= ImGui::DragFloat("Radius##l",     &desc.radius,     0.1f, 0.0f, 500.0f);

            if (desc.type == LightType::Spot)
            {
                ImGui::SeparatorText("Spot Angles");
                changed |= ImGui::SliderFloat("Inner Angle##l", &desc.innerAngle, 0.0f, desc.outerAngle);
                changed |= ImGui::SliderFloat("Outer Angle##l", &desc.outerAngle, desc.innerAngle, 90.0f);
            }

            // manda l'update solo se qualcosa è cambiato
            if (changed)
                lightManager.update(handle, desc);
        }
    }
}

void DebugPanel::drawMatrix4x4(const mat4& m)
{
    if (ImGui::BeginTable("mat4", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit))
    {
        for (int row = 0; row < 4; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < 4; col++)
            {
                ImGui::TableSetColumnIndex(col);
                // colonna major: entries[col*4 + row]
                ImGui::Text("%.3f", m.entries[col * 4 + row]);
            }
        }
        ImGui::EndTable();
    }
}

void DebugPanel::drawBVHPanel(const BVHTree& staticBVH, const BVHTree& dynamicBVH)
{
    return;
}
void DebugPanel::drawAudioPanel(AudioEngine& audioEngine)
{
    ImGui::SeparatorText("Sources");
    std::vector<AudioSource> sources = audioEngine.getSources();
    for (int i = 0; i < sources.size(); ++i)
    {
        AudioSource source = sources[i];
        bool changed = false;

        std::string Label = "Source "+ std::to_string(i);
        if (ImGui::CollapsingHeader(Label.c_str()))
        {
            ImGui::SeparatorText("Transform");
            changed |= ImGui::DragFloat3("Position", source.position.entries, 0.01f);
            changed |= ImGui::DragFloat3("velocity", source.velocity.entries, 0.01f);

            ImGui::SeparatorText("Audio");
            //changed |= ImGui::DragFloat("Playhead position", &(float&)source.playhead, 0.01f);
            changed |= ImGui::DragFloat("Volume", &source.volume, 0.01f);
            changed |= ImGui::Checkbox("Playing", &source.playing);
            changed |= ImGui::Checkbox("Looping", &source.looping);

            if (changed)
            {
                std::vector<size_t> slots = audioEngine.getActiveSourcesSlots();
                
                AudioSourceHandle handle;
                handle.slot = slots[i];
                handle.generation = audioEngine.getSourceSlotGeneration(handle.slot);
                audioEngine.updateSource(handle, source);
            }
        }
    }
}
void DebugPanel::drawRendererPanel(OpenGLRenderer& renderer)
{
    return;
}

void DebugPanel::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
