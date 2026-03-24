#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include "core/scene/Scene.h"
#include "core/scene/Camera.h"
#include "core/resourcemanager/ResourceManager.h"
#include "opengl/OpenGLRenderer.h"
#include "opengl/OpenGLUIRenderer.h"
#include "core/ui/UICanvas.h"
#include "uieditor/UIEditor.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static GLFWwindow* createWindow(int width, int height, const char* title);
struct RenderContext
{
    OpenGLRenderer* renderer;
    OpenGLUIRenderer* ui;
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    auto ctx = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));

    ctx->renderer->onResize(width, height);
    ctx->ui->onResize(width, height);
}

int main()
{
    bool activeEditor = true;

    if (activeEditor)
    {
        UIEditor editor;
        editor.init("assets/exported.json");
        return 0;
    }

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

    MeshHandle     meshHandle = resources.loadMesh("assets/teapot_with_uv.obj");
    TextureHandle  texHandle = resources.loadTexture("C:/Users/simon/vs_projects/GLEngine/img/whiteTexture.png");
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

    Camera camera;
    camera.position = { -5.0f, 0.0f, 3.0f };
    camera.setTarget({ 0.0f, 0.0f, 0.0f });
    camera.fov = 45.0f;
    camera.nearPlane = 0.1f;
    camera.farPlane = 1000.0f;

    OpenGLRenderer renderer(resources, "shaders/vertex.txt", "shaders/fragment.txt");
    renderer.init();
    std::cout << "[Main] 3D renderer initialized\n";

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    OpenGLUIRenderer UIrenderer(resources, w, h, "shaders/UIShaderv1.txt", "shaders/UIShadervf1.txt");
    UIrenderer.init();
    //// fuori dal while, dopo init
    std::cout << "[Main] UI renderer initialized, canvas loaded\n";

    RenderContext ctx{ &renderer, &UIrenderer };

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


    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float  dt = static_cast<float>(now - prevTime);
        prevTime = now;
        static float fpsAccum = 0.0f;
        static float timeAccum = 0.0f;
        static int   fpsCount = 0;

        accumulator += dt;

        // FixedUpdate — timestep fisso, gira N volte se necessario
        while (accumulator >= FIXED_STEP)
        {
            // fisica, collisioni, logica deterministica
            accumulator -= FIXED_STEP;
        }
		//scene.objects[0].transform.rotate({ 0.0f, 1.0f, 0.0f }, 20.0f * dt);
        scene.objects[0].transform.lerpSmooth(startPos, endPos, t);
        scene.objects[0].transform.slerpSmooth(startRot, endRot, t);
        scene.objects[4].transform.rotate({ 0.0f, 0.0f, 1.0f }, 90.0f * dt);

        glfwPollEvents();

        fpsAccum += 1.0f / dt;
        timeAccum += dt;
        fpsCount++;

        if (timeAccum >= 0.25f)  // aggiorna ogni mezzo secondo
        {
            currentFps = static_cast<unsigned int>(fpsAccum / fpsCount);
            fpsAccum = 0.0f;
            timeAccum = 0.0f;
            fpsCount = 0;
        }

        renderer.render(scene, camera);
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
