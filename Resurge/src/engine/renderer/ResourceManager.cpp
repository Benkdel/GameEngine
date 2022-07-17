#include "ResourceManager.h"


// initialice static variables
std::unordered_map<std::string, VertexArray*>	ResManager::rm_VertexArrays;
std::unordered_map<std::string, VertexBuffer*>	ResManager::rm_VertexBuffers;
std::unordered_map<std::string, IndexBuffer*>	ResManager::rm_IndexBuffers;
std::unordered_map<std::string, Shader*>			ResManager::rm_Shaders;
std::unordered_map<std::string, Texture*>		ResManager::rm_Textures;


void ResManager::GenVA(std::string name)
{
	rm_VertexArrays.insert(std::pair<std::string, VertexArray*>(name, new VertexArray()));
}

void ResManager::GenVBO(const void* data, unsigned int size, std::string name)
{
	rm_VertexBuffers.insert(std::pair<std::string, VertexBuffer*>(name, new VertexBuffer(data, size)));
}

void ResManager::GenIBO(unsigned int* data, unsigned int count, std::string name)
{
	rm_IndexBuffers.insert(std::pair<std::string, IndexBuffer*>(name, new IndexBuffer(data, count)));
}

VertexArray* ResManager::GetVA(std::string name)
{
	return rm_VertexArrays[name];
}

VertexBuffer* ResManager::GetVBO(std::string name)
{
	return rm_VertexBuffers[name];
}

IndexBuffer* ResManager::GetIBO(std::string name)
{
	return rm_IndexBuffers[name];
}


void ResManager::CreateShader(const char* vsPath, const char* fsPath, std::string name)
{

}

void ResManager::LoadTexture(const char* texPath, bool alpha, std::string name)
{

}

void ResManager::clear()
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

	// clear shaders

	// clear textures

}