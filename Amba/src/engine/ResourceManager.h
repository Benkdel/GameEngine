#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <GLAD/glad.h>

#include <engine/scene/Scene.h>
#include <engine/scene/Entity.h>
#include <engine/renderer/Buffers.h>
#include <engine/material/Material.h>
#include <engine/renderer/Texture.h>

// Static singleton to keep all resources needed by meshes
// or direct draw calls

class ResManager
{
public:

	// simple storage of resources
	// systems that work with entities don't use resource manager for optimization purposes
	// instead, they take a scene that has all its entities (potential to pre sort by shader and other stuff)

	static std::unordered_map<std::string, Amba::Scene*>				rm_Scenes;
	static std::unordered_map<std::string, Amba::Entity*>				rm_Entities;
	static std::unordered_map<std::string, Amba::Shader*>				rm_Shaders;
	static std::unordered_map<std::string, Amba::Material*>				rm_Materials;
	static std::unordered_map<std::string, Amba::Texture*>				rm_Textures;
	
	static Amba::Scene*		CreateScene(const std::string& name, bool active);
	static Amba::Entity*	CreateEntity(const std::string& name, Amba::Scene* scene = nullptr);
	static Amba::Shader*	CreateShader(const char* vsPath, const char* fsPath, const std::string& name);
	static Amba::Material*	CreateMaterial(const std::string& name);
	static Amba::Texture*	GenTexture(const std::string& filePat, const std::string& name);
	
	static Amba::Scene*		GetScene(const std::string& name);
	static Amba::Entity*	GetEntity(const std::string& name);
	static Amba::Shader*	GetShader(const std::string& name);
	static Amba::Texture*	GetTexture(const std::string& name);
	static Amba::Material*	GetMaterial(const std::string& name);

	static void BindScene(Amba::Scene* scene);

	static void cleanup();

private:
	struct SceneData
	{
		Amba::Scene* p_ActiveScene;
	};

	static SceneData* p_SceneData;
};
