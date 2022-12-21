#pragma once

#include <vector>
#include <engine/dataTypes.h>

class Cell
{
public:
	Cell()
		:m_IsValidCell(true) {};

	Cell(bool valid) { m_IsValidCell = valid; };

	Cell(int idx, bool valid)
		: m_CellIdx(idx), m_IsValidCell(valid) {};

	~Cell() {};

	std::vector<EntityId> entities; // entities in current cell

	// corners of cell
	glm::vec3 bottomLeft;
	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 bottomRight;

	inline int GetCellIdx() { return m_CellIdx; };

	inline bool IsCellValid() { return m_IsValidCell; };

private:
	int m_CellIdx = -1;
	bool m_IsValidCell = true;
};

class GridCell
{
public:

	GridCell(bool valid, Cell& cell)
		: m_IsCellValid(valid), m_Cell(cell)
	{}

	inline bool IsCellValid() { return m_IsCellValid; };
	inline Cell& GetCell() { return m_Cell; };

private:
	bool m_IsCellValid = false;
	Cell& m_Cell;
};


