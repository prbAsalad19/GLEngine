#include "core/AudioPipeline/AudioClip.h"
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include "core/scene/RenderObject.h"
#include "core/scene/Scene.h"
#include "core/assets/Camera.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/Transform.h"
#include "opengl/OpenGLRenderer.h"
#include "opengl/OpenGLUIRenderer.h"
#include "core/ui/UICanvas.h"
#include "core/bvh/BVHTree.h"
#include "core/InputManager/InputManager.h"
#include "core/scene/RenderContext.h"
#include "core/scene/LightManager.h"
#include "core/AudioPipeline/AudioEngine.h"
#include "core/AudioPipeline/MiniaudioBackend.h"
#include "core/ecs/EcsSystem.h"
#include "DebugPanel/DebugPanel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

static GLFWwindow* createWindow(int width, int height, const char* title);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    auto ctx = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));

    ctx->renderer->onResize(width, height);
    ctx->ui->onResize(width, height);
}

int main()
{
    constexpr float FIXED_STEP = 1.0f / 50.0f;
    float accumulator = 0.0f;
    double prevTime = glfwGetTime();

    unsigned int currentFps = 0.0f;

    GLFWwindow* window = createWindow(1280, 720, "engine");
    if (!window)
    {
        std::cout << "OpenGL initialization failed.\n";
        return -1;
    }

    ResourceManager resources;
    LightManager lightManager;

    flecs::world world;
    registerComponents(world);

    world.set<ResourceManagerSingleton>({ &resources});
    world.set<LightManagerSingleton>({ &lightManager });
    world.set<RenderBuckets>({});
    registerRenderExtractSystems(world);

    MeshHandle     meshHandle = resources.loadMesh("assets/teapot_with_uv.obj");
    TextureHandle  texHandle = resources.loadTexture("img/whiteTexture.png");
    MaterialHandle matHandle = resources.loadMaterial(texHandle);
    MeshHandle     planeMeshHandle = resources.loadMesh("assets/plane.obj");




    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            Transform t;
            t.position = { i * 4.0f - 4.0f, j * 4.0f - 4.0f, 0.0f };
            t.scale = { 0.5f, 0.5f, 0.5f };
            t.setEuler({ 90.0f, 0.0f, 90.0f });

            world.entity()
                .set<TransformComponent>({ t })
                .set<MeshComponent>({ meshHandle })
                .set<MaterialComponent>({ matHandle })
                .add<StaticTag>();
        }
    }

    // {
    //     Transform t;
    //     t.position = { 0.0f, 0.0f, -1.0f };
    //     t.scale = { 0.5f, 0.5f, 10.0f };
    //     t.setEuler( { 0.0f, 0.0f, 0.0f } );

    //     world.entity()
    //             .set<TransformComponent>({ t })
    //             .set<MeshComponent>({ meshHandle })
    //             .set<MaterialComponent>({ matHandle })
    //             .add<StaticTag>();
    // }
    

    float t = 0.0f;


    Vector3    startPos = { 0.0f, 0.0f, 0.0f };
    Vector3    endPos = { 0.0f, 0.0f, 4.0f };
    Quaternion startRot = Quaternion::identity();
    Quaternion endRot = Quaternion::fromAxisAngle({ 0.0f, 0.0f, 1.0f }, 180.0f);

    LightDesc lightDesc;
    lightDesc.position = { 0.0f, 0.0f, 5.0f };
    lightDesc.color = { 1.0f, 1.0f, 1.0f };
    lightDesc.intensity = 50.0f;
    lightDesc.radius = 10.0f;
    lightDesc.type = LightType::Point;
    //lightManager.add(lightDesc);

    LightDesc lightDesc_;
    lightDesc_.position = { 0.0f, 0.0f, 5.0f };
    lightDesc_.color = { 1.0f, 1.0f, 1.0f };
    lightDesc_.intensity = 50.0f;
    lightDesc_.radius = 10.0f;
    lightDesc_.type = LightType::Point;
    // lightDesc_.innerAngle = 20.0f;
    // lightDesc_.outerAngle = 30.0f;
    // lightDesc_.direction = Vector3::normalize({ 0.0f, 0.0f, 0.0f});

    {
        Transform t;
        t.position = lightDesc_.position;
        world.entity()
            .set<TransformComponent>( {t})
            .set<LightComponent>( { LightHandle{}, lightDesc_ } );
        lightManager.add(lightDesc_);
    }

    Camera camera;
    camera.position = { -5.0f, 0.0f, 3.0f };
    camera.target =  { 0.0f, 0.0f, 0.0f };
    camera.fovDegrees = 45.0f;
    camera.nearPlane = 0.1f;
    camera.farPlane = 1000.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float lastX = 400.0f, lastY = 300.0f;

    int sensitivity = 65;
    float moveSpeed = 5.0f;

    OpenGLRenderer renderer(resources, "shaders/geometryPass.vert", "shaders/geometryPass.frag",
                                    "shaders/lightingPass.vert", "shaders/lightingPass_debug.frag",
                            "shaders/gridGen.comp", "shaders/lightCulling.comp");
    renderer.init();
    std::cout << "[Main] 3D renderer initialized\n";

    world.set<CameraComponent>({ camera }); //setting up the camera in the ecs system

    //audio system setup start ===================================

    ResourcePool<AudioClipTag, AudioClip> audioClipPool;
    AudioClip clip = AudioClip::load("assets/audio/test.wav");
    AudioClipHandle clipHandle =  audioClipPool.insert("assets/audio/test.wav", std::make_unique<AudioClip>(clip));

    Listener listener;
    listener.position = camera.position;
    listener.forward  = { 0.0f, 1.0f, 0.0f };
    listener.up       = { 0.0f, 0.0f, 1.0f };
    listener.velocity = { 0.0f, 0.0f, 0.0f};

    AudioEngine audioEngine(listener, audioClipPool);
    audioEngine.init(std::make_unique<MiniaudioBackend>(), 900);

    AudioSourceHandle sourceHandle = audioEngine.createSource(
        clipHandle,
        { 0.0f, 0.0f, 0.0f },  // posizione
        1.0f,                    // volume
        true                     // looping
    );

    //audioEngine.play(sourceHandle);


    world.set<AudioEngineSingleton>({ &audioEngine }); //setting up the engine in the ecs system

    //audio system setup end ===================================

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    OpenGLUIRenderer UIrenderer(resources, w, h, "shaders/UIShaderv1.txt", "shaders/UIShadervf1.txt");
    UIrenderer.init();
    //// fuori dal while, dopo init
    std::cout << "[Main] UI renderer initialized, canvas loaded\n";

    InputManager inputManager;
    inputManager.init(window);

    RenderContext ctx{ &renderer, &UIrenderer, &inputManager };

    glfwSetWindowUserPointer(window, &ctx);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    UICanvas canvas;

    canvas.loadUI("assets/testUI.json");

    UIrenderer.loadFont("assets/fonts/arial.ttf", 24.0f);
    canvas.bindString("fps", [&]() { return "FPS: " + std::to_string(currentFps);  });

    //debug ------------------------------------------------------------------------------
    //auto printMat = [](const char* name, const mat4& m) {
    //    std::cout << name << ":\n";
    //    for (int row = 0; row < 4; ++row) {
    //        for (int col = 0; col < 4; ++col)
    //            std::cout << m.entries[col * 4 + row] << "\t";
    //        std::cout << "\n";
    //    }
    //    std::cout << "\n";
    //    };

    //printMat("projection", camera.getProjectionMatrix(1280.0f / 720.0f));
    //printMat("view", camera.getViewMatrix());
    //printMat("model", scene.objects[0].transform.getMatrix());
    //debug ------------------------------------------------------------------------------

    // in main, prima di bvh.build()
    // for (int i = 0; i < scene.objects.size(); i++)
    // {
    //     AABB aabb = resources.getMeshAABB(scene.objects[i].mesh);
    //     std::cout << "oggetto " << i << " aabb min: " 
    //             << aabb.bounds[0].x << " " << aabb.bounds[0].y << " " << aabb.bounds[0].z
    //             << " max: "
    //             << aabb.bounds[1].x << " " << aabb.bounds[1].y << " " << aabb.bounds[1].z << "\n";
    // }

    // // build bvh
    // // fuori dal loop — solo per il BVH build iniziale
    // std::vector<RenderObject> staticObjects;
    // std::vector<RenderObject> quasiStaticObjects;
    // std::vector<RenderObject> dynamicSlowObjects;
    // std::vector<RenderObject> dynamicFastObjects;

    // // query per popolarli prima del build
    // world.each([&](flecs::entity e,
    //             TransformComponent& t,
    //             MeshComponent& m,
    //             MaterialComponent& mat)
    // {
    //     RenderObject obj;
    //     obj.mesh = m.handle;
    //     obj.material = mat.handle;
    //     obj.transform = t.transform;

    //     if      (e.has<StaticTag>())      staticObjects.push_back(obj);
    //     else if (e.has<QuasiStaticTag>()) quasiStaticObjects.push_back(obj);
    //     else if (e.has<DynamicSlowTag>()) dynamicSlowObjects.push_back(obj);
    //     else if (e.has<DynamicFastTag>()) dynamicFastObjects.push_back(obj);
    // });
    world.progress(0.0f + 1.0f );
    RenderBuckets buckets = world.get<RenderBuckets>();

    BVHTree staticBVH    (resources, BVHType::Static);
    BVHTree quasiStaticBVH (resources, BVHType::QuasiStatic);
    BVHTree dynamicBVH   (resources, BVHType::DynamicSlow);

    staticBVH.build(buckets.staticObjects);
    quasiStaticBVH.build(buckets.quasiStaticObjects);
    dynamicBVH.build(buckets.dynamicSlowObjects);


    //std::cout << buckets.staticObjects[0].transform.position.x << std::endl;
    // std::cout << "=================================================" << std::endl;
    // std::cout << "Static Objects:" << buckets.staticObjects.size() << std::endl;
    // for (auto& obj : buckets.staticObjects)
    // {
    //     std::cout << obj.mesh.slot << " | " << obj.transform.position.entries << std::endl;
    // }
    // std::cout << "=================================================" << std::endl;


