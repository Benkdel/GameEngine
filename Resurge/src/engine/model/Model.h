#pragma once

#include <glad/glad.h>

#include <vector>
#include <engine/renderer/Mesh.h>
#include <engine/importer/Importer.h>

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
		void processMesh(ABImp::Importer* data);

		friend class Renderer;
	};

}