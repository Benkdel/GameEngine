#pragma once

#include <engine/renderer/Mesh.h>

namespace Amba {

	class Model {
	public:
		Model();
		~Model();

		void LoadModel(const std::string& path);

		void Cleanup();

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_TexturesLoaded;

		std::string m_Directory;
		void processNode(ABImp::Node* node, ABImp::Importer* data);
		Mesh processMesh(ABImp::Mesh* mesh, ABImp::Node* node);

		std::vector<Texture> LoadTextures(std::string uri, std::string texName);


		friend class Renderer;
	};

}