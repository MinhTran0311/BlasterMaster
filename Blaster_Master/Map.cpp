#include "Map.h"
Map::Map(int newid, LPCWSTR newpath, int newwidth, int newheight, bool newleft, bool newright)
{
	id = newid;
	sceneFilePath = newpath;
	mapWidth = newwidth;
	mapHeight = newheight;
	hasRightEdge = newright;
	hasLeftEdge = newleft;
}

Map::~Map()
{
}

int Map::GetMapWidth()
{
	return mapWidth;
}

int Map::GetMapHeight()
{
	return mapHeight;
}

int Map::GetRightAlign()
{
	if (hasRightEdge)
		return 320;
	return 0;
}

int Map::GetLeftAlign()
{
	if (hasLeftEdge)
		return 320;
	return 0;
}

LPDIRECT3DTEXTURE9 Map::GetMapBackGround()
{
	return CTextures::GetInstance()->Get(id);
}
