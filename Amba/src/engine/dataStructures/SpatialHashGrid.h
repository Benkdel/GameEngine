#pragma once

#include <vector>
#include <engine/dataTypes.h>

#define SPATIAL_GRID_SIDELENGTH 200
#define NUMBER_OF_CELLS 10
#define MAX_HEIGHT 100.0f

/*
	To simplify the collision algo, the spatial grid will be 2D
	using only x and z I will take all objects in that cell irrespective of their y position
	and compare collision (first discarting y) then other dimensions. As I dont expect to have so many objects
	in the sky it really does not matter for now
*/

namespace Amba
{

	struct Boundaries
	{
		glm::vec3 min = glm::vec3(0.0f);
		glm::vec3 max = glm::vec3(0.0f);
	};

	class Spatial2DGrid
	{
	public:

		Spatial2DGrid();

		// create grid so it starts from negative to positive and center of grid is in center of screen. is this a good idea?
		Spatial2DGrid(bool renderGrid);

		~Spatial2DGrid();

		// vector to do main operations
		std::vector<Cell> m_Cells;

		GridCell GetGridCell(glm::vec3 position);

		// for drawing
		bool m_RenderGrid;
		std::vector<glm::vec3> m_Vertices;
		std::vector<unsigned int> m_Indices;
		
		bool IsOutsideBoundaries(glm::vec3 position);
		
		float inline GetCellSize()	{ return m_CellSize; };

	private:
		float m_CellSize;
		glm::vec3 m_InitialCorner = glm::vec3(0.0f);
		Boundaries m_Boundaries;

	};

}