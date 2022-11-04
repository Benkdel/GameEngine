#include "ResourceManager.h"

using namespace Amba;

// initialice static variables
std::unordered_map<std::string, Scene*>				ResManager::rm_Scenes;
std::unordered_map<std::string, Shader*>			ResManager::rm_Shaders;
std::unordered_map<std::string, Material*>			ResManager::rm_Materials;
std::unordered_map<std::string, Texture*>			ResManager::rm_Textures;
std::unordered_map<std::string, Model*>				ResManager::rm_Models;

void ResManager::CreateScene(const std::string& name)
{
	rm_Scenes.insert(std::pair<std::string, Scene*>(name, new Scene()));
}

void ResManager::CreateShader(const char* vsPath, const char* fsPath, const std::string& name)
{
	rm_Shaders.insert(std::pair<std::string, Shader*>(name, new Shader(vsPath, fsPath)));
}

void ResManager::GenTexture(const std::string& filePath, const std::string& name)
{
	rm_Textures.insert(std::pair<std::string, Texture*>(name, new Texture(filePath)));
}

void ResManager::CreateModel(const std::string& name)
{
	rm_Models.insert(std::pair<std::string, Model*>(name, new Model()));
}

void ResManager::CreateMaterial(const std::string& name)
{
	rm_Materials.insert(std::pair<std::string, Material*>(name, new Material()));
}

Scene* ResManager::GetScene(const std::string& name)
{
	return rm_Scenes[name];
}

Shader* ResManager::GetShader(const std::string& name)
{
	return rm_Shaders[name];
}

Texture* ResManager::GetTexture(const std::string& name)
{
	return rm_Textures[name];
}

Amba::Model* ResManager::GetModel(const std::string& name)
{
	return rm_Models[name];
}

Amba::Material* ResManager::GetMaterial(const std::string& name)
{
	return rm_Materials[name];
}


void ResManager::cleanup()
{
	for (auto it : rm_Shaders)
	{
		if (it.second != NULL)
		{
			it.second->Cleanup();
			it.second = NULL;
		}
	}
	rm_Shaders.clear();

	for (auto it : rm_Textures)
	{
		if (it.second != NULL)
		{
			it.second->Cleanup();
			it.second = NULL;
		}
	}
	rm_Textures.clear();

	for (auto it : rm_Models)
	{
		if (it.second != NULL)
		{
			it.second->Cleanup();
			it.second = NULL;
		}
	}
	rm_Models.clear();

	for (auto& it : rm_Materials)
	{
		if (it.second != NULL)
		{
			it.second = NULL;
		}
	}
	rm_Materials.clear();
}
