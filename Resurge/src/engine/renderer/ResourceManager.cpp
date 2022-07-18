#include "ResourceManager.h"


// initialice static variables
std::unordered_map<std::string, VertexArray*>		ResManager::rm_VertexArrays;
std::unordered_map<std::string, VertexBuffer*>		ResManager::rm_VertexBuffers;
std::unordered_map<std::string, IndexBuffer*>		ResManager::rm_IndexBuffers;
std::unordered_map<std::string, Shader*>			ResManager::rm_Shaders;
std::unordered_map<std::string, Texture*>			ResManager::rm_Textures;


void ResManager::GenVA(const std::string& name)
{
	rm_VertexArrays.insert(std::pair<std::string, VertexArray*>(name, new VertexArray()));
}

void ResManager::GenVBO(const void* data, unsigned int size, const std::string& name)
{
	rm_VertexBuffers.insert(std::pair<std::string, VertexBuffer*>(name, new VertexBuffer(data, size)));
}

void ResManager::GenIBO(unsigned int* data, unsigned int count, const std::string& name)
{
	rm_IndexBuffers.insert(std::pair<std::string, IndexBuffer*>(name, new IndexBuffer(data, count)));
}

void ResManager::CreateShader(const char* vsPath, const char* fsPath, const std::string& name)
{
	rm_Shaders.insert(std::pair<std::string, Shader*>(name, new Shader(vsPath, fsPath)));
}

void ResManager::LoadTexture(const char* texPath, const std::string& name)
{
	rm_Textures.insert(std::pair<std::string, Texture*>(name, new Texture(texPath)));
}

VertexArray* ResManager::GetVA(const std::string& name)
{
	return rm_VertexArrays[name];
}

VertexBuffer* ResManager::GetVBO(const std::string& name)
{
	return rm_VertexBuffers[name];
}

IndexBuffer* ResManager::GetIBO(const std::string& name)
{
	return rm_IndexBuffers[name];
}

Shader* ResManager::GetShader(const std::string& name)
{
	return rm_Shaders[name];
}

Texture* ResManager::GetTexture(const std::string& name)
{
	return rm_Textures[name];
}


void ResManager::cleanup()
{
	for (auto it : rm_VertexArrays)
	{
		if (it.second != NULL)
			it.second = NULL;
	}
	rm_VertexArrays.clear();

	for (auto it : rm_VertexBuffers)
	{
		if (it.second != NULL)
			it.second = NULL;
	}
	rm_VertexBuffers.clear();

	for (auto it : rm_IndexBuffers)
	{
		if (it.second != NULL)
			it.second = NULL;
	}
	rm_IndexBuffers.clear();

	for (auto it : rm_Shaders)
	{
		if (it.second != NULL)
			it.second = NULL;
	}
	rm_Shaders.clear();

	// clear textures

}