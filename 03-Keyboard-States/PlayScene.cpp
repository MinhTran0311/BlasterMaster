#pragma once
#include "Worms.h"
#include "Insects.h"
#include "PlayScene.h"
#include "Skulls.h"
#include "Orbs.h"
#include "Jumpers.h"
#include "JasonRocket.h"
#include "JasonBullet.h"
#include "SmallSophiaBullet.h"
#include "Grid.h"
#define ID_SMALL_SOPHIA	0
#define ID_JASON		1
#define ID_BIG_SOPHIA	2

#define MAP2_SIDE	200

//#define OBJECT_TYPE_BRICK		1
//#define OBJECT_TYPE_GATE		2
//
//#define OBJECT_TYPE_WORM 10
//#define OBJECT_TYPE_DOMES 11
//#define OBJECT_TYPE_FLOATER 12
//
//#define OBJECT_TYPE_INSECT 13
//
//
//#define OBJECT_TYPE_ORBS 14
//#define OBJECT_TYPE_JUMPER 15
//#define OBJECT_TYPE_SKULLS 16

#define HUD_Y (SCREEN_HEIGHT/11)

PlayScene::PlayScene()
{
}

PlayScene::PlayScene(int _idStage) : Scene()
{
	idStage = _idStage;
	keyHandler = new PlayScenceKeyHandler(this);
	//_SophiaType = ID_JASON;
	LoadBaseObjects();
	ChooseMap(idStage);
}
void PlayScene::LoadBaseObjects()
{
	texturesFilePath = ToLPCWSTR("Resource\\SceneAndSpec\\base_playscene.txt");
	LoadBaseTextures();
#pragma region create_base_objects
	if (player == NULL)
	{
		player = new JASON(55, 100, PLAYER_MAX_HEALTH, PLAYER_DEFAULT_GUNDAM);
		DebugOut(L"[INFO] JASON CREATED!!! \n");
	}
	//if (ssophia == NULL)
	//{
	//	ssophia = new Small_Sophia(55, 100);
	//	DebugOut(L"[INFO] SMALL SOPHIA CREATED!!! \n");
	//}
	if (gameHUD == NULL)
	{
		gameHUD = new HUD(player->GetHealth(), player->GetgunDam());
		DebugOut(L"[INFO] HUD CREATED! %d \n", player->GetHealth());
	}
#pragma endregion
	gameCamera = Camera::GetInstance();
	gameCamera->SetCamPos(0.0f, 0.0f);	//initial camera
}
void PlayScene::LoadBaseTextures()
{
	DebugOut(L"[INFO] Start loading TEXTURES resources from : %s \n", texturesFilePath);

	ifstream f;
	f.open(texturesFilePath);

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;				//skip commnet line

		if (line == "[TEXTURES]")								//texture section
		{
			section = SCENE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]")								//sprite section
		{
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]")								//animation section
		{
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]")							//animationset section
		{
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[SCENE]")									//secne section
		{
			section = SCENE_SECTION_SCENEFILEPATH; continue;
		}
		if (line == "[MAPTEXTURES]")							//maptexture sectoin
		{
			section = SCENE_SECTION_MAPTEXTURES; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_SCENEFILEPATH: _ParseSection_SCENEFILEPATH(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading TEXTURES resources %s\n", texturesFilePath);
}
void PlayScene::ChooseMap(int Stage)
{
	idStage = Stage;
	CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	sceneFilePath = listSceneFilePath[(Stage % 10) - 1];			
	DebugOut(L"Init");
	CGrid::GetInstance()->InitGrid(listWidth[(idStage % 10) - 1], listHeight[(idStage % 10) - 1]);
	LoadSceneObjects(sceneFilePath);
}

void PlayScene::LoadSceneObjects(LPCWSTR path)
{
	DebugOut(L"[INFO] Start loading scene111 resources from : %s \n", path);

	ifstream f;
	f.open(path);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines

		if (line == "[CLEARTEXTURES]") {
			section = SCENE_SECTION_CLEARTEXTURES; continue;
		}
		if (line == "[CLEARSPRITES]") {
			section = SCENE_SECTION_CLEARSPRITES; continue;
		}
		if (line == "[CLEARANIMATIONS]") {
			section = SCENE_SECTION_CLEARANIMATIONS; continue;
		}
		if (line == "[CLEARANIMATIONSETS]") {
			section = SCENE_SECTION_CLEARANIMATION_SETS; continue;
		}
		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_CLEARTEXTURES: _ParseSection_CLEARTEXTURES(line); break;
		case SCENE_SECTION_CLEARSPRITES: _ParseSection_CLEARSPRITES(line); break;
		case SCENE_SECTION_CLEARANIMATIONS: _ParseSection_CLEARANIMATIONS(line); break;
		case SCENE_SECTION_CLEARANIMATION_SETS: _ParseSection_CLEARANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();
	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 0));		//boundingbox
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}
#pragma region keyhandler

void PlayScenceKeyHandler::KeyState(BYTE* states)
{
	//int _SophiaType = ((PlayScene*)scence)->_SophiaType;
	//JASON* jason = ((PlayScene*)scence)->jason;
	//Small_Sophia* ssophia = ((PlayScene*)scence)->ssophia;
	//if (jason->GetState() == SOPHIA_STATE_DIE) return;

	LPGAMEPLAYER player = ((PlayScene*)scence)->player;
	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
	{
		if (player->GetPlayerType() == EntityType::TAG_JASON)
			player->SetState(SOPHIA_STATE_WALKING_RIGHT);
		if (player->GetPlayerType() == EntityType::TAG_SMALL_SOPHIA)
			player->SetState(SMALL_SOPHIA_STATE_WALKING_RIGHT);

	}
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
	{
		if (player->GetPlayerType() == EntityType::TAG_JASON) {
			player->SetState(SOPHIA_STATE_WALKING_LEFT);
		}
		if (player->GetPlayerType() == EntityType::TAG_SMALL_SOPHIA) {
			player->SetState(SMALL_SOPHIA_STATE_WALKING_LEFT);
		}

	}
	else
	{
		if (player->GetPlayerType() == EntityType::TAG_JASON)
			player->SetState(SOPHIA_STATE_IDLE);
		if (player->GetPlayerType() == EntityType::TAG_SMALL_SOPHIA)
			player->SetState(SMALL_SOPHIA_STATE_IDLE);

	}

	if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
	{
		if (player->GetPlayerType() == EntityType::TAG_JASON)
			dynamic_cast<JASON*>(player)->SetPressSpace(true);
		if (player->GetPlayerType() == EntityType::TAG_SMALL_SOPHIA)
			dynamic_cast<Small_Sophia*>(player)->SetPressSpace(true);
	}

	//if (CGame::GetInstance()->IsKeyDown(DIK_UP))
	//{
	//	if (_SophiaType == ID_JASON)
	//		player->SetP(true);
	//}
	//if (CGame::GetInstance()->IsKeyDown(DIKEYBOARD_LSHIFT))
	//{
	//	if (_SophiaType == ID_JASON)
	//		player
	//}
	if (CGame::GetInstance()->IsKeyDown(DIK_UP))
	{
		if (player->GetPlayerType() == EntityType::TAG_JASON)
			dynamic_cast<JASON*>(player)->SetPressUp(true);
	}

}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//int _SophiaType = ((PlayScene*)scence)->_SophiaType;
	//Small_Sophia* ssophia = ((PlayScene*)scence)->ssophia;
	//JASON* jason = ((PlayScene*)scence)->jason;
	//vector<LPGAMEENTITY> listEnemies = ((PlayScene*)scence)->listEnemies;
	//vector<LPBULLET> listBullets = ((PlayScene*)scence)->listBullets;
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	float xPos, yPos;
	bool isAimingTop;
	int nx, ny, dam;
	LPGAMEPLAYER player = ((PlayScene*)scence)->player;

	switch (player->GetPlayerType())
	{
	case EntityType::TAG_JASON:
		dynamic_cast<JASON*>(player)->GetInfoForBullet(nx, isAimingTop, xPos, yPos);
		break;
	case EntityType::TAG_SMALL_SOPHIA:
		dynamic_cast<Small_Sophia*>(player)->GetInfoForBullet(nx, xPos, yPos);
		break;
	default:
		break;
	}

	switch (KeyCode)
	{
	case DIK_ESCAPE:
		DestroyWindow(CGame::GetInstance()->GetWindowHandle());
		break;
	case DIK_SPACE:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_JASON:
			player->SetState(SOPHIA_STATE_JUMP);
			break;
		case EntityType::TAG_SMALL_SOPHIA:
			player->SetState(SMALL_SOPHIA_STATE_JUMP);
			break;
		}
		break;
	case DIK_LSHIFT:
		/*if (_SophiaType == ID_JASON)
			((PlayScene*)scence)->changePlayer();
		else if (_SophiaType== ID_SMALL_SOPHIA)
		{
			((PlayScene*)scence)->changePlayer();
		}*/
		((PlayScene*)scence)->changePlayer();
		break;
	case DIK_A:
	{
		playScene->Unload();
		((PlayScene*)scence)->player = new JASON(30, 60, PLAYER_MAX_HEALTH, PLAYER_DEFAULT_GUNDAM);
		
		//((PlayScene*)scence)->player->SetHealth(PLAYER_MAX_HEALTH);
		((PlayScene*)scence)->player->SetIsDoneDeath(false);
		((PlayScene*)scence)->player->SetIsDeath(false);
		playScene->ChooseMap(ID_AREA1);

		break;
	}
	case DIK_Z:
	{
		player->FireBullet(0);
		switch (player->GetPlayerType())
		{

		case EntityType::TAG_JASON:
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_NORMAL_BULLET, player->Getx(), player->Gety(), 3))
			{
				Bullet* bullet = new JasonBullet(player->Getx(), player->Gety(), 0, nx, isAimingTop);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			break;
		case EntityType::TAG_SMALL_SOPHIA:
			if (CGrid::GetInstance()->CheckBulletLimitation(SMALL_SOPHIA_NORMAL_BULLET, player->Getx(), player->Gety(), 3))
			{
				Bullet* bullet = new SmallSophiaBullet(player->Getx(), player->Gety(), 0, nx);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			break;
		default:
			break;
		}

		break;
	}

	case DIK_X:
	{
		if (CGrid::GetInstance()->CheckBulletLimitation(JASON_UPGRADE_BULLET, player->Getx(), player->Gety(),3))
		{
			Bullet* bullet = new JasonBullet(player->Getx(), player->Gety(), 1, nx, isAimingTop);
			CGrid::GetInstance()->InsertGrid(bullet);
		}
		break;
	}
	case DIK_V:
	{
		if (CGrid::GetInstance()->CheckBulletLimitation(JASON_UPGRADE_BULLET, player->Getx(), player->Gety(),3))
		{
			Bullet* bullet1 = new JasonBullet(player->Getx(), player->Gety(), 0, nx, isAimingTop);
			Bullet* bullet2 = new JasonBullet(player->Getx(), player->Gety(), 0, nx, isAimingTop);
			Bullet* bullet3 = new JasonBullet(player->Getx(), player->Gety(), 0, nx, isAimingTop);
			CGrid::GetInstance()->InsertGrid(bullet1);
			CGrid::GetInstance()->InsertGrid(bullet2);
			CGrid::GetInstance()->InsertGrid(bullet3);
		}
		break;
	}
	//case DIK_C:
	//	//if (listBullets.size() < 3)
	//	//{
	//	//	Bullet* bullet = new JasonRocket(player->Getx(), player->Gety());
	//	//	((PlayScene*)scence)->listBullets.push_back(bullet);
	//	//}
	//	//break;
	//case DIK_F2:
	//	//if (player->GetBBARGB() == 255)
	//	//{
	//	//	player->SetBBARGB(0);
	//	//}
	//	//else player->SetBBARGB(255);

	//	//for (int i = 0; i < ((PlayScene*)scence)->listBullets.size(); i++)
	//	//{
	//	//	if (((PlayScene*)scence)->listBullets[i]->GetBBARGB() == 255)
	//	//	{
	//	//		DebugOut(L"dan mat mau");
	//	//		((PlayScene*)scence)->listBullets[i]->SetBBARGB(0);
	//	//	}
	//	//	else ((PlayScene*)scence)->listBullets[i]->SetBBARGB(255);
	//	//}
	//	//break;
	}
}

