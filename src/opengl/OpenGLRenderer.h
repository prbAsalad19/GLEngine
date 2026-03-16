#pragma once
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/Scene.h"
#include "core/scene/Camera.h"

class OpenGLRenderer
{
public:
	void init();
	void shutdown();
	void onResize(unsigned int width, unsigned int height);
	void render(Scene* scene, Camera* camera);
private:
	ResourceManager& m_resources;
	int m_width = 1280;
	int m_height = 720;
};