#pragma once
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


#define MAP2_SIDE	200

#define HUD_Y (SCREEN_HEIGHT/2.5)

PlayScene::PlayScene()
{
}

PlayScene::PlayScene(int _idStage) : Scene()
{
	idStage = _idStage;
	keyHandler = new PlayScenceKeyHandler(this);
	LoadBaseObjects();
	ChooseMap(idStage);
	Sound::GetInstance()->LoadSoundResource(SOUND_RESOURCE_UNDERWORLD);
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
		playerInfo.jasonStage = ID_AREA1;

	}

	/*if (gameHUD == NULL)
	{
		gameHUD = new HUD(player->GetHealth(), player->GetgunDam());
		DebugOut(L"[INFO] HUD CREATED! %d \n", player->GetHealth());
	}*/
	HUD::GetInstance()->HUDInit(player->GetHealth(), player->GetgunDam());
#pragma endregion
	//Sound::GetInstance()->LoadSound("Resource\\Sound\\01Opening.wav", "BackgroundMusic");

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
	sceneFilePath = listSceneFilePath[idStage-11];
	DebugOut(L"Init");
	CGrid::GetInstance()->InitGrid(listWidth[idStage - 11], listHeight[idStage - 11]);
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
	if (player->GetPlayerType()==TAG_BIG_SOPHIA)
		if (dynamic_cast<Big_Sophia*>(player)->isAutoRun())
		{
			return;
		}
	if (player->GetPlayerType() != TAG_SMALL_SOPHIA)
	{
		if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
		{
			player->SetState(SOPHIA_STATE_WALKING_RIGHT);

		}
		else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
		{
			player->SetState(SOPHIA_STATE_WALKING_LEFT);
		}
		else
		{
			player->SetState(SOPHIA_STATE_IDLE);
		}

		if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
		{
			if (player->GetPlayerType() == EntityType::TAG_JASON)
				dynamic_cast<JASON*>(player)->SetPressSpace(true);
		}

		if (CGame::GetInstance()->IsKeyDown(DIK_UP))
		{
			if (player->GetPlayerType() == TAG_JASON)
				dynamic_cast<JASON*>(player)->SetPressUp(true);
			else if (player->GetPlayerType() == TAG_BIG_SOPHIA)
				dynamic_cast<Big_Sophia*>(player)->SetState(BIG_SOPHIA_STATE_WALKING_TOP);
			//else if (player->GetPlayerType() == TAG_SMALL_SOPHIA && dynamic_cast<Small_Sophia*>(player)->IsInStairs())
			//{
			//	dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CLIMB_UP);
			//}
		}
		if ((CGame::GetInstance()->IsKeyDown(DIK_DOWN)))
		{
			if (player->GetPlayerType() == TAG_BIG_SOPHIA)
				dynamic_cast<Big_Sophia*>(player)->SetState(BIG_SOPHIA_STATE_WALKING_BOTTOM);
			//else if (player->GetPlayerType() == TAG_SMALL_SOPHIA && dynamic_cast<Small_Sophia*>(player)->IsInStairs())
			//{
			//	dynamic_cast<Small_Sophia*>(player)->SetState(SMALL_SOPHIA_STATE_CLIMB_DOWN);
			//}
		}
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
				player->SetState(SOPHIA_STATE_WALKING_RIGHT);
			}
			else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
			{
				player->SetState(SOPHIA_STATE_WALKING_LEFT);
			}
			else
				player->SetState(SOPHIA_STATE_IDLE);
		}
	}
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	if (playScene->death == true)
	{
		if (playScene->playerInfo.life < 0) {
			if(KeyCode == DIK_DOWN)playScene->select_end = true;
			if(KeyCode == DIK_UP)playScene->select_end = false;
			
		}
	}
	
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
			//player->SetPressUp(false);
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
			//single fire
			player->FireBullet(1);
			break;
		}

		case DIK_X:
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_UPGRADE_BULLET, player->Getx(), player->Gety(), 3) && player->GetPlayerType()!=TAG_BIG_SOPHIA)
			{
				Bullet* bullet = new JasonBullet(player->Getx(), player->Gety(), 1, nx, isAimingTop);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			break;
		}
		case DIK_V:
		{
			//burst fire
			if (player->GetPlayerType() == TAG_JASON)
				player->FireBullet(2);
			break;
		}
		case DIK_C:
		{
			if (player->GetPlayerType()==TAG_JASON)
				player->FireBullet(3);
			break;
		}
		}
	}
}

