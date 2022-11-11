#pragma once


#include <engine/importer/Importer.h>

// typedefs 
typedef unsigned long long EntityId;
typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;

namespace Amba {
	
	// data types

	class Cell
	{
	public:
		Cell() {};

		~Cell() {};

		std::vector<EntityId> entities; // entities in current cell
		
		// corners of cell
		glm::vec3 bottomLeft;
		glm::vec3 topLeft;
		glm::vec3 topRight;
		glm::vec3 bottomRight;
		
		//std::vector<glm::vec3> vertices; // corners of the cell

	};


	struct Rotation {
		float angle;
		glm::vec3 vec3;
	};

	struct Vertex {
		glm::vec3 v_Position;
		glm::vec3 v_Normals;
		glm::vec2 v_TexCoords;
	};
}

