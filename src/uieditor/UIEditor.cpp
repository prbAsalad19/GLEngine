#include "UIEditor.h"
#include "opengl/OpenGLUIRenderer.h"

UIEditor::UIEditor()
{
}

UIEditor::~UIEditor()
{
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

void UIEditor::init()
{
    GLFWwindow* editorWindow = createWindow(1280, 720, "engine-ui editor");
    int w, h;
    glfwGetFramebufferSize(editorWindow, &w, &h);

    UICanvas canvas;
    canvas.loadUI("assets/editorUI.json");
    glfwSetWindowUserPointer(editorWindow, &canvas);
    glfwSetMouseButtonCallback(editorWindow, OpenGLUIRenderer::mouseButtonCallback);

    OpenGLUIRenderer UIrenderer(resources, w, h, "shaders/UIShaderv1.txt", "shaders/UIShadervf1.txt");
    UIrenderer.init();

    while (!glfwWindowShouldClose(editorWindow))
    {
        glfwPollEvents();

        UIrenderer.render(canvas);
    }

    UIrenderer.shutdown();
    glfwDestroyWindow(editorWindow);
    glfwTerminate();
}
