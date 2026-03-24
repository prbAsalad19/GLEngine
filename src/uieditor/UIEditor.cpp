// src/uieditor/UIEditor.cpp
#include "UIEditor.h"
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

UIEditor::UIEditor() = default;
UIEditor::~UIEditor() = default;

GLFWwindow* UIEditor::createWindow(int w, int h, const char* title)
{
    if (!glfwInit()) return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* win = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (!win) { glfwTerminate(); return nullptr; }

    glfwMakeContextCurrent(win);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(win);
        glfwTerminate();
        return nullptr;
    }

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    std::cout << "OpenGL " << glGetString(GL_VERSION)
        << " | " << glGetString(GL_RENDERER) << "\n";
    return win;
}

void UIEditor::init(const std::string& jsonPath)
{
    m_jsonPath = jsonPath;
    m_window = createWindow(1280, 720, ("UI Editor — " + jsonPath).c_str());
    if (!m_window) return;

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    m_renderer = new OpenGLUIRenderer(
        m_resources, w, h,
        "shaders/UIShaderv1.txt",
        "shaders/UIShaderf.txt");
    m_renderer->init();
    m_renderer->loadFont("assets/fonts/arial.ttf", 18.0f);

    // Caricamento iniziale
    if (fs::exists(m_jsonPath))
    {
        m_canvas.loadUI(m_jsonPath);
        m_lastWriteTime = fs::last_write_time(m_jsonPath);
    }
    else
    {
        std::cout << "[UIEditor] File non trovato: " << m_jsonPath << "\n";
    }

    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        // Hot reload
        checkReload();

        int fw, fh;
        glfwGetFramebufferSize(m_window, &fw, &fh);
        m_renderer->onResize(
            static_cast<unsigned int>(fw),
            static_cast<unsigned int>(fh));

        glClear(GL_COLOR_BUFFER_BIT);
        m_renderer->render(m_canvas);
        glfwSwapBuffers(m_window);
    }

    shutdown();
}

void UIEditor::checkReload()
{
    if (!fs::exists(m_jsonPath)) return;

    auto writeTime = fs::last_write_time(m_jsonPath);
    if (writeTime == m_lastWriteTime) return;

    // File cambiato — piccola attesa per assicurarsi che
    // il file sia stato scritto completamente dall'editor di testo
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(50ms);

    std::cout << "[UIEditor] Reload: " << m_jsonPath << "\n";
    m_canvas.loadUI(m_jsonPath);
    m_lastWriteTime = writeTime;
}

void UIEditor::shutdown()
{
    if (m_renderer) { m_renderer->shutdown(); delete m_renderer; m_renderer = nullptr; }
    if (m_window) { glfwDestroyWindow(m_window); m_window = nullptr; }
    glfwTerminate();
}