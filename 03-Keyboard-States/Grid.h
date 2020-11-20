//#pragma once
//#include "Entity.h"
//#include "Camera.h"
//#include "Jason.h"
//#include "Worms.h"
//#include "Brick.h"
//#include "global.h"
//#include "Enemy.h"
//#include "SceneManager.h"
//#include "Jason.h"
//#define CELL_SIZE D3DXVECTOR2(160,137)
//
//class CGrid
//{
//	vector<Entity*>** cells;
//	int columnGrid, rowGrid;
//	float cellW, cellH;
//	vector<Entity*> FilterObjectDuplicate(vector<Entity*> obj);
//	string fileobj;
//	D3DXVECTOR2 posPlayerDefault;
//public:
//	void InitGrid(int _mapW, int _mapH);
//	CGrid(int _mapW, int _mapH, string fileobj);
//	void LoadGrid(vector<string> token);
//	void InsertGrid(Entity* obj, pair<int, int> posGrid);
//	~CGrid();
//	void UpdateGrid(vector<Entity*> objects);
//	void RemoveObj(Entity* obj, bool isDeletePointer = 1);
//	void UnLoadGrid();
//	void InsertGrid(Entity* obj);
//
//	D3DXVECTOR2 GetPosPlayerDefault();
//	vector<Entity*> GetListUpdateObj();
//	vector<Entity*> GetListRenderObj();
//};
//
