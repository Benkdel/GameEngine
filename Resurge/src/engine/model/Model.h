#pragma once

#include <glad/glad.h>

#include <vector>
#include <engine/renderer/Mesh.h>
#include <ASSIMP/scene.h>

#include <engine/importer/Importer.h>

namespace Amba {

	class Model {
	public:
		Model();
		~Model();

		void LoadModel(const std::string& path);
		void LoadModelImporter(const std::string& path);

		void Cleanup();

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_TexturesLoaded;

		std::string m_Directory;

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);

		//Mesh processMeshImporter(ABImp::Importer* data);

		friend class Renderer;
	};

}