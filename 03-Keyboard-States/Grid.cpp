#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{
	for (int i = 0; i < GRID_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_CELL_MAX_COLUMN; j++)
		{
			cells[i][j].clear();
		}
}

void Grid::Insert(Entity object)
{
	int cell_x = object.Getx() / GRID_CELL_WIDTH;
	int cell_y = object.Gety() / GRID_CELL_HEIGHT;
	
}

void Grid::Update(DWORD dt)
{
	for (int i = 0; i < GRID_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_CELL_MAX_COLUMN; j++)
		{
			if (isActive[i][j])
			{
				for (int k = 0; k < cells[i][j].size(); k++)
				{
					cells[i][j][k]->Update(dt);
				}
			}
		}
}

void Grid::Render()
{
	for (int i = 0; i < GRID_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_CELL_MAX_COLUMN; j++)
		{
			if (isActive[i][j])
			{
				for (int k = 0; k < cells[i][j].size(); k++)
				{
					cells[i][j][k]->Render();
				}
			}
		}
}