void PlayScene::changePlayer()
{
	if (player->GetPlayerType() == EntityType::TAG_JASON)
	{
		playerInfo.jasonHealth = player->GetHealth();
		playerInfo.jasonGundam = player->GetgunDam();
		this->player->SetState(SOPHIA_STATE_OUT);
		backup_player = player;
		player = new Small_Sophia(backup_player->Getx(), backup_player->Gety(),playerInfo.sophiaHealth,playerInfo.sophiaGundam);
		player->SetState(SMALL_SOPHIA_STATE_OUT);

		CGrid::GetInstance()->SetTargetForEnemies(player);
	}
	else if (player->GetPlayerType() == EntityType::TAG_SMALL_SOPHIA)
	{
		if (abs(backup_player->x - player->x) < DISTANCE_TO_OUT && abs(backup_player->y - player->y) < DISTANCE_TO_OUT)
		{
			playerInfo.sophiaHealth = player->GetHealth();
			playerInfo.sophiaGundam = player->GetgunDam();
			this->player->SetState(SMALL_SOPHIA_STATE_IDLE);
			this->player->SetState(SMALL_SOPHIA_STATE_OUT);


			//this->_SophiaType = ID_JASON;
			delete player;
			player = backup_player;
			backup_player=NULL;
			CGrid::GetInstance()->SetTargetForEnemies(player);
		}
	}
}

