#pragma once
#include "Map.h"
#include "global.h"
#include "debug.h"

class MapManager
{
public:
	MapManager();
	~MapManager();
	static MapManager* GetIntance();
	Map* Get(int id);
	void Add(int id, Map* map);

	int GetMapWidth(int idstage);
	int GetMapHeight(int idstage);
	int GetRightAlign(int idstage);
	int GetLeftAlign(int idstage);
	void SetCurrentStage(int id) { currentStage = id; };
	int GetCurrenStage() { return currentStage; };
	LPDIRECT3DTEXTURE9 GetMapBackGround(int idstage);
	LPCWSTR GetFilePath(int idstage);
private:
	unordered_map<int, Map*> maps;
	static MapManager* intance;
	int currentStage;

};

