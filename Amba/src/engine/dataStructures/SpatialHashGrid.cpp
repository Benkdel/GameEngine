#include "SpatialHashGrid.h"

#include <engine/renderer/Renderer.h>


namespace Amba
{
	static Shader* shader;

	Spatial2DGrid::~Spatial2DGrid()
	{
		shader = nullptr;
	}

	Spatial2DGrid::Spatial2DGrid() {}

	Spatial2DGrid::Spatial2DGrid(bool renderGrid)
		: m_RenderGrid(renderGrid)
	{
		unsigned int cellSideLength = SPATIAL_GRID_SIDELENGTH / NUMBER_OF_CELLS;

		m_CellSize = cellSideLength;
		unsigned int idx = 0;

		for (unsigned int z = 0; z < SPATIAL_GRID_SIDELENGTH - m_CellSize; z += m_CellSize)
		{
			for (unsigned int x = 0; x < SPATIAL_GRID_SIDELENGTH - m_CellSize; x += m_CellSize)
			{
				Cell cell;

				cell.vertices.push_back(glm::vec3(x, 0, z));
				cell.vertices.push_back(glm::vec3(x, 0, z + m_CellSize));
				cell.vertices.push_back(glm::vec3(x + m_CellSize, 0, z + m_CellSize));
				cell.vertices.push_back(glm::vec3(x + m_CellSize, 0, z));

				m_Cells.push_back(cell);

#ifdef AB_ENABLE_ASSERTS
				// store this only if we want to visualized grid
				m_Vertices.insert(m_Vertices.end(), std::begin(cell.vertices), std::end(cell.vertices));

				// indices
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx - 4);
#endif

			}
		}

				Cell cell = GetCell(glm::vec3(75.0f, 10.0f, 15.0f));
				for (int i = 0; i < 4; i++)
					AB_INFO("Point {0} coords: [x, y, z] -- [{1}, {2}, {3}]", i, cell.vertices[i].x,
						cell.vertices[i].y, cell.vertices[i].z);
	}

	Cell Spatial2DGrid::GetCell(glm::vec3 position)
	{

		int cell_x = position.x / m_CellSize;
		int cell_z = position.z / m_CellSize;

		AB_INFO("Cell hit {0}", cell_z * m_CellSize + cell_x);

		return m_Cells[cell_z * m_CellSize + cell_x];
	
	}


}

