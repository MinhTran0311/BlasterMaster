﻿#pragma once
#include "Worms.h"
#include "Insects.h"
#include "PlayScene.h"
#include "Skulls.h"
#include "Orbs.h"
#include "Jumpers.h"
#include "JasonRocket.h"
#include "JasonBullet.h"
#include "JasonBullet.h"
#include "SmallSophiaBullet.h"
#include "Grid.h"
#include "Big_Sophia.h"
#include "IntroScene.h"
#include "ChooseWeaponScene.h"
#include "Random.h"
#include "Boss.h"
#include "Item.h"
#include "PlayerHandler.h"
#include "MapManager.h"
#define MAP2_SIDE	200

#define HUD_Y (SCREEN_HEIGHT/2.5)

PlayScene::PlayScene()
{
}

PlayScene::PlayScene(int _idStage) : Scene()
{
	idStage = _idStage;
	keyHandler = new PlayScenceKeyHandler(this);
	PlayerHandler::GetInstance()->Init();
	LoadBaseObjects();
	ChooseMap(idStage);
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_UNDERWORLD);
	Sound::GetInstance()->Play("MusicMap", 1, 10000);
	srand(time(NULL));

}
void PlayScene::LoadBaseObjects()
{
	texturesFilePath = ToLPCWSTR("Resource\\SceneAndSpec\\base_playscene.txt");
	LoadBaseTextures();
#pragma region create_base_objects
	if (player == NULL)
	{
		player = new JASON(PlayerHandler::GetInstance()->GetJasonXPos(), PlayerHandler::GetInstance()->GetJasonYPos(), PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
		DebugOut(L"[INFO] JASON CREATED!!! health:%d, dam:%d \n", player->GetHealth(), player->GetgunDam());

		PlayerHandler::GetInstance()->SetJasonStage(ID_AREA1);
	}
	HUD::GetInstance()->HUDInit(player->GetHealth(), player->GetgunDam());
#pragma endregion
	Camera::GetInstance()->SetCamPos(0.0f, 0.0f);	//initial camera
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
	//sceneFilePath = listSceneFilePath[idStage - 11];
	//mapWidth = listWidth[idStage - 11];
	//mapHeight = listHeight[idStage - 11];
	MapManager::GetIntance()->SetCurrentStage(idStage);
	sceneFilePath = MapManager::GetIntance()->GetFilePath(idStage);
	mapWidth = MapManager::GetIntance()->GetMapWidth(idStage);
	mapHeight = MapManager::GetIntance()->GetMapHeight(idStage);
	DebugOut(L"Init");
	CGrid::GetInstance()->InitGrid(mapWidth, mapHeight);
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
	LPGAMEPLAYER player = ((PlayScene*)scence)->player;

	if (player->GetPlayerType() == TAG_BIG_SOPHIA)
		if (dynamic_cast<Big_Sophia*>(player)->isAutoRun())
		{
			return;
		}
	if (player->GetPlayerType() != TAG_SMALL_SOPHIA)
	{
		bool idleState = true;
		player->SetState(JASON_STATE_IDLE);
		if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
		{
			player->SetState(JASON_STATE_WALKING_RIGHT);
			//idleState = false;
		}
		if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
		{
			player->SetState(JASON_STATE_WALKING_LEFT);
			//idleState = false;
		}
		if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
		{
			if (player->GetPlayerType() == EntityType::TAG_JASON)
			{
				player->SetState(JASON_STATE_IDLE);
				dynamic_cast<JASON*>(player)->SetPressSpace(true);
			}
			//idleState = false;
		}

		if (CGame::GetInstance()->IsKeyDown(DIK_UP))
		{
			if (player->GetPlayerType() == TAG_JASON)
			{
				player->SetState(JASON_STATE_IDLE);
				dynamic_cast<JASON*>(player)->SetPressUp(true);

			}
			else if (player->GetPlayerType() == TAG_BIG_SOPHIA)
				dynamic_cast<Big_Sophia*>(player)->SetState(BIG_SOPHIA_STATE_WALKING_TOP);
			//idleState = false;
		}
		if ((CGame::GetInstance()->IsKeyDown(DIK_DOWN)))
		{
			if (player->GetPlayerType() == TAG_BIG_SOPHIA)
				dynamic_cast<Big_Sophia*>(player)->SetState(BIG_SOPHIA_STATE_WALKING_BOTTOM);
			//idleState = false;
		}
		//if (idleState) player->SetState(JASON_STATE_IDLE);

	}
	else
	{
		if (dynamic_cast<Small_Sophia*>(player)->IsInStairs())
		{
			if (CGame::GetInstance()->IsKeyDown(DIK_UP))
				dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CLIMB_UP);
			else if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
				dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CLIMB_DOWN);
			else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
				dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_WALKING_LEFT);
			else if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
				dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_WALKING_RIGHT);
			else
				dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CLIMB_IDLE);
		}
		else
		{
			if ((CGame::GetInstance()->IsKeyDown(DIK_SPACE)))
				dynamic_cast<Small_Sophia*>(player)->SetPressSpace(true);
			else if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
			{
				player->SetState(SMALL_SOPHIA_STATE_WALKING_RIGHT);
			}
			else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
			{
				player->SetState(SMALL_SOPHIA_STATE_WALKING_LEFT);
			}
			else
					player->SetState(SOPHIA_STATE_IDLE);
			if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
			{
				dynamic_cast<Small_Sophia*>(player)->SetIsCrawl(true);
			}
		}
	}
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	if (playScene->GetInforDisplay() == LIFE_DISPLAY)
	{
		if (PlayerHandler::GetInstance()->GetLife() < 0) {
			if (KeyCode == DIK_DOWN)playScene->select_end = true;
			if (KeyCode == DIK_UP)playScene->select_end = false;
			if (KeyCode == DIK_RETURN) {
				if (playScene->select_end == true) {
					DestroyWindow(CGame::GetInstance()->GetWindowHandle());
				}
				else {
					//PlayerHandler::GetInstance()->SetLife(2);
					SceneManager::GetInstance()->SetScene(new PlayScene(ID_AREA1));
					delete this;
				}
			}
		}
	}
	else {
		if (KeyCode == DIK_RETURN)
		{
			if (playScene->GetInforDisplay() != CHOOSING_WEAPON_DISPLAY)
				playScene->SetInforDisplay(CHOOSING_WEAPON_DISPLAY);
		}
		else
		{
			float xPos, yPos;
			bool isAimingTop;
			int nx, ny, dam;
			LPGAMEPLAYER player = ((PlayScene*)scence)->player;
			if (player->GetPlayerType() == TAG_BIG_SOPHIA)
				if (dynamic_cast<Big_Sophia*>(player)->isAutoRun())
					return;
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
					player->SetState(JASON_STATE_JUMP);
					break;
				case EntityType::TAG_SMALL_SOPHIA:
					player->SetState(SMALL_SOPHIA_STATE_JUMP);
					break;
				}
				break;
			case DIK_LSHIFT:
				((PlayScene*)scence)->changePlayer();
				break;
			case DIK_A:
			{
				playScene->Unload();
				PlayerHandler::GetInstance()->Init();
				((PlayScene*)scence)->player = new JASON(PlayerHandler::GetInstance()->GetJasonXPos(), PlayerHandler::GetInstance()->GetJasonYPos(), PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
				//PlayerHandler::GetInstance()->Init();
				playScene->ChooseMap(ID_AREA1);

				break;
			}
			case DIK_Z:
			{
				//single fire
				player->FireBullet(1);
				break;
			}
			case DIK_V:
			{
				if (player->GetPlayerType() == TAG_JASON)
				//burst fire
				player->FireBullet(2);
				break;
			}
			case DIK_C:
			{
				player->FireBullet(3);
				break;
			}
			//case DIK_UP:
			//{
			//	if (dynamic_cast<Big_Sophia*>(player))
			//	{
			//		player->SetDirectionY(0);
			//	}
			//	break;
			//}
			//case DIK_DOWN:
			//{
			//	if (dynamic_cast<Big_Sophia*>(player))
			//	{
			//		player->SetDirectionY(0);
			//	}
			//	break;
			//}
			}
		}
	}
}

