#pragma once

#include <GLAD/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <engine/renderer/Texture.h>

namespace Amba {

	struct Rotation {
		float angle;
		glm::vec3 vec3;
	};

	struct TSR {
		glm::vec3 translation;
		glm::vec3 scaling;
		Rotation rotation;
	};

	struct Vertex {
		glm::vec3 v_Position;
		glm::vec3 v_Normals;
		glm::vec2 v_TexCoords;
	};

	class Mesh
	{
	public:

		Mesh();
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, TSR tsr);
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, TSR tsr);
		~Mesh();
	
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<Texture> m_Textures;
		
		TSR m_TSR;
	
		void Cleanup();
		inline bool ContainsTextures() const { return m_NoTextures; }

	private:
		bool m_NoTextures;

	};

}