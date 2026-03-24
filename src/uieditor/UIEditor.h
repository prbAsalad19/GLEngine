// src/uieditor/UIEditor.h
#pragma once
#include "core/CoreConfig.h"
#include "core/ui/UICanvas.h"
#include "core/resourcemanager/ResourceManager.h"
#include "opengl/OpenGLUIRenderer.h"
#include <filesystem>

class UIEditor
{
public:
    UIEditor();
    ~UIEditor();

    void init(const std::string& jsonPath);
    void shutdown();

private:
    ResourceManager      m_resources;
    OpenGLUIRenderer* m_renderer = nullptr;
    GLFWwindow* m_window = nullptr;

    UICanvas             m_canvas;
    std::string          m_jsonPath;
    std::filesystem::file_time_type m_lastWriteTime;

    void checkReload();

    static GLFWwindow* createWindow(int w, int h, const char* title);
};