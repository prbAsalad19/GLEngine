#pragma once
#include "opengl/OpenGLRenderer.h"
#include "opengl/OpenGLUIRenderer.h"
#include "core/InputManager/InputManager.h"

struct RenderContext
{
    OpenGLRenderer*   renderer;
    OpenGLUIRenderer* ui;
    InputManager*     input;
};