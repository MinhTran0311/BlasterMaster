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
#include "Gate.h"
#include "PlayScene.h"
#include <iostream>
#include <fstream>
#include "Sound.h"
using namespace std;


#define Intro_Scene 700
#define Ending_Scene 701
#define Intro_Animation_Logo 0
#define Intro_Animation_Frog 1
#define Intro_Animation_Car 2
#define Endding_Background1 0

#define Endding_Background2 6
#define Endding_Text1 8
#define Endding_Text2 7

#define Endding_Mountain 1
#define Endding_Frog 2
#define Endding_Hair 3
#define Endding_Cloud 4
#define Endding_Forest 5
#define Endding_Bg2 1

#define Intro_Done 4



class IntroScene : public Scene
{
private:
	float posX, posY;
	int time = 0;
	float moutainY=0;
	float textY = 0;
	int setEndding = 0;
	IntroScene();
	IntroScene(int idStage);
	~IntroScene();
	int setAnimation = 0;
	int soundSension = 0;

protected:
	Camera* gameCamera;
	LPANIMATION_SET intro_ani_set;

	int idStage;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);
	void _ParseSection_SCENEFILEPATH(string line);


public:
	void LoadBaseObjects();
	void LoadBaseTextures();
	int GetAnimation() { return setAnimation; }
	void SetAnimation(int animation) { setAnimation = animation; }
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSceneObjects(LPCWSTR path) {};

	friend class IntroScenceKeyHandler;
};

class IntroScenceKeyHandler : public ScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	IntroScenceKeyHandler(Scene* s) :ScenceKeyHandler(s) {};
};
