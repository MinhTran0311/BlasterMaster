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

#include "GunUp.h"
#include "PowerUp.h"
#include <iostream>
#include <fstream>
using namespace std;

#define LIFE_DISPLAY			1
#define CHOOSING_WEAPON_DISPLAY	2

struct PlayerHealthAndGunInfo
{

	int life = 2;
	int playerDirectionBeforePassGate = 1;
	int jasonHealth = PLAYER_MAX_HEALTH;
	int jasonGundam = PLAYER_DEFAULT_GUNDAM;
	int jasonStage = 1;
	float jasonXPos, jasonYPos;
	int sophiaHealth = PLAYER_MAX_HEALTH;
	int sophiaGundam = PLAYER_DEFAULT_GUNDAM;
	int sophiaStage = 1;
	float sophiaXPos, sophiaYPos;
	int specialWeapon = 1;
};

class PlayScene : public Scene
{
public:
	bool select_end = false;
	bool death = false;
	int time_drawlife = 0;
	bool isReset = false;
	float oldPosX;
	float oldPosY;
	int CamMoveDirection = -1;
	float posX, posY;
	float xPosCamGo, xPosCamBack, yPosCamGo, yPosCamBack;



	float nCamXGo;
	float nCamXBack;
	float nCamYGo;
	float nCamYBack;
	int camMap1X;
	int camMap1Y;
	bool isNeedResetCamera;
	//Entity* currentPlayer;
	PlayScene();
	DWORD timeResetCam;
	PlayScene (int idStage);
	~PlayScene();
	bool isUnloaded = false;
	
	int inforDisplay = 0;

	/*void SetKeyhandler(Scene* scene) {
		keyHandler = new PlayScenceKeyHandler(scene);
		CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	}*/
protected:

	LPANIMATION_SET animation_set;
	LPGAMEPLAYER player;
	LPGAMEPLAYER backup_player;
	PlayerHealthAndGunInfo playerInfo;
	JASON* jason;
	Small_Sophia* ssophia;
	HUD* gameHUD;
	void LoadBaseObjects();
	void LoadBaseTextures();
	int mapWidth, mapHeight;
	int camMaxWidth;
	void SetInforDisplay(int type) { inforDisplay = type; };
	int GetInforDisplay() { return inforDisplay; };
	void CheckPlayerReachGate();

	void ChooseMap(int whatStage);


	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path);
	virtual void changePlayer();
	//Item* RandomItem(float x, float y);
	//Item* DropItem(EntityType createrType, float x, float y, int idCreater = 0);

	friend class PlayScenceKeyHandler;
protected:
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
