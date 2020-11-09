/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 03 - KEYBOARD AND OBJECT STATE

	This sample illustrates how to:

		1/ Process keyboard input
		2/ Control object state with keyboard events
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Enemy.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"TUAN - 1"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 90

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_BLASTER_MASTER_ENEMY 30

CGame* game;
CMario* mario;
CEnemy2* EnemyJump = new CEnemy2(40.0f, 30.0f);
CEnemy1* EnemyVehicle = new CEnemy1;


class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//switch (KeyCode)
	//{
	//case DIK_SPACE:
	//	mario->SetState(MARIO_STATE_JUMP);
	//	break;
	//case DIK_W:
	//	mario->SetState(MARIO_STATE_JUMP_YELLOW);
	//	break;
	//}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	//if (game->IsKeyDown(DIK_RIGHT))
	//	mario->SetState(MARIO_STATE_WALKING_RIGHT);
	//else if (game->IsKeyDown(DIK_LEFT))
	//	mario->SetState(MARIO_STATE_WALKING_LEFT);
	//else if (game->IsKeyDown(DIK_A))
	//	mario->SetState(MARIO_STATE_WALKING_LEFT_YELLOW);
	//else if (game->IsKeyDown(DIK_D))
	//	mario->SetState(MARIO_STATE_WALKING_RIGHT_YELLOW);
	//else mario->SetState(MARIO_STATE_IDLE);
	if (game->IsKeyDown(DIK_A))
		EnemyJump->SetState(ENEMY_STATE_JUMPING_LEFT);
	else if (game->IsKeyDown(DIK_D))
		EnemyJump->SetState(ENEMY_STATE_JUMPING_RIGHT);
	else EnemyJump->SetState(ENEMY_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemy.png", D3DCOLOR_XRGB(176, 224, 248));


	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);
	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);


	//sprites->Add(10001, 246, 154, 260, 181, texMario);

	//sprites->Add(10002, 275, 154, 290, 181, texMario);
	//sprites->Add(10003, 304, 154, 321, 181, texMario);

	//sprites->Add(10011, 186, 154, 200, 181, texMario);

	//sprites->Add(10012, 155, 154, 170, 181, texMario);
	//sprites->Add(10013, 125, 154, 140, 181, texMario);

	////new one
	//sprites->Add(20001, 246, 393, 260, 422, texMario);

	//sprites->Add(20002, 275, 393, 290, 422, texMario);
	//sprites->Add(20003, 304, 393, 321, 422, texMario);

	//sprites->Add(20011, 186, 393, 200, 422, texMario);

	//sprites->Add(20012, 155, 393, 170, 422, texMario);
	//sprites->Add(20013, 125, 393, 140, 422, texMario);

	//LPANIMATION ani;

	//ani = new CAnimation(100);	
	//ani->Add(10001);
	//animations->Add(400, ani);

	//ani = new CAnimation(100);
	//ani->Add(10011);
	//animations->Add(401, ani);


	//ani = new CAnimation(100);
	//ani->Add(10001);
	//ani->Add(10002);
	//ani->Add(10003);
	//animations->Add(500, ani);

	//ani = new CAnimation(100);
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//animations->Add(501, ani);

	////new
	//ani = new CAnimation(100);
	//ani->Add(20001);
	//animations->Add(600, ani);

	//ani = new CAnimation(100);
	//ani->Add(20011);
	//animations->Add(601, ani);


	//ani = new CAnimation(100);
	//ani->Add(20001);
	//ani->Add(20002);
	//ani->Add(20003);
	//animations->Add(700, ani);

	//ani = new CAnimation(100);
	//ani->Add(20011);
	//ani->Add(20012);
	//ani->Add(20013);
	//animations->Add(701, ani);

	//enemy vehicle
	//running left
	sprites->Add(90001, 96, 672, 122, 690, texEnemy);
	sprites->Add(90002, 68, 672, 94, 690, texEnemy);
	sprites->Add(90003, 40, 672, 66, 690, texEnemy);
	sprites->Add(90004, 12, 672, 38, 690, texEnemy);
	//running right
	sprites->Add(90010, 132, 672, 158, 690, texEnemy);
	sprites->Add(90011, 160, 672, 186, 690, texEnemy);
	sprites->Add(90012, 188, 672, 214, 690, texEnemy);
	sprites->Add(90013, 216, 672, 242, 690, texEnemy);

	//enemy jump
	//jump left
	sprites->Add(90100, 42, 530, 60, 548, texEnemy);
	sprites->Add(90101, 62, 527, 80, 548, texEnemy);
	//jump right
	sprites->Add(90110, 194, 530, 212, 548, texEnemy);
	sprites->Add(90111, 174, 527, 192, 548, texEnemy);


	//Add animations
	//enemy vehicle
	LPANIMATION ani;
	ani = new CAnimation(100);
	ani->Add(90001);
	ani->Add(90002);
	ani->Add(90003);
	ani->Add(90004);
	animations->Add(900, ani);

	ani = new CAnimation(100);
	ani->Add(90010);
	ani->Add(90011);
	ani->Add(90012);
	ani->Add(90013);
	animations->Add(901, ani);

	//enemy jump
	ani = new CAnimation(100);
	ani->Add(90100);
	ani->Add(90101);
	animations->Add(902, ani);

	ani = new CAnimation(100);
	ani->Add(90110);
	ani->Add(90111);
	animations->Add(903, ani);

	//IDLE	

	ani = new CAnimation(100);
	ani->Add(90100);
	animations->Add(904, ani);

	ani = new CAnimation(100);
	ani->Add(90110);
	animations->Add(905, ani);

	CEnemy1::AddAnimation(900);
	CEnemy1::AddAnimation(901);

	CEnemy2::AddAnimation(902);
	CEnemy2::AddAnimation(903);
	CEnemy2::AddAnimation(904);
	CEnemy2::AddAnimation(905);


	//mario = new CMario();
	//CMario::AddAnimation(400);		// idle right
	//CMario::AddAnimation(401);		// idle left
	//CMario::AddAnimation(500);		// walk right
	//CMario::AddAnimation(501);		// walk left	

	////colored
	//CMario::AddAnimation(600);		// idle right
	//CMario::AddAnimation(601);		// idle left
	//CMario::AddAnimation(700);		// walk right
	//CMario::AddAnimation(701);		// walk left



	//mario->SetPosition(0.0f, 100.0f);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	//mario->Update(dt);
	EnemyJump->Update(dt);
	EnemyVehicle->Update(dt);
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		//mario->Render();
		EnemyJump->Render();
		EnemyVehicle->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	LoadResources();
	Run();

	return 0;
}