#pragma once

#include <vector>
#include <engine/ecs/Entcs.h>
#include <engine/renderer/dataTypes.h>

#define SPATIAL_GRID_SIDELENGTH 600
#define NUMBER_OF_CELLS 40

/*
	To simplify the collision algo, the spatial grid will be 2D
	using only x and z I will take all objects in that cell irrespective of their y position
	and compare collision (first discarting y) then other dimensions. As I dont expect to have so many objects
	in the sky it really does not matter for now
*/

namespace Amba
{

	class Cell
	{
	public:
		Cell() {};

		~Cell() {};

		std::vector<EntityId> entities; // entities in current cell
		std::vector<glm::vec3> vertices; // corners of the cell

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

		Cell GetCell(glm::vec3 position);
		
		
		
		// for drawing
		bool m_RenderGrid;
		std::vector<glm::vec3> m_Vertices;
		std::vector<unsigned int> m_Indices;
		

	private:
		float m_CellSize;

	};

}