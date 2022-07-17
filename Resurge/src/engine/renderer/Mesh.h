#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// include shader

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 v_Position;
	glm::vec3 v_Normals;
	glm::vec2 v_TexCoords;
};

class Mesh
{
public:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	// textures TODO
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void Draw();


private:


};