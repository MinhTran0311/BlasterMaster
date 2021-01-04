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
#include "Player.h"
#include "Jason.h"
#include "Small_Sophia.h"
#include "Worms.h"
#include "Floaters.h"
#include "Small_Sophia.h"
#include "Brick.h"
#include "Gate.h"
#include "Sound.h"
#include <iostream>
#include <fstream>
using namespace std;

#define LIFE_DISPLAY			1
#define CHOOSING_WEAPON_DISPLAY	2
#define FREEZE_DURATION			4000
#define COOLDOWN_DURATION		2000

class PlayScene : public Scene
{
private:
	bool select_end = false;
	int mapWidth, mapHeight;
	bool isReset = false;

	int CamMoveDirection = 0;
	float xPosCamGo, xPosCamBack, yPosCamGo, yPosCamBack;
	int textureAlpha = 255;

	int camResetXPos;
	int camResetYPos;
	bool isNeedResetCamera;
	bool isUnloaded = false;
	int inforDisplay = 0;
	LPANIMATION_SET animation_set;
	LPGAMEPLAYER player;
	LPGAMEPLAYER backup_player;


	Timer* cooldownTimer = new Timer(COOLDOWN_DURATION);
	Timer* BossIntroTimer = new Timer(FREEZE_DURATION);
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
