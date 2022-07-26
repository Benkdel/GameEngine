#include "Mesh.h"

Amba::Mesh::Mesh()
{
}

Amba::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, TSR tsr)
	: m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_NoTextures(false), m_TSR(tsr)
{
}

Amba::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, TSR tsr)
	: m_Vertices(vertices), m_Indices(indices), m_NoTextures(true), m_TSR(tsr)
{
}

Amba::Mesh::~Mesh()
{
}

void Amba::Mesh::Cleanup()
{
	m_Vertices.clear();
	m_Indices.clear();
	m_Textures.clear();
}