void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//JASON* jason = ((PlayScene*)scence)->jason;
	//Small_Sophia* ssophia = ((PlayScene*)scence)->ssophia;
	
	LPGAMEPLAYER player = ((PlayScene*)scence)->player;
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	switch (KeyCode)
	{
	case DIK_UP:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_JASON:
			dynamic_cast<JASON*>(player)->SetPressUp(false);
			player->SetState(SOPHIA_STATE_GUN_UNFLIP);
			break;
		}
		break;
	case DIK_SPACE:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_JASON:
			dynamic_cast<JASON*>(player)->SetPressSpace(false);
			break;
		case EntityType::TAG_SMALL_SOPHIA:
			dynamic_cast<Small_Sophia*>(player)->SetPressSpace(false);
			break;
		}
		break;
	}
}
#pragma endregion
#pragma region parseSection
void PlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);

	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
	DebugOut(L"Added animationset %d \n", ani_set_id);
}
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	CGrid::GetInstance()->LoadGrid(tokens, player);
}
void PlayScene::_ParseSection_CLEARTEXTURES(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CTextures::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Texture %d!\n", idClear);
}
void PlayScene::_ParseSection_CLEARSPRITES(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CSprites::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Sprite %d!\n", idClear);
}
void PlayScene::_ParseSection_CLEARANIMATIONS(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CAnimations::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation %d!\n", idClear);
}
void PlayScene::_ParseSection_CLEARANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CAnimationSets::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation Set %d!\n", idClear);
}
void PlayScene::_ParseSection_SCENEFILEPATH(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;

	listSceneFilePath.push_back(ToLPCWSTR(tokens[0]));
	listWidth.push_back(atoi(tokens[1].c_str()));
	listHeight.push_back(atoi(tokens[2].c_str()));
}
#pragma endregion

