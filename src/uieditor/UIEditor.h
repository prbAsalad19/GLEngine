#pragma once
#include "opengl/OpenGLUIRenderer.h"

class UIEditor
{
public:
    UIEditor();
    ~UIEditor();

    void init();
    void shutDown();

private:
    ResourceManager resources;
};
