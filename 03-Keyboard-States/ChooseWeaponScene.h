#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
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
#include "Gate.h"
#include "PlayScene.h"
#include <iostream>
#include <fstream>
using namespace std;


class ChooseWeaponScene : public Scene
{
public:

	ChooseWeaponScene();
	ChooseWeaponScene(int idStage);
	~ChooseWeaponScene();
	bool isUnloaded = false;
	void setidStage(int _idStage) { idStage = _idStage; };
	int getidStage() { return idStage; };
	void SetIsFinished(bool finished){ isfished =finished;}
protected:
	bool isfished = false;

	LPGAMEPLAYER player;
	LPGAMEPLAYER backup_player;
	JASON* jason;

	HUD* gameHUD;
	void LoadBaseObjects();
	void LoadBaseTextures();
	int mapWidth, mapHeight;
	int camMaxWidth;

	LPANIMATION_SET chooseWeaponScene_ani_set;
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path) {};
	
	//Item* RandomItem(float x, float y);
	//Item* DropItem(EntityType createrType, float x, float y, int idCreater = 0);

	friend class ChooseWeaponSceneScenceKeyHandler;
protected:
	Camera* gameCamera;
	vector<LPCWSTR> listSceneFilePath;
#pragma region lists
	vector<int> listWidth;
	vector<int> listHeight;
#pragma endregion

	int idStage;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);

	//Get stage objects' link and tilemap resources
	void _ParseSection_SCENEFILEPATH(string line);

};

class ChooseWeaponSceneKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	ChooseWeaponSceneKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};
