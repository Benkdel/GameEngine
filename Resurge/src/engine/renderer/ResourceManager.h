#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <GLAD/glad.h>

#include "Buffers.h"
#include "Shader.h"
#include "Texture.h"

// Static singleton to keep all resources needed by meshes
// or direct draw calls

class ResManager
{
public:

	static std::unordered_map<std::string, VertexBuffer*>		rm_VertexBuffers;
	static std::unordered_map<std::string, IndexBuffer*>		rm_IndexBuffers;
	static std::unordered_map<std::string, VertexArray*>		rm_VertexArrays;
	static std::unordered_map<std::string, Shader*>				rm_Shaders;
	static std::unordered_map<std::string, Texture*>			rm_Textures;

	static void GenVA(std::string name);
	static void GenVBO(const void* data, unsigned int size, std::string name);
	static void GenIBO(unsigned int* data, unsigned int count, std::string name);
	static void CreateShader(const char* vsPath, const char* fsPath, std::string name);
	static void LoadTexture(const char* texPath, bool alpha, std::string name);

	static VertexArray* GetVA(std::string name);
	static VertexBuffer* GetVBO(std::string name);
	static IndexBuffer* GetIBO(std::string name);
	static Shader* GetShader(std::string name);
	static Texture* GetTexture(std::string name);

	static void cleanup();

private:

};