PlayScene::~PlayScene()
{
}

//void PlayScene::SwitchScene(int scene_id)
//{
//	DebugOut(L"dung cong");
//}
void PlayScene::CheckPlayerReachGate()
{
	if (dynamic_cast<JASON*>(player)->GetGateColliding())
	{
		Gate* gate = dynamic_cast<JASON*>(player)->GetGate();
		DebugOut(L"[Info] success\n");
		//dynamic_cast<JASON*>(player)->SetPlayerType(gate->typePlayer);
		int tempMap = gate->GetIdScene();
		float tempx = gate->newPlayerx;
		float tempy = gate->newPlayery;
		int tempState = gate->newPlayerState;
		tempNeed = gate->directionCam;
		camMap1X = gate->camPosX;
		camMap1Y = gate->camPosY;
		
		DebugOut(L"posX: %d, posY: %d\n", camMap1X, camMap1Y);

		playerInfo.jasonGundam = player->GetgunDam();
		playerInfo.jasonHealth = player->GetHealth();
		Unload();

		ChooseMap(tempMap);
		switch (gate->typePlayer)
		{
		case EntityType::TAG_JASON:
			player = new JASON(tempx, tempy, playerInfo.jasonHealth, playerInfo.jasonGundam);
			player->SetState(tempState);
		default:
			break;
		}
		CGrid::GetInstance()->SetTargetForEnemies(player);
		//DebugOut(L"xong chuyen cong %d\n ", idStage);
		//jason->SetGateColliding(false);
		//jason->ResetGate();
		//jason->SetPosition(tempx, tempy);
		//jason->Setvx(0);
		//jason->Setvy(0);
		//jason->SetState(tempState);
	}
}
void PlayScene::Update(DWORD dt)
{
	if (player->GetPlayerType() == EntityType::TAG_JASON)
	{
		//DebugOut(L"done reachgete\n");
		CheckPlayerReachGate();
	}
	//DebugOut(L"Update first line %d\n ", idStage);
#pragma region camera
	float cx, cy;
	mapWidth = listWidth[(idStage% 10) - 1];
	mapHeight= listHeight[(idStage % 10) - 1];
	switch (player->GetPlayerType())
	{
	case EntityType::TAG_JASON:
		player->GetPosition(cx, cy);
		if (tempNeed)
		{
			gameCamera->SetCamPos(camMap1X, camMap1Y);
			posY = camMap1Y;
			tempNeed = 0;
		}
		else
		{
			if (player->Getx() + SCREEN_WIDTH / 2 >= mapWidth)
				cx = mapWidth - SCREEN_WIDTH;
			else
			{
				if (player->Getx() < SCREEN_WIDTH / 2)
					cx = 0;
				else
					cx -= SCREEN_WIDTH / 2;
			}
			if (cy - posY > SCREEN_HEIGHT)
			{
				posY = cy - SCREEN_HEIGHT / 2;
			}
			if (cy + SCREEN_HEIGHT/1.85 >= mapHeight)
			{
				cy = mapHeight - SCREEN_HEIGHT;
				posY = cy;
			}
			else
			{
				if (cy < SCREEN_HEIGHT / 4)
				{
					posY = 0;
				}
				else
				{
					if ((cy - posY) < (SCREEN_HEIGHT / 4))
					{
						posY -= CAMERA_SPEED_WORLD1 * dt;
					}
					if ((cy - posY) > (SCREEN_HEIGHT / 2))
					{
						posY += CAMERA_SPEED_WORLD1 * dt;
					}
				}
			}
			gameCamera->SetCamPos(cx, posY);
		}
		break;
	case EntityType::TAG_SMALL_SOPHIA:
	{
		player->GetPosition(cx, cy);
		if (player->Getx() + SCREEN_WIDTH / 2 >= mapWidth)
			cx = mapWidth - SCREEN_WIDTH;
		else
		{
			if (player->Getx() < SCREEN_WIDTH / 2)
				cx = 0;
			else
				cx -= SCREEN_WIDTH / 2;
		}
		cy -= SCREEN_HEIGHT / 2;
		gameCamera->SetCamPos(cx, cy);//cy khi muon camera move theo y player
		break;
	}
	}
#pragma endregion
#pragma region sceneswitching

	//DebugOut(L"middle\n");
#pragma endregion
	if (isUnloaded)
	{
		CGrid::GetInstance()->SetTargetForEnemies(player);
		isUnloaded = false;
	}
	//init coObjects
	//DebugOut(L"before - get gird update update\n");

	vector<LPGAMEENTITY> coObjects = CGrid::GetInstance()->GetListUpdateObj(gameCamera->GetRectCam());
	//DebugOut(L"before - player update\n");
	if (player != NULL)
	{
		//DebugOut(L"pre - player update\n");
		player->Update(dt, &coObjects);
		//DebugOut(L"player update\n");
	}

	//ssophia->Update(dt,&coObjects);
	if (coObjects.size() != 0)
	{//update obj
		for (int i = 0; i < coObjects.size(); i++)
		{
			if (coObjects.at(i)->GetType()!=EntityType::TAG_BRICK && coObjects.at(i)->GetType() != EntityType::TAG_GATE)
			{
				coObjects[i]->Update(dt, &coObjects);
			}
		}
		//sua cho nay
		int k = 0;
		for (int i = 0; i<coObjects.size()-k; i++)
		{
			if ((coObjects.at(i)->isDeath()))
			{
				float xPos, yPos;
				coObjects.at(i)->GetPosition(xPos, yPos);
				LPGAMEENTITY backup = coObjects.at(i);

				coObjects.erase(coObjects.begin() + i);

				float _xtemp, _ytemp;
				backup->GetPosition(_xtemp, _ytemp);
#pragma region add item into grid
				switch (backup->GetType())
				{
				case EntityType::ENEMY:
				{
					LPGAMEENTITY _PowerUp = new PowerUp(xPos, yPos);
					CGrid::GetInstance()->InsertGrid(_PowerUp);
					break;
				}
				default:
					break;
				}
#pragma endregion
				CGrid::GetInstance()->RemoveObj(backup,true);
				k = 1;
				i--;
			}
			//item effect
			else {
				k = 0;
			}
		}
	}
	CGrid::GetInstance()->UpdateGrid(coObjects);
	//player

	gameHUD->Update(cx, HUD_Y + posY, player->GetHealth(), player->GetgunDam());

}

void PlayScene::Render()
{
	//idStage / STAGE_1 + 10
	//DebugOut(L"Render first line %d\n ", idStage);
	LPDIRECT3DTEXTURE9 maptexture = CTextures::GetInstance()->Get(idStage);
	CGame::GetInstance()->OldDraw(0, 0, maptexture, 0, 0, mapWidth, mapHeight);
	vector<LPGAMEENTITY> coObjects = CGrid::GetInstance()->GetListRenderObj(gameCamera->GetRectCam());
	for (int i = 0; i < coObjects.size(); i++)
		coObjects[i]->Render();
	switch (player->GetPlayerType())
	{
	case EntityType::TAG_JASON:
		player->Render();
		break;
	case EntityType::TAG_SMALL_SOPHIA:
		backup_player->Render();
		player->Render();

	}
	gameHUD->Render(player);

}
void PlayScene::Unload()
{
	CGrid::GetInstance()->UnLoadGrid();

	posX = posY = 0;
	delete player;
	isUnloaded = true;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