void PlayScene::changePlayer()
{

	if (player->GetPlayerType() == EntityType::TAG_JASON && !dynamic_cast<JASON*>(player)->IsGunFlip() && !dynamic_cast<JASON*>(player)->IsJumping())
	{
		playerInfo.jasonHealth = player->GetHealth();
		playerInfo.jasonGundam = player->GetgunDam();
		playerInfo.jasonStage = idStage;
		playerInfo.jasonXPos = player->Getx();
		playerInfo.jasonYPos = player->Gety();
		playerInfo.sophiaStage = idStage;

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

void PlayScene::CheckEnterBoss()
{
	if (player->GetPlayerType() == TAG_BIG_SOPHIA && player->Getx() > 60 && player->Getx() < 80 && player->Gety() > 1844 && player->Gety() < 1880 && !dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea() && idStage== ID_MAPOVERWORLD)
	{
		dynamic_cast<Big_Sophia*>(player)->SetIsEnterIntroBossArea(true);
		BossIntroTimer->Start();
	}
}

void PlayScene::SetUpFightBoss()
{
	dynamic_cast<Big_Sophia*>(player)->SetIsEnterIntroBossArea(false);
	player->SetPosition(1790, 1130);
	Camera::GetInstance()->SetCamPos(1607, 995);
	Camera::GetInstance()->SetIsFollowPlayer(false);
}

void PlayScene::BossAreaController()
{
	//intro
	if (!dynamic_cast<Big_Sophia*>(player)->IsEnterIntroBossArea() && !dynamic_cast<Big_Sophia*>(player)->IsFightWithBoss())
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
		dynamic_cast<Big_Sophia*>(player)->SetIsFightWithBoss(false);
		player->SetPosition(880, 656);
		Camera::GetInstance()->SetCamPos(767, 479);
		BossIntroTimer->Start();
	}
	// back to return gate
	if (BossIntroTimer->IsTimeUp() && dynamic_cast<Big_Sophia*>(player)->IsDoneFightWithBoss())
	{
		player->SetPosition(159, 1889);
		Camera::GetInstance()->SetCamPos(0, 1774);
		BossIntroTimer->Reset();
		dynamic_cast<Big_Sophia*>(player)->SetIsDoneFightWithBoss(false);
	}
}

void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//JASON* jason = ((PlayScene*)scence)->jason;
	//Small_Sophia* ssophia = ((PlayScene*)scence)->ssophia;

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

void PlayScene::CheckPlayerReachGate()
{
	if (player->GetGateColliding())
	{
		switch (player->GetPlayerType())
		{
		case TAG_JASON:
		{
			Gate* gate = dynamic_cast<Gate*>(player->GetGate());
			if (gate->GetIdScene() != ID_MAPOVERWORLD)
			{
				DebugOut(L"[Info] success\n");
				playerInfo.jasonStage = gate->GetIdScene();
				playerInfo.jasonXPos = gate->GetNewPlayerX();
				playerInfo.jasonYPos = gate->GetNewPlayerY();
				int tempState = gate->GetNewPlayerState();
				//isNeedResetCamera = gate->directionCam;

				camMap1X = gate->GetNewCamXPos();
				camMap1Y = gate->GetNewCamYPos();
				isNeedResetCamera = true;
				DebugOut(L"camposX: %d, camposY: %d\n", camMap1X, camMap1Y);
				DebugOut(L"posX: %f, posY: %f\n", playerInfo.jasonXPos, playerInfo.jasonYPos);


				playerInfo.jasonGundam = player->GetgunDam();
				playerInfo.jasonHealth = player->GetHealth();
				playerInfo.playerDirectionBeforePassGate = player->GetDirection();

				Unload();

				ChooseMap(playerInfo.jasonStage);

				Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());

				player = new JASON(playerInfo.jasonXPos, playerInfo.jasonYPos, playerInfo.jasonHealth, playerInfo.jasonGundam);
				player->SetDirection(playerInfo.playerDirectionBeforePassGate);
				player->SetState(tempState);
				//CGrid::GetInstance()->SetTargetForEnemies(player);
			}
			break;
		}
		case TAG_SMALL_SOPHIA:
		{
			Gate* gate = dynamic_cast<Gate*>(player->GetGate());

			DebugOut(L"[Info] samll gate success\n");
			playerInfo.sophiaStage = gate->GetIdScene();
			playerInfo.sophiaXPos = gate->GetNewPlayerX();
			playerInfo.sophiaYPos = gate->GetNewPlayerY();
			int tempState = gate->GetNewPlayerState();
			//isNeedResetCamera = gate->directionCam;
			camMap1X = gate->GetNewCamXPos();
			camMap1Y = gate->GetNewCamYPos();
			isNeedResetCamera = true;
			DebugOut(L"camposX: %d, camposY: %d\n", camMap1X, camMap1Y);
			DebugOut(L"posX: %d, posY: %d\n", playerInfo.sophiaXPos, playerInfo.sophiaYPos);

			playerInfo.sophiaGundam = player->GetgunDam();
			playerInfo.sophiaHealth = player->GetHealth();
			playerInfo.playerDirectionBeforePassGate = player->GetDirection();

			Unload();

			//DebugOut(L"type %d\n", gate->typePlayer);
			ChooseMap(playerInfo.sophiaStage);

			Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());

			switch (playerInfo.sophiaStage)
			{
			case ID_MAPOVERWORLD:
			{
				player = new Big_Sophia(playerInfo.sophiaXPos, playerInfo.sophiaYPos, playerInfo.sophiaHealth, playerInfo.sophiaGundam);

				break;
			}
			default:
			{
				player = new Small_Sophia(playerInfo.sophiaXPos, playerInfo.sophiaYPos + 2.0f, playerInfo.sophiaHealth, playerInfo.sophiaGundam);
				player->SetDirection(playerInfo.playerDirectionBeforePassGate);
				player->SetState(tempState);
				if (playerInfo.sophiaStage == playerInfo.jasonStage)
				{
					backup_player = new JASON(playerInfo.jasonXPos, playerInfo.jasonYPos, playerInfo.jasonHealth, playerInfo.jasonGundam);
				}
				break;
			}
			}
			//CGrid::GetInstance()->SetTargetForEnemies(player);

			break;
		}
		case TAG_BIG_SOPHIA:
		{
			if (dynamic_cast<GateOverworld*>(player->GetGate()))
			{
				GateOverworld* gate = dynamic_cast<GateOverworld*>(player->GetGate());
				dynamic_cast<Big_Sophia*>(player)->AutoRun(gate->directionCam);
				xPosCamGo = gate->GetXPosGo();
				xPosCamBack = gate->GetXPosBack();
				yPosCamGo = gate->GetYPosGo();
				yPosCamBack = gate->GetYPosBack();
				CamMoveDirection = gate->GetCamDirection();
				Camera::GetInstance()->SetIsFollowPlayer(false);
			}
			else
			{
				CamMoveDirection = -1;

				Gate* gate = dynamic_cast<Gate*>(player->GetGate());

				DebugOut(L"[Info] return gate success\n");
				playerInfo.sophiaStage = gate->GetIdScene();
				playerInfo.sophiaXPos = gate->GetNewPlayerX();
				playerInfo.sophiaYPos = gate->GetNewPlayerY();
				//isNeedResetCamera = gate->directionCam;
				camMap1X = gate->GetNewCamXPos();
				camMap1Y = gate->GetNewCamYPos();
				playerInfo.sophiaGundam = player->GetgunDam();
				playerInfo.sophiaHealth = player->GetHealth();
				isNeedResetCamera = true;
				Unload();
				ChooseMap(playerInfo.sophiaStage);

				Camera::GetInstance()->SetIsFollowPlayer(gate->IsCamFollowPlayer());

				player = new Small_Sophia(playerInfo.sophiaXPos, playerInfo.sophiaYPos, playerInfo.sophiaHealth, playerInfo.sophiaGundam);
				backup_player = new JASON(playerInfo.jasonXPos, playerInfo.jasonYPos, playerInfo.jasonHealth, playerInfo.jasonGundam);
				//CGrid::GetInstance()->SetTargetForEnemies(player);
			}
			break;
		}
		}
	}
}

