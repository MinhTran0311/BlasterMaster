#pragma once
#include <Windows.h>
#include "Entity.h"
#include "Camera.h"
#include "Jason.h"
#include "Worms.h"
#include "Brick.h"
#include "global.h"
#include "Enemy.h"
#include "SceneManager.h"
#include "Jason.h"
#define CELL_SIZE D3DXVECTOR2(160,137)

class CGrid
{
	static CGrid* __instance;
	vector<LPGAMEENTITY>** cells;
	int columnGrid, rowGrid;
	float cellW, cellH;
	vector<LPGAMEENTITY> FilterObjectDuplicate(vector<LPGAMEENTITY> obj);
	//string fileobj;
	D3DXVECTOR2 posPlayerDefault;

public:
	static CGrid* GetInstance();
	void InitGrid(int _mapW, int _mapH);
	CGrid();
	void LoadGrid(vector<string> token);
	void InsertGrid(LPGAMEENTITY obj, vector<pair<int, int>> posGrid);
	~CGrid();
	void UpdateGrid(vector<LPGAMEENTITY> objects);
	void RemoveObj(LPGAMEENTITY obj, bool isDeletePointer = 1);
	void UnLoadGrid();
	void InsertGrid(LPGAMEENTITY obj);

	D3DXVECTOR2 GetPosPlayerDefault();
	vector<LPGAMEENTITY> GetListUpdateObj();
	vector<LPGAMEENTITY> GetListRenderObj();
};

