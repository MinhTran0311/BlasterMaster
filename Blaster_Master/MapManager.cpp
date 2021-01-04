#include "MapManager.h"
MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}
MapManager* MapManager::intance = NULL;
MapManager* MapManager::GetIntance()
{
	if (intance == NULL)
		intance = new MapManager();
	return intance;
}

Map* MapManager::Get(int id)
{
	Map* map = maps[id];
	return map;
}

void MapManager::Add(int id, Map* map)
{
	maps[id] = map;
}

int MapManager::GetMapWidth(int idstage)
{
	return maps[idstage]->GetMapWidth();
}

int MapManager::GetMapHeight(int idstage)
{
	return maps[idstage]->GetMapHeight();
}

int MapManager::GetRightAlign(int idstage)
{
	return maps[idstage]->GetRightAlign();
}

int MapManager::GetLeftAlign(int idstage)
{
	return maps[idstage]->GetLeftAlign();
}

LPDIRECT3DTEXTURE9 MapManager::GetMapBackGround(int idstage)
{
	return maps[idstage]->GetMapBackGround();
}

LPCWSTR MapManager::GetFilePath(int idstage)
{
	return maps[idstage]->GetFilePath();
}
