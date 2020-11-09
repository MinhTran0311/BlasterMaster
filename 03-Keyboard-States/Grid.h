#pragma once
#include "global.h"
#include "Entity.h"
#include "Camera.h"
using namespace std;

#define GRID_CELL_HEIGHT SCREEN_HEIGHT/2;
#define GRID_CELL_WIDTH SCREEN_WIDTH/2;

#define GRID_CELL_MAX_ROW 20 // số dòng tối đa;
#define GRID_CELL_MAX_COLUMN 20 // số cột tối đa

class Grid
{

	vector<Entity*> cells[GRID_CELL_MAX_ROW][GRID_CELL_MAX_COLUMN];
	bool isActive[GRID_CELL_MAX_ROW][GRID_CELL_MAX_COLUMN];
public:
	Grid();
	~Grid();

	void Insert(Entity object);
	void Update(DWORD dt);
	void Render();
	void getListObjectInCamZone(Camera* cam);
};