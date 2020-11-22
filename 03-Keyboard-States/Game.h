#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Global.h"
#include "Camera.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "KeyboardHandler.h"
#define KEYBOARD_BUFFER_SIZE 1024
#include <dinput.h>
#include <unordered_map>
#include "Scene.h"
#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_BLASTER_MASTER_ENEMY 30

/*
Abstract class to define keyboard event handlers
*/
using namespace std;

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

public:

	void InitWindow(int nCmdShow);
	void InitDirectX(HWND hWnd);
	void InitKeyboard();
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom);

	void Draw(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);


	void OldDraw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	//void MapDraw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);


	void DrawY(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);
	void DrawTopBottom(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);

	void SwitchScene(int id_scene);
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void LoadResources();

	static bool IsCollidingAABB(
		float ml,			// move left 
		float mt,
		float mr,
		float mb,
		float sl,			// static left
		float st,
		float sr,
		float sb);

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	HWND GetWindowHandle() { return this->hWnd; }
	static CGame* GetInstance();
	CGame();
	~CGame();
};


