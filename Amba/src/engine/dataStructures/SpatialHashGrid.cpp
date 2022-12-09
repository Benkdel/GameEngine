#include "SpatialHashGrid.h"

#include <engine/renderer/Renderer.h>

namespace Amba
{
	static Shader* shader;

	Spatial2DGrid::~Spatial2DGrid()
	{
		AB_WARN("Spatial 2D grid destructor called");
		shader = nullptr;
	}

	Spatial2DGrid::Spatial2DGrid() {}

	bool Spatial2DGrid::IsOutsideBoundaries(glm::vec3 position)
	{
		if (position.x < m_Boundaries.min.x || position.x > m_Boundaries.max.x)
			return true;
		if (position.z < m_Boundaries.min.z || position.z > m_Boundaries.max.z)
			return true;
		if (position.y < m_Boundaries.min.y || position.y > m_Boundaries.max.y)
			return true;
		return false;
	}

	Spatial2DGrid::Spatial2DGrid(bool renderGrid)
		: m_RenderGrid(renderGrid)
	{
		unsigned int cellSideLength = SPATIAL_GRID_SIDELENGTH / NUMBER_OF_CELLS;

		m_CellSize = cellSideLength;
		unsigned int idx = 0;

		m_Boundaries.max = m_InitialCorner + glm::vec3(SPATIAL_GRID_SIDELENGTH, MAX_HEIGHT, SPATIAL_GRID_SIDELENGTH);
		m_Boundaries.min = m_InitialCorner - glm::vec3(0.0f, MAX_HEIGHT, 0.0f);

		for (unsigned int z = 0; z < SPATIAL_GRID_SIDELENGTH; z += m_CellSize)
		{
			for (unsigned int x = 0; x < SPATIAL_GRID_SIDELENGTH; x += m_CellSize)
			{
				Cell cell(m_Cells.size(), true);

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

	GridCell Spatial2DGrid::GetGridCell(glm::vec3 position)
	{
		if (IsOutsideBoundaries(position))
			return GridCell(false, Cell(false));

		int cell_x = position.x / m_CellSize;
		int cell_z = position.z / m_CellSize;
		
		return GridCell(true, m_Cells[cell_z * NUMBER_OF_CELLS + cell_x]);
	}


}

