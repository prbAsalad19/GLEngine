#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include "core/scene/Scene.h"
#include "core/scene/Camera.h"
#include "core/resourcemanager/ResourceManager.h"
#include "opengl/OpenGLRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static GLFWwindow* createWindow(int width, int height, const char* title);

int main()
{
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
    RenderObject obj;
    obj.mesh = meshHandle;
    obj.material = matHandle;
    obj.transform.position = { 0.0f, 0.0f, 0.0f }; 
    obj.transform.setEuler({-135.0f, 0.0f, 90.0f});
    obj.transform.scale = { 0.5f, 0.5f, 0.5f };
    scene.objects.push_back(obj);

    Camera camera;
    camera.position = { -5.0f, 0.0f, 3.0f };
    camera.setTarget({ 0.0f, 0.0f, 0.0f });
    camera.fov = 45.0f;
    camera.nearPlane = 0.1f;
    camera.farPlane = 1000.0f;

    OpenGLRenderer renderer(resources, "shaders/vertex.txt", "shaders/fragment.txt");
    renderer.init();

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
        glfwPollEvents();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        renderer.onResize(static_cast<unsigned int>(w),
            static_cast<unsigned int>(h));

        renderer.render(scene, camera);
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
