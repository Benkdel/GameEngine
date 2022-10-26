#pragma once


#include <engine/importer/Importer.h>


namespace Amba {
	
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

