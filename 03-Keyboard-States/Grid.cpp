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

void Grid::Insert(Entity* object)
{
	int height = 0;
	int width = 0;
	int left = (int)(object->Getx() / GRID_CELL_WIDTH);
	int right = (int)((object->Getx() + width) / GRID_CELL_WIDTH);
	int top = (int)(object->Gety() / GRID_CELL_HEIGHT);
	int bottom = (int)((object->Gety()+height) / GRID_CELL_HEIGHT);
	//cần height và width của entity để xác định các cell mà nó thuộc về
	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++)
			cells[i][j].push_back(object);
}



void Grid::Update(DWORD dt)
{
	for (int i = 0; i < GRID_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_CELL_MAX_COLUMN; j++)
		{
			if (isActive[i][j])
			{
				for (UINT k = 0; k < cells[i][j].size(); k++)
				{
					cells[i][j].at(k)->Update(dt);
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
				for (UINT k = 0; k < cells[i][j].size(); k++)
				{
					cells[i][j].at(k)->Render();
				}
			}
		}
}

void Grid::getListObjectInCamZone(vector<Entity*>& ListObj,Camera* cam)
{
	ListObj.clear();

	unordered_map<int, Entity*> mapObject;
	int bottom = (int)(((cam->GetCamy() + SCREEN_HEIGHT) - 1) / GRID_CELL_HEIGHT);
	int top = (int)((cam->GetCamy() + 1) / GRID_CELL_HEIGHT);

	int left = (int)((cam->GetCamx() + 1) / GRID_CELL_WIDTH);
	int right = (int)((cam->GetCamx() + SCREEN_WIDTH - 1) / GRID_CELL_WIDTH);
	
	//for (int i = top; i <= bottom; i++)
	//	for (int j = left; j <= right; j++)
			//isActive[i][j] = true;
			////for (UINT k = 0; k < cells[i][j].size(); k++)
			////{
			////	if (cells[i][j].at(k)->GetHealth() > 0)	//còn sống
			////	{
			////		if ()
			////	}
			////}
}
