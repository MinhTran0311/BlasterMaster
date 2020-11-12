#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"
#include "Camera.h"

#include "global.h"
#include "Textures.h"
#include "Utils.h"
#include "Sprites.h"
#include "Animations.h"

#include "Jason.h"
#include "Worms.h"

#include "Brick.h"
#include "Gate.h"

#include <iostream>
#include <fstream>
using namespace std;

class PlayScene : public Scene
{
public:
	int _SophiaType = -1;
	PlayScene ();
	~PlayScene();
protected:
	JASON* jason;

	void LoadBaseObjects();
	void LoadBaseTextures();
	int mapWidth, mapHeight;
	int camMaxWidth;


	void ChooseMap(int whatStage);
	//bool PlayerPassingStage(float DistanceXWant, int directionGo);
	//void PlayerGotGate();
	//void PlayerGotCar();
	//void PlayerTouchEnemy();
	//void PlayerCollideItem();
	//virtual void LoadSceneObjects();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path);
	//Item* RandomItem(float x, float y);
	//Item* DropItem(EntityType createrType, float x, float y, int idCreater = 0);

	friend class PlayScenceKeyHandler;
protected:
	Camera* gameCamera;
	vector<LPCWSTR> listSceneFilePath;
#pragma region lists
	vector<LPGAMEENTITY> listEnemies;
	vector<LPGAMEENTITY> listObjects;
	vector<int> listWidth;
	vector<int> listHeight;
#pragma endregion

	int idStage;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);

	//Get stage objects' link and tilemap resources
	void _ParseSection_SCENEFILEPATH(string line);

};

class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};


