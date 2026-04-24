#include "InputManager.h"
#include "core/scene/RenderContext.h"
#include <cstring>

void InputManager::init(GLFWwindow* window)
{
    m_window = window;
    //glfwSetWindowUserPointer(window, this);
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    m_mousePos = { (float)x, (float)y };
    glfwSetScrollCallback(window, scroll_callback);
    m_lastMousePos = m_mousePos;
    m_scrollDelta = 0.0f;
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* ctx = static_cast<RenderContext*>(glfwGetWindowUserPointer(window));

    if (ctx && ctx->input)
    {
        ctx->input->m_scrollDelta = static_cast<float>(yoffset);
    }
}

void InputManager::update()
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    Vector2 newPos = { (float)x, (float)y };
    m_mouseDelta = newPos - m_lastMousePos;
    m_lastMousePos = newPos;
    m_mousePos = newPos;   
    m_scrollDelta = 0.0f;
    memcpy(m_previousKeys, m_currentKeys, sizeof(m_currentKeys));
    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        m_currentKeys[i] = glfwGetKey(m_window, i) == GLFW_PRESS;
    }
}

bool InputManager::isKeyHeld(int key) const
{
    return m_currentKeys[key];
}

bool InputManager::isKeyPressed(int key) const
{
    return m_currentKeys[key] && !m_previousKeys[key];
}

Vector2 InputManager::getMouseDelta() const
{
    return m_mouseDelta;
}

float InputManager::getScrollDelta() const
{
    return m_scrollDelta;
}

void InputManager::setCursorLocked(bool locked)
{
    if (locked)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

bool InputManager::isCursorLocked() const
{
    return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}
