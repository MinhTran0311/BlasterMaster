#include "Game.h"
#include "debug.h"
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Entity.h"
#include "Textures.h"

#include "Cannons.h"
#include "Domes.h"
#include "Eyeballs.h"
#include "Floaters.h"
#include "Insects.h"
#include "Jumpers.h"
#include "Orbs.h"
#include "Skulls.h"
#include "Telepoters.h"
#include "Worms.h"

#include "Enemy.h"
CGame* CGame::__instance = NULL;

CGame* CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}
CGame::CGame()
{
}
void CGame::InitWindow(int nCmdShow)
{
}

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::InitDirectX(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;									

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw 
*/
//void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom)
//{
//	D3DXVECTOR3 p(x, y, 0);
//	RECT r; 
//	r.left = left;
//	r.top = top;
//	r.right = right;
//	r.bottom = bottom;
//	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
//}

//void CGame::Draw(float x, float y, int nx, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom)
//{
//	D3DXVECTOR3 p(x, y, 0);
//	RECT r;
//	r.left = left;
//	r.top = top;
//	r.right = right;
//	r.bottom = bottom;
//
//	//flip sprite using nx parameter
//	D3DXMATRIX oldTransform;
//	D3DXMATRIX newTransform;
//
//	spriteHandler->GetTransform(&oldTransform);
//
//	D3DXVECTOR2 center = D3DXVECTOR2(p.x + (right - left) / 2, p.y + (bottom - top) / 2);
//	D3DXVECTOR2 rotate = D3DXVECTOR2(nx > 0 ? 1 : -1, 1);
//
//	D3DXMatrixTransformation2D(&newTransform, &center, 0.0f, &rotate, NULL, 0.0f, NULL);
//	D3DXMATRIX finalTransform = newTransform * oldTransform;
//	spriteHandler->SetTransform(&finalTransform);
//
//	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
//	spriteHandler->SetTransform(&oldTransform);
//}

void CGame::OldDraw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	D3DXVECTOR3 p(floor(x - Camera::GetInstance()->GetCamx()), floor(y - Camera::GetInstance()->GetCamy()), 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void CGame::DrawY(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	D3DXVECTOR3 p(floor(x - Camera::GetInstance()->GetCamx()), floor(y - Camera::GetInstance()->GetCamy()), 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXMATRIX oldMatrix;
	D3DXMATRIX newMatrix;

	D3DXVECTOR2 scaling;
	if (direction < 0)
		scaling = D3DXVECTOR2(1, 1);
	else
		scaling = D3DXVECTOR2(1, -1);

	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(p.x + (float)(right - left) / 2, p.y + (float)(bottom - top) / 2), 0, &scaling, NULL, 0, NULL);
	spriteHandler->GetTransform(&oldMatrix);
	spriteHandler->SetTransform(&newMatrix);
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	spriteHandler->SetTransform(&oldMatrix);

}

void CGame::DrawTopBottom(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	D3DXVECTOR3 p(floor(x - Camera::GetInstance()->GetCamx()), floor(y - Camera::GetInstance()->GetCamy()), 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXMATRIX oldMatrix;
	D3DXMATRIX newMatrix;

	D3DXVECTOR2 scaling;
	if (direction > 0)
		scaling = D3DXVECTOR2(1, 1);
	else
		scaling = D3DXVECTOR2(-1, -1);

	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(p.x + (float)(right - left) / 2, p.y + (float)(bottom - top) / 2), 0, &scaling, NULL, 0, NULL);
	spriteHandler->GetTransform(&oldMatrix);
	spriteHandler->SetTransform(&newMatrix);
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	spriteHandler->SetTransform(&oldMatrix);
}

//void CGame::MapDraw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
//{
//	D3DXVECTOR3 p(floor(x - Camera::GetInstance()->GetCamx()), floor(y - Camera::GetInstance()->GetCamy()), 0);
//	RECT r;
//	r.left = left;
//	r.top = top;
//	r.right = right;
//	r.bottom = bottom;
//
//	D3DXMATRIX oldMatrix;
//	D3DXMATRIX newMatrix;
//
//	D3DXVECTOR2 scaling;
//	scaling = D3DXVECTOR2(2, 2);
//
//	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(p.x + (float)(right - left) / 2, p.y + (float)(bottom - top) / 2), 0, &scaling, NULL, 0, NULL);
//	spriteHandler->GetTransform(&oldMatrix);
//	spriteHandler->SetTransform(&newMatrix);
//	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
//
//	spriteHandler->SetTransform(&oldMatrix);
//}

int CGame::IsKeyDown(int KeyCode)
{ 
	return (keyStates[KeyCode] & 0x80) > 0;
}

void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK) 
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	//this->keyHandler = handler;

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CGame::Draw(int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	D3DXVECTOR3 p(floor(x - Camera::GetInstance()->GetCamx()), floor(y - Camera::GetInstance()->GetCamy()), 0);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXMATRIX oldMatrix;
	D3DXMATRIX newMatrix;

	D3DXVECTOR2 scaling;
	if (direction > 0)
		scaling = D3DXVECTOR2(1, 1);
	else
		scaling = D3DXVECTOR2(-1, 1);

	D3DXMatrixTransformation2D(&newMatrix, &D3DXVECTOR2(p.x + (float)(right - left) / 2, p.y + (float)(bottom - top) / 2), 0, &scaling, NULL, 0, NULL);
	spriteHandler->GetTransform(&oldMatrix);
	spriteHandler->SetTransform(&newMatrix);
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	spriteHandler->SetTransform(&oldMatrix);
}


void CGame::ProcessKeyboard()
{
	HRESULT hr; 

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h==DI_OK)
			{ 
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE *)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

bool CGame::IsCollidingAABB(float ml, float mt, float mr, float mb, float sl, float st, float sr, float sb)
{
	return !(mr < sl || ml > sr || mb < st || mt > sb);
}

void CGame::SweptAABB(float ml, float mt, float mr, float mb, float dx, float dy, float sl, float st, float sr, float sb, float& t, float& nx, float& ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

//void CGame::LoadResources()
//{
//	CTextures* textures = CTextures::GetInstance();
//
//	textures->Add(ID_TEX_ENEMY, L"textures\\enemy.png", D3DCOLOR_XRGB(176, 224, 248));
//
//
//	CSprites* sprites = CSprites::GetInstance();
//	CAnimations* animations = CAnimations::GetInstance();
//
//	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
//
//
//	//enemy vehicle
//	//running right
//	sprites->Add(90000, 132, 672, 158, 690, texEnemy);
//	sprites->Add(90001, 160, 672, 186, 690, texEnemy);
//	sprites->Add(90002, 188, 672, 214, 690, texEnemy);
//	sprites->Add(90003, 216, 672, 242, 690, texEnemy);
//
//	//enemy move
//	//move right
//	sprites->Add(90100, 194, 530, 212, 548, texEnemy);
//	sprites->Add(90101, 174, 527, 192, 548, texEnemy);
//
//	//Add animations
//	//enemy vehicle
//	LPANIMATION ani;
//
//	ani = new CAnimation(100);
//	ani->Add(90000);
//	ani->Add(90001);
//	ani->Add(90002);
//	ani->Add(90003);
//	animations->Add(900, ani);
//
//	//enemy jump
//	ani = new CAnimation(100);
//	ani->Add(90100);
//	ani->Add(90101);
//	animations->Add(901, ani);
//
//	//IDLE	
//	ani = new CAnimation(100);
//	ani->Add(90100);
//	animations->Add(902, ani);
//
//	Enemy1::AddAnimation(900);
//
//	Enemy2::AddAnimation(901);
//	Enemy2::AddAnimation(902);
//
//	//blaster master enemies
//	//Area2
//	// readline => id, left, top, right 
//
//	//10000	Orbs
//	sprites->Add(10001, 158, 387, 176, 405, texEnemy);
//	sprites->Add(10002, 118, 387, 136, 405, texEnemy);
//	sprites->Add(10003, 98, 387, 113, 405, texEnemy);
//	sprites->Add(10004, 138, 387, 156, 405, texEnemy);
//	
//	ani = new CAnimation(100);	
//	ani->Add(10001);
//	animations->Add(1001, ani);			//Orbs floating
//
//	ani = new CAnimation(100);	
//	ani->Add(10002);
//	ani->Add(10003);
//	ani->Add(10004);
//	animations->Add(1002, ani);			//Orbs turning around: right->left
//	
//	COrbs::AddAnimation(1001);
//	COrbs::AddAnimation(1002);
//
//	//10010	Floaters
//	sprites->Add(10011, 132, 406, 150, 422, texEnemy);
//	sprites->Add(10012, 152, 407, 170, 422, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10011);
//	animations->Add(1011, ani);			//Floaters floating	
//	
//	ani = new CAnimation(100);
//	ani->Add(10012);
//	animations->Add(1012, ani);			//Floaters fire
//
//	CFloaters::AddAnimation(1011);
//	CFloaters::AddAnimation(1012);
//	//10020	Worms
//	sprites->Add(10021, 172, 412, 190, 422, texEnemy);
//	sprites->Add(10022, 191, 412, 208, 422, texEnemy);
//	
//	ani = new CAnimation(100);
//	ani->Add(10021);
//	ani->Add(10022);
//	animations->Add(1021, ani);			//Worm move 
//
//	CWorms::AddAnimation(1021);
//	//10030	Domes thuan chieu kim dong ho
//	//left
//	sprites->Add(10031, 133, 461, 149, 479, texEnemy);
//	sprites->Add(10032, 132, 441, 149, 459, texEnemy);
//	
//	ani = new CAnimation(100);
//	ani->Add(10031);
//	ani->Add(10032);
//	animations->Add(1031, ani);			//Domes West side
//	
//	//top
//	sprites->Add(10033, 149, 425, 167, 441, texEnemy);
//	sprites->Add(10034, 169, 424, 187, 441, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10033);
//	ani->Add(10034);
//	animations->Add(1032, ani);			//Domes North side
//	//right
//	sprites->Add(10035, 187, 441, 203, 459, texEnemy);
//	sprites->Add(10036, 187, 461, 204, 479, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10035);
//	ani->Add(10036);
//	animations->Add(1033, ani);			//Domes East side
//	//bottom
//	sprites->Add(10037, 169, 479, 187, 495, texEnemy);
//	sprites->Add(10038, 149, 479, 167, 496, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10037);
//	ani->Add(10038);
//	animations->Add(1034, ani);			//Domes South side
//	CDomes::AddAnimation(1031);
//	CDomes::AddAnimation(1032);
//	CDomes::AddAnimation(1033);
//	CDomes::AddAnimation(1034);
//
//	//10040	Jumpers
//	sprites->Add(10041, 132, 500, 149, 525, texEnemy);
//	sprites->Add(10042, 152, 499, 167, 525, texEnemy);
//	sprites->Add(10043, 170, 500, 185, 525, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10041);
//	ani->Add(10042);
//	ani->Add(10043);
//	animations->Add(1041, ani);			//Jumpers move
//	CJumpers::AddAnimation(1041);
//	//10050	Insects
//	sprites->Add(10051, 132, 530, 150, 548, texEnemy);
//	sprites->Add(10052, 152, 533, 212, 548, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10051);
//	ani->Add(10052);
//	animations->Add(1051, ani);			//Insects move
//	CInsects::AddAnimation(1051);
//	//10060	Skulls
//	sprites->Add(10061, 174, 527, 192, 548, texEnemy);
//	sprites->Add(10062, 194, 530, 170, 548, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10061);
//	ani->Add(10062);
//	animations->Add(1061, ani);			//Skulls move
//	CSkulls::AddAnimation(1061);
//	//10070	Eyeballs
//	sprites->Add(10071, 102, 550, 118, 566, texEnemy);
//	sprites->Add(10072, 119, 550, 135, 566, texEnemy);
//	sprites->Add(10073, 136, 550, 152, 566, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10073);
//	ani->Add(10071);	
//	ani->Add(10073);
//	ani->Add(10071);	
//	ani->Add(10073);
//	ani->Add(10071);
//	ani->Add(10072);
//	animations->Add(1071, ani);			//Eyeballs
//	CEyeballs::AddAnimation(1071);
//	//10080	Telepoters
//	sprites->Add(10081, 132, 568, 156, 600, texEnemy);
//	sprites->Add(10082, 159, 568, 183, 600, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10081);
//	animations->Add(1081, ani);			//Telepoters coverd	
//	
//	ani = new CAnimation(100);
//	ani->Add(10082);
//	animations->Add(1082, ani);			//Telepoters uncoverd
//	CTelepoters::AddAnimation(1081);
//	CTelepoters::AddAnimation(1082);
//	//10090	Cannons
//	sprites->Add(10091, 132, 603, 158, 629, texEnemy);
//	sprites->Add(10092, 162, 603, 184, 629, texEnemy);
//	sprites->Add(10093, 187, 607, 213, 629, texEnemy);
//
//	ani = new CAnimation(100);
//	ani->Add(10091);
//	ani->Add(10092);
//	ani->Add(10091);
//	ani->Add(10093);
//	animations->Add(1091, ani);			//Cannons fire
//	CCannons::AddAnimation(1091);
//}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}



