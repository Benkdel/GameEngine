#include "SpatialHashGrid.h"
#include <engine/core.h>
#include <engine/dataStructures/Cells.h>
#include <engine/scene/Scene.h>
#include <engine/ecs/components.h>

namespace Amba
{
	Spatial2DGrid::~Spatial2DGrid()
	{
#ifdef AB_ENABLE_ASSERTS
		AB_WARN("Spatial2DGrid destructor called");
#endif
	}

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

	Spatial2DGrid::Spatial2DGrid()
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

	void Spatial2DGrid::AssignEntity(unsigned long long id, Scene* scene)
	{
		if (!EntityHandler::IsEntityValid(id))
			return;

		if (scene->EntHasComponent<CameraComponent>(id))
			return;

		TransformComponent* tsr = scene->GetComponent<TransformComponent>(id);
		GridCell gridCell = GetGridCell(tsr->GetPosition());

		int oldCellIndex = tsr->m_CurrentCell;
		int oldIndexInCell = tsr->m_IndexInCell;
		int newCellIndex = -1;

		if (gridCell.IsCellValid())
		{
			Cell& cell = gridCell.GetCell();
			newCellIndex = cell.GetCellIdx();
			tsr->m_CurrentCell = newCellIndex;

			// no change == skip
			if (oldCellIndex == newCellIndex)
				return;

			cell.entities.push_back(id);
			tsr->m_IndexInCell = m_Cells[newCellIndex].entities.size() - 1;
		}

		// delete entity from old cell vector (bookkeeping)
		if (oldCellIndex >= 0)
		{
			Cell& oldCell = m_Cells[oldCellIndex];
			oldCell.entities.
				erase(oldCell.entities.begin() + oldIndexInCell);

			// for now, move all entities to the right of ent being assigned one position to the left (vector size is reduced)
			for (size_t i = oldIndexInCell; i < oldCell.entities.size(); i++)
			{
				scene->GetComponent<TransformComponent>(oldCell.entities[i])->m_IndexInCell -= 1;
			}
		}

		// if entity is out of world map boundaries, destroy it.
		if (IsOutsideBoundaries(tsr->GetPosition()))
		{
			scene->DestroyEntity(id);
			return;
		}

		tsr = nullptr;
	}

	std::vector<Cell> Spatial2DGrid::GetNearbyCells(glm::vec3 position)
	{
		GridCell gridCell = GetGridCell(position);
		std::vector<Cell> cellsToCheck;

		if (!gridCell.IsCellValid())
			return cellsToCheck;

		Cell& cell = gridCell.GetCell();

		// check if entity is completly inside the cell
		// WARNING: for now this only works for entities smaller than cells
		cellsToCheck.push_back(cell);

		return cellsToCheck;
	}

}