void PlayScene::Update(DWORD dt)
{
	DebugOut(L"x: %f, y: %f\n", player->Getx(), player->Gety());
	if (this->inforDisplay == CHOOSING_WEAPON_DISPLAY &&  player->GetPlayerType()==TAG_JASON)
	{
		SceneManager::GetInstance()->SetHolderScene(SceneManager::GetInstance()->GetScene());
		/*if (SceneManager::GetInstance()->GetHolderScene() == nullptr)*/
		DebugOut(L"khong null: %d\n", SceneManager::GetInstance()->GetHolderScene());
		inforDisplay = 0;
		SceneManager::GetInstance()->SetScene(new ChooseWeaponScene(dynamic_cast<JASON*>(player)));

	}
	else if (this->inforDisplay == LIFE_DISPLAY)
	{

	}
	else
#pragma region mạng và reset
	{
		/*SetKeyhandler(this);*/
		if (player->IsDoneDeath())
		{
			if (playerInfo.life < 0)
			{
				//ending
				this->death = true;
			}
			else
			{
				isReset = true;
				Unload();

				//hiển thị số mạng còn dựa vào playerInfo.life
				this->death = true;

				if (player->GetPlayerType() == TAG_JASON)
				{
					ChooseMap(playerInfo.jasonStage);
					player->Reset(playerInfo.jasonHealth, playerInfo.jasonGundam);
				}
				else
				{
					ChooseMap(playerInfo.sophiaStage);
					player->Reset(playerInfo.sophiaHealth, playerInfo.sophiaGundam);

				}
				isNeedResetCamera = true;
			}
		}
	}
	Sound::GetInstance()->Play("MusicMap", 1, 10000);

#pragma endregion
	if (player->GetPlayerType() == TAG_BIG_SOPHIA && idStage == ID_MAPOVERWORLD)
	{
		BossAreaController();
	}

	CheckPlayerReachGate();
	
#pragma region camera
	float cx, cy;
	mapWidth = listWidth[idStage - 11];
	mapHeight = listHeight[idStage - 11];
	player->GetPosition(cx, cy);
	if (isNeedResetCamera)
	{
		DebugOut(L"middle\n");
		Camera::GetInstance()->SetCamPos(camMap1X, camMap1Y);
		//DebugOut(L"y: %d \n",camMap1Y);
		//posY = camMap1Y;
		isNeedResetCamera = false;
	}
	else
	{
		if (!player->IsDoneDeath())
		{
			Camera::GetInstance()->Update(cx, cy, player->GetPlayerType(), dt, listWidth[idStage - 11], listHeight[idStage - 11], player->GetDirection(), player->GetDirctionY(), xPosCamGo, xPosCamBack, yPosCamGo, yPosCamBack, CamMoveDirection);

		}
	}
#pragma endregion

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
	{//update obj
		for (int i = 0; i < coObjects.size(); i++)
		{
			if (coObjects.at(i)->GetType() != EntityType::TAG_BRICK && coObjects.at(i)->GetType() != TAG_GATE && coObjects.at(i)->GetType() != TAG_GATE_OVERWORLD)
			{

				coObjects[i]->Update(dt, &coObjects);
			}
		}
		//sua cho nay
		int k = 0;
		for (int i = 0; i < coObjects.size() - k; i++)
		{
			if ((coObjects.at(i)->IsDeath()))
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
				CGrid::GetInstance()->RemoveObj(backup, true);
				k = 1;
				i--;
			}
			//item effect
			else {
				k = 0;
			}
		}
		

	CGrid::GetInstance()->UpdateGrid(coObjects);
	//player

	HUD::GetInstance()->Update(Camera::GetInstance()->GetCamx()+20, HUD_Y + Camera::GetInstance()->GetCamy(), player->GetHealth(), player->GetgunDam(), player->GetPlayerType());
	}
