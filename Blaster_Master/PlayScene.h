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
#include "Player.h"
#include "Jason.h"
#include "Small_Sophia.h"
#include "Gate.h"
#include "Sound.h"
#include <iostream>
#include <fstream>
#include "Grid.h"
#include "Big_Sophia.h"
#include "IntroScene.h"
#include "ChooseWeaponScene.h"
#include "Random.h"
#include "Boss.h"
#include "Item.h"
#include "PlayerHandler.h"
#include "MapManager.h"
using namespace std;

#define LIFE_DISPLAY			1
#define CHOOSING_WEAPON_DISPLAY	2
#define FREEZE_DURATION			4000

//struct PlayerHealthAndGunInfo
//{
//	int life = 2;
//	int playerDirectionBeforePassGate = 1;
//	int jasonHealth = PLAYER_MAX_HEALTH;
//	int jasonGundam = PLAYER_DEFAULT_GUNDAM;
//	int jasonStage = 1;
//	float jasonXPos, jasonYPos;
//	int sophiaHealth = PLAYER_MAX_HEALTH;
//	int sophiaGundam = PLAYER_DEFAULT_GUNDAM;
//	int sophiaStage = 1;
//	float sophiaXPos, sophiaYPos;
//	int specialWeapon = 1;
//};

class PlayScene : public Scene
{
private:
	bool select_end = false;
	bool death = false;
	int time_drawlife = 0;
	bool isReset = false;
	float oldPosX;
	float oldPosY;
	int CamMoveDirection = 0;
	float posX, posY;
	float xPosCamGo, xPosCamBack, yPosCamGo, yPosCamBack;
	int textureAlpha = 255;
	float nCamXGo;
	float nCamXBack;
	float nCamYGo;
	float nCamYBack;
	int camMap1X;
	int camMap1Y;
	bool isNeedResetCamera;
	bool isUnloaded = false;
	int inforDisplay = 0;
	LPANIMATION_SET animation_set;
	LPGAMEPLAYER player;
	LPGAMEPLAYER backup_player;
	int mapWidth, mapHeight;

	Timer* BossIntroTimer = new Timer(FREEZE_DURATION);
	DWORD timeResetCam;
	Random* random = new Random();

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

	void _ParseSection_SCENEFILEPATH(string line);

public:
	void LoadBaseObjects();
	void LoadBaseTextures();
	void SetInforDisplay(int type) { inforDisplay = type; };
	int GetInforDisplay() { return inforDisplay; };
	void CheckPlayerReachGate();
	void ChooseMap(int whatStage);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path);
	virtual void changePlayer();
	void RandomSpawnItem(LPGAMEENTITY ItemSpawer);
	void CheckEnterBoss();
	void SetUpFightBoss();
	void BossAreaController();
	friend class PlayScenceKeyHandler;
	PlayScene();
	PlayScene(int idStage);
	~PlayScene();
};

class PlayScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlayScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};
