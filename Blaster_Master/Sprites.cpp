#include "Sprites.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

void CSprite::Draw(int direction, float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(direction, x, y, texture, left, top, right, bottom, alpha);
}
void CSprite::IntroDraw(int direction, float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->IntroDraw(direction, x, y, texture, left, top, right, bottom, alpha);
}
CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}


void CSprite::DrawTopBottom(int direction, float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->DrawTopBottom(direction, x, y, texture, left, top, right, bottom, alpha);
}

void CSprite::RawDraw(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->RawDraw(x, y, texture, left, top, right, bottom, alpha);
}


void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}


void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

void CSprites::ClearAt(int i)
{
	LPSPRITE spr = sprites[i];
	delete spr;
	sprites.erase(i);
}