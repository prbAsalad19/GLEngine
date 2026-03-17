#include "resourceManager.h"

const MeshHandle ResourceManager::loadMesh(const std::string& path)
{
	CPUMesh cpu;
	if (!MeshLoader::loadOBJ(path, cpu)) return MeshHandle{};

	std::vector<unsigned int> indices;
	auto mesh = std::make_unique<OpenGLMesh>(MeshLoader::toVertexArray(cpu, indices), cpu.getFaces());

	return meshPool.insert(path, std::move(mesh));
}

OpenGLMesh* ResourceManager::getMesh(MeshHandle handle)
{
	return meshPool.get(handle);
}

void ResourceManager::deleteMesh(MeshHandle handle)
{
	meshPool.remove(handle);
}

const TextureHandle ResourceManager::loadTexture(const std::string& path)
{
	auto texture = std::make_unique<OpenGLTexture>(path.c_str());
	return texturePool.insert(path, std::move(texture));
}

OpenGLTexture* ResourceManager::getTexture(TextureHandle handle)
{
	return texturePool.get(handle);
}

void ResourceManager::deleteTexture(TextureHandle handle)
{
	texturePool.remove(handle);
}

const MaterialHandle ResourceManager::loadMaterial(TextureHandle albedo)
{
    // Risolvi texture mancante prima di costruire la chiave
    if (albedo.isNull())
        albedo = loadTexture("img/defaultTexture.png");

    // Chiave unica basata sulle texture che compongono il materiale
    std::string key = "__mat_albedo_" + std::to_string(albedo.slot)
                    + "_" + std::to_string(albedo.generation);

    Material mat;
    mat.textures["albedo"] = albedo;

    auto material = std::make_unique<Material>(mat);
    return materialPool.insert(key, std::move(material));
}

Material* ResourceManager::getMaterial(MaterialHandle handle)
{
	return materialPool.get(handle);
}

void ResourceManager::deleteMaterial(MaterialHandle handle)
{
	materialPool.remove(handle);
}