void PlayScene::changePlayer()
{

	if (player->GetPlayerType() == EntityType::TAG_JASON && !dynamic_cast<JASON*>(player)->IsGunFlip() && !dynamic_cast<JASON*>(player)->IsJumping())
	{
		PlayerHandler::GetInstance()->SetJasonInfor(idStage, player->Getx(), player->Gety(), player->GetHealth(), player->GetgunDam());
		PlayerHandler::GetInstance()->SetSophiaStage(idStage);

		this->player->SetState(JASON_STATE_OUT);
		backup_player = player;
		//player = new Small_Sophia(backup_player->Getx(), backup_player->Gety(), playerInfo.sophiaHealth, playerInfo.sophiaGundam);
		player = new Small_Sophia(backup_player->Getx(), backup_player->Gety(), PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());
		player->SetState(SMALL_SOPHIA_STATE_OUT);

		CGrid::GetInstance()->SetTargetForEnemies(player);
	}
	else if (player->GetPlayerType() == EntityType::TAG_SMALL_SOPHIA)
	{
		if (abs(backup_player->Getx() - player->Getx()) < DISTANCE_TO_OUT && abs(backup_player->Gety() - player->Gety()) < DISTANCE_TO_OUT)
		{
			PlayerHandler::GetInstance()->SetSophiaHealth(player->GetHealth());
			PlayerHandler::GetInstance()->SetSophiaGunDam(player->GetgunDam());
			//this->player->SetState(SMALL_SOPHIA_STATE_IDLE);
			this->player->SetState(SMALL_SOPHIA_STATE_OUT);

			delete player;
			player = backup_player;
			backup_player = NULL;
			DebugOut(L"player: %d", (int)player->GetBBox().bottom);
			CGrid::GetInstance()->SetTargetForEnemies(player);
		}
	}
	Sound::GetInstance()->Play("EjectJason", 0, 1);
}
void PlayScene::CheckEnterBoss()
{
	if (player->GetPlayerType() == TAG_BIG_SOPHIA && player->Getx() > 871 && player->Getx() < 910 && player->Gety() > 627 && player->Gety() < 653 && !dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea() && idStage == ID_MAPOVERWORLD)
	{
		dynamic_cast<Big_Sophia*>(player)->SetIsEnterIntroBossArea(true);
		BossIntroTimer->Start();
		Sound::GetInstance()->Stop("MusicMap");
		Sound::GetInstance()->Play("BossIntro", 1);
	}
}

