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
using namespace std;


class ChooseWeaponScene : public Scene
{
public:

	ChooseWeaponScene();
	ChooseWeaponScene(JASON* player);
	~ChooseWeaponScene();
	bool isUnloaded = false;
	void SetWeaponId(int _weaponId) { weaponId = _weaponId; };
	int GetWeaponId() { return weaponId; };
	void SetIsFinished(bool finished){ isfished =finished;}
	void SetSpecialWeapon(int _weaponId);
protected:

	
	void LoadBaseObjects();
	void LoadBaseTextures();

	LPANIMATION_SET chooseWeaponScene_ani_set;
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path) {};
	

	friend class ChooseWeaponSceneScenceKeyHandler;
private:
	bool isfished = false;
	int noHomingMissles, noElectric, noRocket;
	int weaponId;
	JASON* player;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);
};

class ChooseWeaponSceneKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	ChooseWeaponSceneKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};
