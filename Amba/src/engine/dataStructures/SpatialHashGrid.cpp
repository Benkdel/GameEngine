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

				cell.bottomLeft = glm::vec3(x, 0, z);
				cell.topLeft = glm::vec3(x, 0, z + m_CellSize);
				cell.topRight = glm::vec3(x + m_CellSize, 0, z + m_CellSize);
				cell.bottomRight = glm::vec3(x + m_CellSize, 0, z);

				m_Cells.push_back(cell);

#ifdef AB_ENABLE_ASSERTS
				// store this only if we want to visualized grid
				m_Vertices.push_back(cell.bottomLeft);
				m_Vertices.push_back(cell.topLeft);
				m_Vertices.push_back(cell.topRight);
				m_Vertices.push_back(cell.bottomRight);

				// indices
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx++);
				m_Indices.push_back(idx - 4);
#endif

			}
		}
	}

	Cell &Spatial2DGrid::GetCell(glm::vec3 position)
	{
		int cell_x = position.x / m_CellSize;
		int cell_z = position.z / m_CellSize;

		// for now
		if (cell_x < 0)
			cell_x = 0;
		if (cell_z < 0)
			cell_z = 0;
				
		return m_Cells[cell_z * m_CellSize + cell_x];
		
	}


}