void PlayScene::SetUpFightBoss()
{
	dynamic_cast<Big_Sophia*>(player)->SetIsEnterIntroBossArea(false);
	player->SetPosition(1790, 1130);
	Camera::GetInstance()->SetCamPos(1607, 995);
	Camera::GetInstance()->SetIsFollowPlayer(false);
	Sound::GetInstance()->Stop("BossIntro");
	Sound::GetInstance()->Play("Boss", 1);
}

void PlayScene::BossAreaController()
{
	if (!PlayerHandler::GetInstance()->IsWinBoss())
	{
		//intro
		if (!dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea() && !dynamic_cast<Big_Sophia*>(player)->IsFightWithBoss() && !dynamic_cast<Big_Sophia*>(player)->IsDoneFightWithBoss())
			CheckEnterBoss();
		if (BossIntroTimer->IsTimeUp() && dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea())
		{
			dynamic_cast<Big_Sophia*>(player)->SetIsFightWithBoss(true);
			BossIntroTimer->Reset();
			textureAlpha = 255;
		}
		//enter boss area
		if (dynamic_cast<Big_Sophia*>(player)->IsFightWithBoss() && dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea())
		{
			SetUpFightBoss();
		}
		//out
		else if (dynamic_cast<Big_Sophia*>(player)->IsFightWithBoss() && dynamic_cast<Big_Sophia*>(player)->IsDoneFightWithBoss())
		{
			Sound::GetInstance()->Stop("BossDead");
			Sound::GetInstance()->Play("Award", 0, 1);
			dynamic_cast<Big_Sophia*>(player)->SetIsFightWithBoss(false);
			LPGAMEENTITY award = new Item(892, 590, TAG_ITEM_CRUSHER_BEAM, true);
			CGrid::GetInstance()->InsertGrid(award);

			player->SetPosition(880, 656);
			Camera::GetInstance()->SetCamPos(767, 479);
			BossIntroTimer->Start();
		}
		// back to return gate
		if (BossIntroTimer->IsTimeUp() && dynamic_cast<Big_Sophia*>(player)->IsDoneFightWithBoss())
		{
			player->SetPosition(150, 1905);
			Camera::GetInstance()->SetCamPos(0, 1774);
			BossIntroTimer->Reset();
			//dynamic_cast<Big_Sophia*>(player)->SetIsDoneFightWithBoss(false);
			Sound::GetInstance()->Stop("Award");
			Sound::GetInstance()->Play("MusicMap", 1, 10000);
			PlayerHandler::GetInstance()->SetWinBoss(true);
		}
	}
}

