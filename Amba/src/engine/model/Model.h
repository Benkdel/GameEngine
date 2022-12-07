#pragma once

#include <engine/renderer/Mesh.h>
#include <engine/dataTypes.h>


namespace Amba {

	class Entity;
	
	class Model {
	public:
		//Model();
		//~Model();

		static void LoadModel(const std::string& path, Entity* parentEntity);

		// void Cleanup();

	private:
		static void InitUtils();
		static void ResetUtils();

		struct Utils
		{
			std::string m_Directory;
			std::vector<Texture*> m_TexturesLoaded;	
		};

		static Utils* s_Utils;

		static void ProcessNode(ABImp::Node* node, ABImp::Importer* data, Entity* parentEntity);
		static void ProcessMesh(ABImp::Mesh* mesh, ABImp::Node* node, Entity* ent);
		static Texture* LoadTextures(std::string uri, std::string texName);

	};

}