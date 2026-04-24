// InputManager.h
#pragma once
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class InputManager
{
public:
    void init(GLFWwindow* window);
    void update();

    bool isKeyHeld(int key) const;
    bool isKeyPressed(int key) const;

    Vector2 getMouseDelta() const;
    Vector2 getMousePos() const;
    bool isMouseButtonHeld(int button) const;
    float getScrollDelta() const;

    void setCursorLocked(bool locked);
    bool isCursorLocked() const;

private:
    GLFWwindow* m_window;
    Vector2 m_mousePos;
    Vector2 m_mouseDelta;
    Vector2 m_lastMousePos;
    float m_scrollDelta;

    bool m_currentKeys[GLFW_KEY_LAST] = {};
    bool m_previousKeys[GLFW_KEY_LAST] = {};

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
