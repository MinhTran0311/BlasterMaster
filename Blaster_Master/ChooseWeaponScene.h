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
#include "Jason.h"
#include "PlayerHandler.h"
using namespace std;

#define Animation_ChooseWeaponScene		52000
#define Animation_ChooseWeaponBackgroud	0
#define Animation_Item_Left				1
#define Animation_Item_Right			2

class ChooseWeaponScene : public Scene
{
private:
	bool isfished = false;
	int noHomingMissles, noElectric, noRocket;
	int weaponId;
	int Number_1 = 0;
	int Number_2 = 0;
	bool isUnloaded = false;
	LPANIMATION_SET chooseWeaponScene_ani_set;
	//JASON* player;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);

protected:
	void LoadBaseObjects();
	void LoadBaseTextures();
	friend class ChooseWeaponSceneScenceKeyHandler;

public:
	ChooseWeaponScene();
	~ChooseWeaponScene();
	void SetWeaponId(int _weaponId) { weaponId = _weaponId; };
	int GetWeaponId() { return weaponId; };
	void SetIsFinished(bool finished) { isfished = finished; }
	void SetSpecialWeapon(int _weaponId);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path) {};
};

class ChooseWeaponSceneKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	ChooseWeaponSceneKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};