#pragma endregion

}

void PlayScene::Render()
{
	if (this->death==true)
	{
		if (this->playerInfo.life < 0) {
			
			CGame::GetInstance()->DrawTextInScene(L"CONTINUE", 100, 100, 400, 400);
			CGame::GetInstance()->DrawTextInScene(L"END", 100, 130, 400, 400);
			this->animation_set = CAnimationSets::GetInstance()->Get(61000);
			this->animation_set->at(0)->Render(1, 80, 115 + 30*this->select_end);
			this->time_drawlife++;
			if (this->time_drawlife == 20) { this->death = false; this->time_drawlife = 0;  this->playerInfo.life--; this->player->health = 8; }
		}
		else {
			wchar_t buffer[256];
			wsprintfW(buffer, L"LEFT %d", this->playerInfo.life);
			CGame::GetInstance()->DrawTextInScene(buffer, 100, 100, 400, 400);
			this->time_drawlife++;
			if (this->time_drawlife == 20) { this->death = false; this->time_drawlife = 0;  this->playerInfo.life--; this->player->health = 1; }
		}
		//LPCWSTR Life = L"LEFT %d" + this->playerInfo.life;
		
	}
	else {
		LPDIRECT3DTEXTURE9 maptexture = CTextures::GetInstance()->Get(idStage);
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
		CGame::GetInstance()->OldDraw(0, 0, maptexture, 0, 0, mapWidth, mapHeight, textureAlpha);

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