void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{

	LPGAMEPLAYER player = ((PlayScene*)scence)->player;
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	if (player->GetPlayerType() == TAG_BIG_SOPHIA)
	{
		if (dynamic_cast<Big_Sophia*>(player)->isAutoRun())
			return;
	}
	switch (KeyCode)
	{
	case DIK_UP:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_JASON:
			dynamic_cast<JASON*>(player)->SetPressUp(false);
			player->SetState(JASON_STATE_GUN_UNFLIP);
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
	case DIK_DOWN:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_SMALL_SOPHIA:
			dynamic_cast<Small_Sophia*>(player)->SetIsCrawl(false);
			player->SetPosition(player->Getx(), player->Gety() - 10);
			break;
		}
		break;



	case DIK_LEFT:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_SMALL_SOPHIA:
			dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CRAWL_STOP);
			break;
		}
		break;



	case DIK_RIGHT:
		switch (player->GetPlayerType())
		{
		case EntityType::TAG_SMALL_SOPHIA:
			dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CRAWL_STOP);
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
	Map* map = new Map(atoi(tokens[0].c_str()), ToLPCWSTR(tokens[1]), atoi(tokens[2].c_str()), atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), atoi(tokens[5].c_str()));
	MapManager::GetIntance()->Add(atoi(tokens[0].c_str()),map);
}
#pragma endregion

PlayScene::~PlayScene()
{
}

