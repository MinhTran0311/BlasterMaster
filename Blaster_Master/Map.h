#pragma once
#include "Sprites.h"
#include "Textures.h"
class Map
{
public:
	Map(int, LPCWSTR, int, int, bool, bool);
	~Map();
	int GetMapWidth();
	int GetMapHeight();
	int GetRightAlign();
	int GetLeftAlign();
	LPCWSTR GetFilePath() { return sceneFilePath; };
	LPDIRECT3DTEXTURE9 GetMapBackGround();
private:
	int id;
	//LPDIRECT3DTEXTURE9 backgroud;
	int mapWidth, mapHeight;
	bool hasRightEdge, hasLeftEdge;
	LPCWSTR sceneFilePath;
};

