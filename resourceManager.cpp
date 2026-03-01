#include "config.h"
#include "resourceManager.h"

int ResourceManager::loadMesh(const std::string& filepath, Transform transform)
{
	std::string canonicalPath = std::filesystem::canonical(filepath).string();
	auto it = meshCache.find(canonicalPath);
	if (it != meshCache.end())
	{
		return it->second;
	}

	CPUMesh cpuMesh;
	std::vector<unsigned int> vertices;
	if (!MeshLoader::loadOBJ(canonicalPath, cpuMesh))
	{
		std::cout << "Failed to create mesh resource" << canonicalPath << std::endl;
		return -1;
	}
	Mesh mesh = new Mesh(MeshLoader::toVertexArray(cpuMesh, vertices), cpuMesh.getFaces(), transform);
	meshes.push_back(std::move(mesh));
}