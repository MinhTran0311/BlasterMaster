#pragma once
#include "Game.h"
#include <unordered_map>

using namespace std;

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(int direction, float x, float y, int alpha = 255);
	void DrawY(int direction, float x, float y, int alpha = 255);
	void DrawTopBottom(int direction, float x, float y, int alpha = 255);
	void OldDraw(float x, float y, int alpha = 255);
	int GetFrameWidth() { return right - left; }
	int GetFrameHeight() { return bottom - top; }
};

typedef CSprite* LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	void CSprites::Clear();
	void CSprites::ClearAt(int i);

	static CSprites* GetInstance();
};
