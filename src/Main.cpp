#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include "core/scene/Scene.h"
#include "core/assets/Camera.h"
#include "core/resourcemanager/ResourceManager.h"
#include "opengl/OpenGLRenderer.h"
#include "opengl/OpenGLUIRenderer.h"
#include "core/ui/UICanvas.h"
#include "core/bvh/BVHTree.h"
#include "core/InputManager/InputManager.h"
#include "core/scene/RenderContext.h"
#include "core/scene/LightManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

    MeshHandle     meshHandle = resources.loadMesh("assets/teapot_with_uv.obj");
    TextureHandle  texHandle = resources.loadTexture("img/whiteTexture.png");
    MaterialHandle matHandle = resources.loadMaterial(texHandle);

    Scene scene;
    //RenderObject obj;
    //obj.mesh = meshHandle;
    //obj.material = matHandle;
    //obj.transform.position = { -0.5f, 0.0f, -0.5f }; 
    //obj.transform.setEuler({-135.0f, 0.0f, 90.0f});
    //obj.transform.scale = { 0.5f, 0.5f, 0.5f };
    //scene.objects.push_back(obj);
    // 9 teiere in griglia 3x3

    std::vector<RenderObject> staticObjects;
    std::vector<RenderObject> quasiStaticObjects;
    std::vector<RenderObject> dynamicSlowObjects;
    std::vector<RenderObject> dynamicFastObjects;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            RenderObject obj;
            obj.mesh = meshHandle;
            obj.material = matHandle;
            obj.transform.position = { i * 4.0f - 4.0f, j * 4.0f - 4.0f, 0.0f };
            obj.transform.scale = { 0.5f, 0.5f, 0.5f };
			obj.transform.setEuler({ 90.0f, 0.0f, 90.0f });
            scene.objects.push_back(obj);
        }
    }

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
    lightManager.add(lightDesc);

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
                                    "shaders/lightingPass.vert", "shaders/lightingPass.frag",
                            "shaders/clusterCompute.comp", "shaders/lightCulling.comp");
    renderer.init();
    std::cout << "[Main] 3D renderer initialized\n";

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

    // build bvh
    scene.objects[4].tier = ObjectTier::QuasiStatic;
    for (const auto& obj : scene.objects)
    {
        switch (obj.tier)
        {
            case ObjectTier::Static:      staticObjects.push_back(obj);      break;
            case ObjectTier::QuasiStatic: quasiStaticObjects.push_back(obj); break;
            case ObjectTier::DynamicSlow: dynamicSlowObjects.push_back(obj); break;
            case ObjectTier::DynamicFast: dynamicFastObjects.push_back(obj); break;
        }
    }

    BVHTree staticBVH    (resources, BVHType::Static);
    BVHTree quasiStaticBVH (resources, BVHType::QuasiStatic);
    BVHTree dynamicBVH   (resources, BVHType::DynamicSlow);

    staticBVH.build(staticObjects);
    quasiStaticBVH.build(quasiStaticObjects);
    dynamicBVH.build(dynamicSlowObjects);


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
		//scene.objects[0].transform.rotate({ 0.0f, 1.0f, 0.0f }, 20.0f * dt);
        scene.objects[0].transform.lerpSmooth(startPos, endPos, t);
        scene.objects[0].transform.slerpSmooth(startRot, endRot, t);
        scene.objects[4].transform.rotate({ 0.0f, 0.0f, 1.0f }, 90.0f * dt);

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

        if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            bool currentlyLocked = inputManager.isCursorLocked();
            inputManager.setCursorLocked(!currentlyLocked);
        }

        if (inputManager.isCursorLocked())
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
            }
        }
        //=================================================================================

        dynamicBVH.update(dynamicSlowObjects);

        renderer.render(scene, lightManager, 
                staticObjects, 
                quasiStaticObjects, 
                dynamicSlowObjects, 
                camera, staticBVH, quasiStaticBVH, dynamicBVH, dynamicFastObjects);

        UIrenderer.render(canvas);
        glfwSwapBuffers(window);
    }

    renderer.shutdown();
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

    glfwSwapInterval(0);

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
