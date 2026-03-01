#pragma once
#include "config.h"
#include "objLoader.h"

class ResourceManager
{
	std::unordered_map<std::string, int> meshCache;
	std::vector<Mesh> meshes;

public:
	int loadMesh(const std::string& filepath, Transform transform = Transform::getIdentityTransform());

};