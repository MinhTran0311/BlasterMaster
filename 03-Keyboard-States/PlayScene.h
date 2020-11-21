#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "Bullet.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"
#include "Camera.h"
#include "HUD.h"
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
	float oldPosX;
	float oldPosY;
	int directMoveCam = -1;
	float posX, posY;

	float nCamXGo;
	float nCamXBack;
	float nCamYGo;
	float nCamYBack;
	float camMap1X;
	float camMap1Y;
	bool tempNeed;
	DWORD timeResetCam;
	PlayScene (int idStage);
	~PlayScene();
protected:
	JASON* jason;
	HUD* gameHUD;
	void LoadBaseObjects();
	void LoadBaseTextures();
	int mapWidth, mapHeight;
	int camMaxWidth;

	void CheckPlayerReachGate();

	void ChooseMap(int whatStage);
	//bool PlayerPassingStage(float DistanceXWant, int directionGo);
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
	vector<LPGAMEENTITY> listGates;
	vector<LPGAMEENTITY> listEnemies;
	vector<LPBULLET> listBullets;
	vector<LPGAMEENTITY> listObjects;
	vector<int> listWidth;
	vector<int> listHeight;
#pragma endregion
	void EraseInactiveObject();
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