void PlayScene::CheckPlayerReachGate()
{
	if (player->GetGateColliding() && !Camera::GetInstance()->IsFinishPassScene())
	{
		switch (player->GetPlayerType())
		{
		case TAG_JASON:
		{
			Gate* gate = dynamic_cast<Gate*>(player->GetGate());
			if (gate->GetIdScene()== ID_INTROENDING)
			{
				SceneManager::GetInstance()->SetScene(new IntroScene(ID_INTROENDING));
			}
			else if (gate->GetIdScene() != ID_MAPOVERWORLD)
			{
				//playerInfo.jasonStage = gate->GetIdScene();
				//playerInfo.jasonXPos = gate->GetNewPlayerX();
				//playerInfo.jasonYPos = gate->GetNewPlayerY();
				//isNeedResetCamera = gate->directionCam;
				//DebugOut(L"camposX: %d, camposY: %d\n", camMap1X, camMap1Y);
				//DebugOut(L"posX: %f, posY: %f\n", playerInfo.jasonXPos, playerInfo.jasonYPos);
				//playerInfo.jasonGundam = player->GetgunDam();
				//playerInfo.jasonHealth = player->GetHealth();
				//playerInfo.playerDirectionBeforePassGate = player->GetDirection();



				PlayerHandler::GetInstance()->SetJasonInfor(gate->GetIdScene(), gate->GetNewPlayerX(), gate->GetNewPlayerY(), player->GetHealth(), player->GetgunDam(), player->GetDirection());
				PlayerHandler::GetInstance()->SetCamFollow(gate->IsCamFollowPlayer());
				camMap1X = gate->GetNewCamXPos();
				camMap1Y = gate->GetNewCamYPos();
				int tempState = gate->GetNewPlayerState();


				CamMoveDirection = gate->GetMoveDirection();
				//DebugOut(L"move directoin: %d", CamMoveDirection);
				player->SetAutoRun(true);



				//	Unload();
				//isNeedResetCamera = true;
				//	//ChooseMap(playerInfo.jasonStage);
				//	ChooseMap(PlayerHandler::GetInstance()->GetJasonStage());
				//	Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());
				//	//player = new JASON(playerInfo.jasonXPos, playerInfo.jasonYPos, playerInfo.jasonHealth, playerInfo.jasonGundam);
				//	//player->SetDirection(playerInfo.playerDirectionBeforePassGate);
				//	player = new JASON(gate->GetNewPlayerX(), gate->GetNewPlayerY(), PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
				//	player->SetDirection(PlayerHandler::GetInstance()->GetPlayerDirectionBeforePassGate());
				//	player->SetState(tempState);
				//	//CGrid::GetInstance()->SetTargetForEnemies(player);
				//}
			}
			break;
		}
		case TAG_SMALL_SOPHIA:
		{
			Gate* gate = dynamic_cast<Gate*>(player->GetGate());
			if (gate->GetIdScene() != ID_INTROENDING)
			{
			//playerInfo.sophiaStage = gate->GetIdScene();
			//playerInfo.sophiaXPos = gate->GetNewPlayerX();
			//playerInfo.sophiaYPos = gate->GetNewPlayerY();
			//isNeedResetCamera = gate->directionCam;
			//DebugOut(L"camposX: %d, camposY: %d\n", camMap1X, camMap1Y);
			//DebugOut(L"posX: %d, posY: %d\n", playerInfo.sophiaXPos, playerInfo.sophiaYPos);
			//playerInfo.sophiaGundam = player->GetgunDam();
			//playerInfo.sophiaHealth = player->GetHealth();
			//playerInfo.playerDirectionBeforePassGate = player->GetDirection();
			//int tempState = gate->GetNewPlayerState();

				PlayerHandler::GetInstance()->SetSophiaInfor(gate->GetIdScene(), gate->GetNewPlayerX(), gate->GetNewPlayerY(), player->GetHealth(), player->GetgunDam(), player->GetDirection());
				PlayerHandler::GetInstance()->SetCamFollow(gate->IsCamFollowPlayer());
				camMap1X = gate->GetNewCamXPos();
				camMap1Y = gate->GetNewCamYPos();

				if (PlayerHandler::GetInstance()->GetSophiaStage() == ID_MAPOVERWORLD)
				{

					isNeedResetCamera = true;
					Unload();
					DebugOut(L"Tao overworld 1\n");
					ChooseMap(PlayerHandler::GetInstance()->GetSophiaStage());
					DebugOut(L"Tao overworld 2\n");
					Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());
					player = new Big_Sophia(gate->GetNewPlayerX(), gate->GetNewPlayerY(), PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());

				}
				else
				{
					CamMoveDirection = gate->GetMoveDirection();
					player->SetAutoRun(true);
				}
				//isNeedResetCamera = true;
				//Unload();
				//DebugOut(L"type %d\n", gate->typePlayer);
				//ChooseMap(PlayerHandler::GetInstance()->GetSophiaStage());
				//Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());
				//switch (PlayerHandler::GetInstance()->GetSophiaStage())
				//{
				//case ID_MAPOVERWORLD:
				//{
				//	player = new Big_Sophia(gate->GetNewPlayerX(), gate->GetNewPlayerY(), PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());
				//	break;
				//}
				//default:
				//{
				//	player = new Small_Sophia(gate->GetNewPlayerX(), gate->GetNewPlayerY() + 2.0f, PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());
				//	player->SetDirection(PlayerHandler::GetInstance()->GetPlayerDirectionBeforePassGate());
				//	player->SetState(tempState);
				//	if (PlayerHandler::GetInstance()->GetSophiaStage() == PlayerHandler::GetInstance()->GetJasonStage())
				//	{
				//		float xPos, yPos;
				//		PlayerHandler::GetInstance()->GetJasonPosition(xPos, yPos);
				//		backup_player = new JASON(xPos, yPos, PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
				//	}
				//	break;
				//}
				//}
				//CGrid::GetInstance()->SetTargetForEnemies(player);
			}
			break;
		}
		case TAG_BIG_SOPHIA:
		{
			if (dynamic_cast<GateOverworld*>(player->GetGate()))
			{
				GateOverworld* gate = dynamic_cast<GateOverworld*>(player->GetGate());
				if ((gate->GetCamDirection() == 1 && player->GetDirection() != 0) || (gate->GetCamDirection() == 2 && player->GetDirctionY() != 0))
				{
					dynamic_cast<Big_Sophia*>(player)->AutoRun(gate->GetCamDirection());
					xPosCamGo = gate->GetXPosGo();
					xPosCamBack = gate->GetXPosBack();
					yPosCamGo = gate->GetYPosGo();
					yPosCamBack = gate->GetYPosBack();
					CamMoveDirection = gate->GetCamDirection();
					Camera::GetInstance()->SetIsFollowPlayer(false);
				}
				else
					break;
			}
			else
			{
				CamMoveDirection =	0;

				Gate* gate = dynamic_cast<Gate*>(player->GetGate());

				//DebugOut(L"[Info] return gate success\n");
				//playerInfo.sophiaStage = gate->GetIdScene();
				//playerInfo.sophiaXPos = gate->GetNewPlayerX();
				//playerInfo.sophiaYPos = gate->GetNewPlayerY();
				//isNeedResetCamera = gate->directionCam;
				//camMap1X = gate->GetNewCamXPos();
				//camMap1Y = gate->GetNewCamYPos();
				//playerInfo.sophiaGundam = player->GetgunDam();
				//playerInfo.sophiaHealth = player->GetHealth();
				//isNeedResetCamera = true;

				PlayerHandler::GetInstance()->SetSophiaInfor(gate->GetIdScene(), gate->GetNewPlayerX(), gate->GetNewPlayerY(), player->GetHealth(), player->GetgunDam(), player->GetDirection());
				int tempState = gate->GetNewPlayerState();
				camMap1X = gate->GetNewCamXPos();
				camMap1Y = gate->GetNewCamYPos();
				isNeedResetCamera = true;

				Unload();
				ChooseMap(PlayerHandler::GetInstance()->GetSophiaStage());

				Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());

				player = new Small_Sophia(gate->GetNewPlayerX(), gate->GetNewPlayerY(), PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());
				float xPos, yPos;
				PlayerHandler::GetInstance()->GetJasonPosition(xPos, yPos);
				backup_player = new JASON(xPos,yPos, PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
			}
			break;
		default:
			CamMoveDirection = 0;
			break;
		}
		}
	}
	else if (Camera::GetInstance()->IsFinishPassScene() && player->isAutoRun())
	{
		switch (player->GetPlayerType())
		{
		case TAG_JASON:
		{
			Unload();
			isNeedResetCamera = true;
			ChooseMap(PlayerHandler::GetInstance()->GetJasonStage());
			Camera::GetInstance()->SetIsFollowPlayer(PlayerHandler::GetInstance()->IsCamFollow());

			player = new JASON(PlayerHandler::GetInstance()->GetJasonXPos(), PlayerHandler::GetInstance()->GetJasonYPos(), PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
			player->SetDirection(PlayerHandler::GetInstance()->GetPlayerDirectionBeforePassGate());
			break;
		}
		case TAG_SMALL_SOPHIA:
		{
			Unload();
			isNeedResetCamera = true;
			ChooseMap(PlayerHandler::GetInstance()->GetSophiaStage());
			Camera::GetInstance()->SetIsFollowPlayer(PlayerHandler::GetInstance()->IsCamFollow());

			player = new Small_Sophia(PlayerHandler::GetInstance()->GetSophiaXPos(), PlayerHandler::GetInstance()->GetSophiaYPos()+2.0f, PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());
			player->SetDirection(PlayerHandler::GetInstance()->GetPlayerDirectionBeforePassGate());
			if (PlayerHandler::GetInstance()->GetSophiaStage() == PlayerHandler::GetInstance()->GetJasonStage())
			{
				float xPos, yPos;
				PlayerHandler::GetInstance()->GetJasonPosition(xPos, yPos);
				backup_player = new JASON(xPos, yPos, PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
			}
			break;
		}
		}
		Camera::GetInstance()->SetFinishPassScene(false);
	}
}
void PlayScene::RandomSpawnItem(LPGAMEENTITY ItemSpawer)
{
	if ((ItemSpawer->GetType() == ENEMY && !dynamic_cast<CBoss*>(ItemSpawer)) || (ItemSpawer->GetType() == TAG_SOFT_BRICK))
	{
		float prop = random->getRandomFloat(0.0f, 1.0f);
		if (idStage != ID_MAPOVERWORLD)
		{
			if (prop > 0.8f)
			{
				LPGAMEENTITY _PowerUp = new Item(ItemSpawer->Getx(), ItemSpawer->Gety(),TAG_ITEM_SINGLE_POWER_UP,true);
				CGrid::GetInstance()->InsertGrid(_PowerUp);
			}
		}
		else
		{
			if (prop > 0.7f)
			{
				LPGAMEENTITY _PowerUp = new Item(ItemSpawer->Getx(), ItemSpawer->Gety(), TAG_ITEM_SINGLE_POWER_UP,true);
				CGrid::GetInstance()->InsertGrid(_PowerUp);
			}
			else if (prop > 0.1f && prop < 0.3f)
			{
				LPGAMEENTITY _GunUp = new Item(ItemSpawer->Getx(), ItemSpawer->Gety(), TAG_ITEM_SINGLE_GUN_UP, true);
				CGrid::GetInstance()->InsertGrid(_GunUp);
			}
			else if (prop < 0.1f)
			{
				LPGAMEENTITY _Hover = new Item(ItemSpawer->Getx(), ItemSpawer->Gety(),TAG_ITEM_HOVER,true);
				CGrid::GetInstance()->InsertGrid(_Hover);
			}
		}
	}
}

void PlayScene::Update(DWORD dt)
{
	if (this->inforDisplay == CHOOSING_WEAPON_DISPLAY && player->GetPlayerType() == TAG_JASON)
	{
		SceneManager::GetInstance()->SetHolderScene(SceneManager::GetInstance()->GetScene());
		/*if (SceneManager::GetInstance()->GetHolderScene() == nullptr)*/
		DebugOut(L"khong null: %d\n", SceneManager::GetInstance()->GetHolderScene());
		inforDisplay = 0;
		SceneManager::GetInstance()->SetScene(new ChooseWeaponScene());

	}
	else if (this->inforDisplay == LIFE_DISPLAY)
	{

		//this->player->SetHealth(8);
		if (PlayerHandler::GetInstance()->GetLife() > 0) {

			if (this->cooldownTimer->IsTimeUp()) {

				PlayerHandler::GetInstance()->SetLife(PlayerHandler::GetInstance()->GetLife() - 1);
				Sound::GetInstance()->Play("MusicMap", 1, 10000);
				SetInforDisplay(0);
			}
		}
		else if (PlayerHandler::GetInstance()->GetLife() == 0)
		{
			PlayerHandler::GetInstance()->SetLife(-1);
			Sound::GetInstance()->Stop("");
			Sound::GetInstance()->Play("GameOver", 0, 1);
		}
		return;
	}
	else
#pragma region mạng và reset
	{
		if (player->IsDoneDeathAni())
		{
			SetInforDisplay(LIFE_DISPLAY);
			this->cooldownTimer->Start();
			if (PlayerHandler::GetInstance()->GetLife() < 0)
			{
			}
			else
			{
				isReset = true;
				Unload();
				if (player->GetPlayerType() == TAG_JASON)
				{
					ChooseMap(PlayerHandler::GetInstance()->GetJasonStage());
					player->Reset(PlayerHandler::GetInstance()->GetJasonHealth(), PlayerHandler::GetInstance()->GetJasonGunDam());
					DebugOut(L"mau: %d", PlayerHandler::GetInstance()->GetJasonHealth());
				}
				else
				{
					ChooseMap(PlayerHandler::GetInstance()->GetSophiaStage());
					player->Reset(PlayerHandler::GetInstance()->GetSophiaHealth(), PlayerHandler::GetInstance()->GetSophiaGunDam());

				}
				isNeedResetCamera = true;
			}
		}
	}

#pragma endregion
	if (player->GetPlayerType() == TAG_BIG_SOPHIA && idStage == ID_MAPOVERWORLD)
	{
		BossAreaController();
	}
	CheckPlayerReachGate();
#pragma region camera
	if (isNeedResetCamera)
	{
		DebugOut(L"middle\n");
		Camera::GetInstance()->SetCamPos(camMap1X, camMap1Y);
		isNeedResetCamera = false;
	}
	else
	{
		if (!player->IsDoneDeathAni())
		{
			//Camera::GetInstance()->Update(player->Getx(), player->Gety(), player->GetPlayerType(), dt, mapWidth, mapHeight, player->GetDirection(), player->GetDirctionY(), xPosCamGo, xPosCamBack, yPosCamGo, yPosCamBack, CamMoveDirection);
			Camera::GetInstance()->Update(player->Getx(), player->Gety(), player->GetPlayerType(), dt,mapWidth, mapHeight, player->GetDirection(), player->GetDirctionY(), xPosCamGo, xPosCamBack, yPosCamGo, yPosCamBack, CamMoveDirection);
			HUD::GetInstance()->Update(Camera::GetInstance()->GetCamx() + 20, HUD_Y + Camera::GetInstance()->GetCamy(), player->GetPlayerType(), player->GetHealth(), player->GetgunDam());
		}
	}

#pragma endregion
	if (player->isAutoRun() && player->GetPlayerType() != TAG_BIG_SOPHIA)
	{
		return;
	}

#pragma region update objects
	if (isUnloaded)
	{
		CGrid::GetInstance()->SetTargetForEnemies(player);
		isUnloaded = false;
	}

	vector<LPGAMEENTITY> coObjects = CGrid::GetInstance()->GetListUpdateObj(Camera::GetInstance()->GetRectCam());
	if (player != NULL)
	{
		player->Update(dt, &coObjects);
	}
	if (coObjects.size() != 0)
	{
		for (int i = 0; i < coObjects.size(); i++)
		{
			if (coObjects.at(i)->GetType() != EntityType::TAG_BRICK && coObjects.at(i)->GetType() != TAG_GATE && coObjects.at(i)->GetType() != TAG_GATE_OVERWORLD)
			{
				coObjects[i]->Update(dt, &coObjects);
			}
		}
		int k = 0;
		for (int i = 0; i < coObjects.size() - k; i++)
		{
			if ((coObjects.at(i)->IsDeath()))
			{
				if (coObjects.at(i)->GetType() != ITEM)
				{
					float xPos, yPos;
					coObjects.at(i)->GetPosition(xPos, yPos);
					RandomSpawnItem(coObjects.at(i));
				}
				CGrid::GetInstance()->RemoveObj(coObjects.at(i), true);
				coObjects.erase(coObjects.begin() + i);
				k = 1;
				i--;
			}
			else {
				k = 0;
			}
		}

	CGrid::GetInstance()->UpdateGrid(coObjects);
	}
#pragma endregion
}
void PlayScene::Render()
{
	if (this->inforDisplay == LIFE_DISPLAY)
	{

		if (PlayerHandler::GetInstance()->GetLife() < 0) {

			CGame::GetInstance()->DrawTextInScene(L"CONTINUE", 100, 100, 400, 400);
			CGame::GetInstance()->DrawTextInScene(L"END", 100, 130, 400, 400);
			this->animation_set = CAnimationSets::GetInstance()->Get(61004);
			this->animation_set->at(0)->Render(1, 80, 110 + 30 * this->select_end);
		}
		else {
			wchar_t buffer[256];
			wsprintfW(buffer, L"LEFT %d", PlayerHandler::GetInstance()->GetLife());
			CGame::GetInstance()->DrawTextInScene(buffer, 100, 100, 400, 400);
		}
	}
	else {
		//LPDIRECT3DTEXTURE9 maptexture = CTextures::GetInstance()->Get(idStage);
		LPDIRECT3DTEXTURE9 maptexture = MapManager::GetIntance()->GetMapBackGround(idStage);
		if (player->GetPlayerType() == TAG_BIG_SOPHIA && idStage == ID_MAPOVERWORLD)
		{
			if (dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea())
			{
				if (textureAlpha == 255)
					textureAlpha = 140;
				else
					textureAlpha = 255;
			}
		}
		CGame::GetInstance()->Draw(1,-MapManager::GetIntance()->GetLeftAlign(idStage), 0, maptexture, 0, 0, mapWidth + MapManager::GetIntance()->GetLeftAlign(idStage) + MapManager::GetIntance()->GetRightAlign(idStage), mapHeight, textureAlpha);

		if (player->isAutoRun() && player->GetPlayerType() != TAG_BIG_SOPHIA)
		{
			HUD::GetInstance()->Render(player);
			return;
		}
		vector<LPGAMEENTITY> coObjects = CGrid::GetInstance()->GetListRenderObj(Camera::GetInstance()->GetRectCam());
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
			break;
		case EntityType::TAG_BIG_SOPHIA:
			player->Render();
			break;
		}
		HUD::GetInstance()->Render(player);

	}
}
void PlayScene::Unload()
{
	CGrid::GetInstance()->UnLoadGrid();
	//Sound::GetInstance()->UnLoadSound("BackgroundMusic");
	posX = posY = 0;
	if (!isReset)
		delete player;
	isUnloaded = true;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
	isReset = false;
}