#ifdef ENGINE_DEBUG_UI
    DebugPanel debugPanel;
    debugPanel.init(window);
#endif


    renderer.enVsync(true);

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float  dt = static_cast<float>(now - prevTime);
        prevTime = now;
        static float fpsAccum = 0.0f;
        static float timeAccum = 0.0f;
        static int   fpsCount = 0;

        accumulator += dt;

        while (accumulator >= FIXED_STEP)
        {
            accumulator -= FIXED_STEP;
        }

        glfwPollEvents();
        inputManager.update();

        fpsAccum += 1.0f / dt;
        timeAccum += dt;
        fpsCount++;
        

        //=================================================================================
        if (timeAccum >= 0.25f)  //update FPS every 0.25 seconds
        {
            currentFps = static_cast<unsigned int>(fpsAccum / fpsCount);
            fpsAccum = 0.0f;
            timeAccum = 0.0f;
            fpsCount = 0;
        }

        world.progress(dt);

        if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            bool currentlyLocked = inputManager.isCursorLocked();
            inputManager.setCursorLocked(!currentlyLocked);
        }

#ifdef ENGINE_DEBUG_UI
    bool imguiWantsInput = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
#else
    bool imguiWantsInput = false;
#endif

        if (inputManager.isCursorLocked() && !imguiWantsInput)
        {
            Vector2 mouseDelta = inputManager.getMouseDelta();
            yaw -= mouseDelta.x * (sensitivity / 100.0f);
            pitch -= mouseDelta.y * (sensitivity / 100.0f);  // invert Y for typical FPS controls

            // Clamp pitch to avoid flipping
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            camera.target = camera.position + Vector3::targetFromEuler({ pitch, yaw, 0.0f });

            Vector3 forward = Vector3::normalize(camera.target - camera.position);
            Vector3 worldUp = { 0.0f, 0.0f, 1.0f };
            Vector3 right = Vector3::normalize(Vector3::cross(forward, worldUp));
            
            listener.forward = forward;

            Vector3 moveDir = { 0.0f, 0.0f, 0.0f };
            float currentSpeed = moveSpeed * dt;

            if (inputManager.isKeyHeld(GLFW_KEY_W)) moveDir = moveDir + forward;
            if (inputManager.isKeyHeld(GLFW_KEY_S)) moveDir = moveDir - forward;
            if (inputManager.isKeyHeld(GLFW_KEY_D)) moveDir = moveDir + right;
            if (inputManager.isKeyHeld(GLFW_KEY_A)) moveDir = moveDir - right;
            
            // Vertical movement (Z-up)
            if (inputManager.isKeyHeld(GLFW_KEY_SPACE))      moveDir.z += 1.0f;
            if (inputManager.isKeyHeld(GLFW_KEY_LEFT_SHIFT)) moveDir.z -= 1.0f;

            if (inputManager.isKeyHeld(GLFW_KEY_LEFT_CONTROL))
            {
                currentSpeed *= 3.0f; // Sprint multiplier
            }

            // 3. Apply movement
            if (Vector3::dot(moveDir, moveDir) > 0.0f) // Only move if a key is pressed
            {
                moveDir.normalize();
                Vector3 displacement = moveDir * currentSpeed;
                
                camera.position = camera.position + displacement;
                camera.target = camera.target + displacement; // Keep target relative to position

                listener.position = camera.position;
                float audioDt = std::min(dt, 0.1f);
                listener.velocity = displacement * (1.0f / audioDt);
            }
        }

        if (inputManager.isKeyPressed(GLFW_KEY_F1)) renderer.setDebugMode(0);
        if (inputManager.isKeyPressed(GLFW_KEY_F2)) renderer.setDebugMode(1);
        if (inputManager.isKeyPressed(GLFW_KEY_F3)) renderer.setDebugMode(2);
        if (inputManager.isKeyPressed(GLFW_KEY_F4)) renderer.setDebugMode(3);
        if (inputManager.isKeyPressed(GLFW_KEY_F5)) renderer.setDebugMode(4);
        if (inputManager.isKeyPressed(GLFW_KEY_F6)) renderer.setDebugMode(5);
        if (inputManager.isKeyPressed(GLFW_KEY_F7)) renderer.setDebugMode(6);
        if (inputManager.isKeyPressed(GLFW_KEY_F8)) renderer.setDebugMode(7);
        if (inputManager.isKeyPressed(GLFW_KEY_F9)) renderer.setDebugMode(8);


        //=================================================================================
        audioEngine.setListener(listener);
        audioEngine.update(dt);

        world.set<CameraComponent>( { camera });

        buckets = world.get<RenderBuckets>();


        // std::cout << buckets.staticObjects[0].transform.position.x << std::endl;
        // std::cout << "=================================================" << std::endl;
        // std::cout << "Static Objects:" << buckets.staticObjects.size() << std::endl;
        // for (auto& obj : buckets.staticObjects)
        // {
        //     std::cout << obj.mesh.slot << " | " << obj.transform.position.entries << std::endl;
        // }
        // std::cout << "=================================================" << std::endl;


        renderer.render(lightManager, 
                buckets,
                camera, staticBVH, quasiStaticBVH, dynamicBVH);
        renderer.fps = currentFps;
        renderer.dt = dt;

        dynamicBVH.update(buckets.dynamicSlowObjects);
        UIrenderer.render(canvas);

#ifdef ENGINE_DEBUG_UI
        debugPanel.beginFrame();
        debugPanel.render(camera, 
                lightManager, 
                audioEngine, 
                renderer, 
                staticBVH, quasiStaticBVH, dynamicBVH);

        debugPanel.endFrame();
#endif

        glfwSwapBuffers(window);
    }

    UIrenderer.shutdown();
    renderer.shutdown();
    
#ifdef ENGINE_DEBUG_UI
    debugPanel.shutdown();
#endif

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

static GLFWwindow* createWindow(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW.\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create window.\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    std::cout << "OpenGL " << glGetString(GL_VERSION)
        << " | " << glGetString(GL_RENDERER) << "\n";

    return window;
}
