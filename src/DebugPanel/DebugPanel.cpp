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
                const BVHTree& quasiStaticBVH,
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
            size_t visCount = renderer.getVisibleCount();
            drawBVHPanel(staticBVH, quasiStaticBVH, dynamicBVH, visCount);
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

void DebugPanel::drawBVHPanel(const BVHTree& staticBVH, const BVHTree& quasiStaticBVH, const BVHTree& dynamicBVH, const size_t visibleIndicesSize)
{
    ImGui::SeparatorText("Nodes info");
    ImGui::LabelText("static", "%zu", staticBVH.getNodes().size());
    ImGui::LabelText("quasiynamic", "%zu", quasiStaticBVH.getNodes().size());
    ImGui::LabelText("dynamic", "%zu", dynamicBVH.getNodes().size());
    ImGui::SeparatorText("BVH Info");
    std::string bvhInfo = std::to_string(visibleIndicesSize) 
                                        + "/" 
                                        + std::to_string(staticBVH.getNodes().size() 
                                                        + quasiStaticBVH.getNodes().size() 
                                                        + dynamicBVH.getNodes().size());
    ImGui::LabelText("Visible nodes", "%s", bvhInfo.c_str()); 
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
// ==========================================
    // 1. GPU INFO
    // ==========================================
    if (ImGui::CollapsingHeader("GPU Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Info stringhe (ottenibili tramite glGetString)
        ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));
        
        ImGui::Separator();
        
        // Limiti Hardware
        GLint maxUboSize = 0, maxSsboSize = 0;
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUboSize);
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSsboSize);
        ImGui::Text("Max UBO Size: %.2f KB", maxUboSize / 1024.0f);
        ImGui::Text("Max SSBO Size: %.2f MB", maxSsboSize / (1024.0f * 1024.0f));

        // Memoria NVIDIA (Opzionale/Condizionale)
        // Nota: Definisci questi enum se non sono presenti negli header standard
        #define GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 0x9048
        #define GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
        
        GLint totalMem = 0, availMem = 0;
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMem);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availMem);
        
        if (totalMem > 0) // Se l'estensione è supportata ed è NVIDIA
        {
            ImGui::Separator();
            ImGui::Text("VRAM Totale: %.2f MB", totalMem / 1024.0f);
            ImGui::Text("VRAM Disponibile: %.2f MB", availMem / 1024.0f);
            ImGui::Text("VRAM Utilizzata: %.2f MB", (totalMem - availMem) / 1024.0f);
        }
    }

    // ==========================================
    // 2. FRAME STATS & CULLING
    // ==========================================
    if (ImGui::CollapsingHeader("Frame Stats", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Stats base passate o lette dal renderer
        float fps = renderer.fps; // Sostituisci con la tua logica
        float dtMs = renderer.dt * 1000.0f;
        
        ImGui::Text("Performance: %.1f FPS (%.2f ms)", fps, dtMs);
        ImGui::Separator();
        
        // Contatori geometrici
        ImGui::Text("Draw Calls: %d", renderer.getDrawCalls());
        ImGui::Text("Triangoli Generati: %d", renderer.getDrawnTriangles()); // Se usi GL_PRIMITIVES_GENERATED
        
        ImGui::Separator();
        
        // Scena e Culling
        int totalObjects = renderer.getTotalObjects();
        int visibleObjects = renderer.getVisibleObjects();
        int culledObjects = totalObjects - visibleObjects;
        
        ImGui::Text("Oggetti in Scena: %d", totalObjects);
        ImGui::Text("Oggetti Visibili (BVH): %d", visibleObjects);
        ImGui::Text("Oggetti Culled: %d (%.1f%%)", culledObjects, totalObjects > 0 ? (culledObjects / (float)totalObjects) * 100.0f : 0.0f);
    }

    // ==========================================
    // 3. GBUFFER PREVIEW
    // ==========================================
    if (ImGui::CollapsingHeader("GBuffer Info & Previews"))
    {
        // Info Generiche
        ImGui::Text("Risoluzione GBuffer: %dx%d", renderer.getGBufferWidth(), renderer.getGBufferHeight());
        ImGui::Text("Formati: Albedo (RGBA8) | Normals (RG16F) | Material (RGBA8) | Depth (D24)");
        
        ImGui::Spacing();
        
        // Layout a griglia per le miniature delle texture
        ImVec2 previewSize(320, 180); // 16:9 proporzionale
        
        ImVec2 uv0(0.0f, 1.0f);
        ImVec2 uv1(1.0f, 0.0f);

        if (ImGui::TreeNode("Previews"))
        {
            // Riga 1: Albedo e Normals
            ImGui::BeginGroup();
            ImGui::Text("Albedo");
            ImGui::Image((ImTextureID)(intptr_t)renderer.getGBufferAlbedoTexID(), previewSize, uv0, uv1);
            ImGui::EndGroup();
            
            ImGui::SameLine();
            
            ImGui::BeginGroup();
            ImGui::Text("Normals (Ottaedrica)");
            ImGui::Image((ImTextureID)(intptr_t)renderer.getGBufferNormalTexID(), previewSize, uv0, uv1);
            ImGui::EndGroup();
            
            // Riga 2: Depth e Material
            ImGui::BeginGroup();
            ImGui::Text("Depth");
            ImGui::Image((ImTextureID)(intptr_t)renderer.getGBufferDepthTexID(), previewSize, uv0, uv1);
            ImGui::EndGroup();
            
            ImGui::SameLine();
            
            ImGui::BeginGroup();
            ImGui::Text("Material (R/M/AO)");
            ImGui::Image((ImTextureID)(intptr_t)renderer.getGBufferMaterialTexID(), previewSize, uv0, uv1);
            ImGui::EndGroup();
            
            ImGui::TreePop();
        }
    }

    // ==========================================
    // 4. CLUSTER GRID
    // ==========================================
    if (ImGui::CollapsingHeader("Cluster Grid"))
    {
        // Costanti esposte
        ImGui::Text("Dimensioni Griglia: %d x %d x %d", CLUSTERS_X, CLUSTERS_Y, CLUSTERS_Z);
        ImGui::Text("Totale Cluster: %d", TOTAL_CLUSTERS);
        
        // ImGui::Separator();
        // // Qui potrai mettere info avanzate estratte dalla GPU in futuro
        // ImGui::Text("Luci totali attive (LightManager): %d", lightManager.count());
    }

    // ==========================================
    // 5. SHADOW ENGINE
    // ==========================================
    if (ImGui::CollapsingHeader("Shadow Engine"))
    {
        // ImGui::Text("Shadow Casters attivi: %d", renderer.getShadowCasterCount());
        // // Esempio se hai una dimensione fissa dell'atlas
        // ImGui::Text("Shadow Atlas Size: 4096 x 4096"); 
    }

    // ==========================================
    // 6. INTERACTIVE CONTROLS
    // ==========================================
    if (ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // VSync
        bool vsync = renderer.isVsyncEnabled(); // o variabile locale sincronizzata
        if (ImGui::Checkbox("Enable VSync", &vsync))
        {
            renderer.enVsync(vsync);
        }
        
        // Wireframe
        static bool wireframe = false; // Gestisci lo stato persistente dove preferisci
        if (ImGui::Checkbox("Wireframe Mode", &wireframe))
        {
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        }
        
        // Frustum Culling Toggle
        bool frustumCulling = renderer.isFrustumCullingEnabled();
        if (ImGui::Checkbox("Enable Frustum Culling (BVH)", &frustumCulling))
        {
            renderer.setFrustumCullingEnabled(frustumCulling);
        }
        
        ImGui::Separator();
        
        // Clear Color Live
        static float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        if (ImGui::ColorEdit4("Clear Color", clearColor))
        {
            glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        }
        
        ImGui::Separator();
        
        // Debug Mode Selector (Radio Buttons o Combo)
        int currentDebugMode = renderer.getDebugMode(); 
        ImGui::Text("Debug Render Mode:");
        
        // Array di stringhe comodo per il mapping dei tuoi m_debugMode
        const char* debugModes[] = { 
            "Disabled (Lit)",   // 0
            "Albedo",           // 1
            "Normals",          // 2
            "Depth",            // 3
            "Material (R/M/AO)",// 4
            "Cluster Heatmap",  // 5
            "Shadow Cascades",  // 6
            "BVH Bounds"        // 7
        };
        
        // Soluzione pulita con ImGui::Combo
        if (ImGui::Combo("##debugModeCombo", &currentDebugMode, debugModes, IM_ARRAYSIZE(debugModes)))
        {
            renderer.setDebugMode(currentDebugMode);
        }
        
        // In alternativa, se preferisci i Radio Button espliciti uno sotto l'altro:
        /*
        for (int i = 0; i < IM_ARRAYSIZE(debugModes); i++)
        {
            if (ImGui::RadioButton(debugModes[i], currentDebugMode == i))
            {
                renderer.setDebugMode(i);
            }
        }
        */
    }
}

void DebugPanel::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
