#pragma once
#include "Worms.h"
#include "PlayScene.h"
#include "JasonRocket.h"
#include "JasonBullet.h"
#include "Grid.h"

#define ID_SMALL_SOPHIA	0
#define ID_JASON		1
#define ID_BIG_SOPHIA	2

#define MAP2_SIDE	200


#define HUD_Y (SCREEN_HEIGHT/11) 

PlayScene::PlayScene(int _idStage) : Scene()
{
	idStage = _idStage;
	keyHandler = new PlayScenceKeyHandler(this);
	_SophiaType = ID_JASON;
	LoadBaseObjects();
	ChooseMap(idStage);
	
}



PlayScene::~PlayScene()
{
}

//void PlayScene::SwitchScene(int scene_id)
//{
//	DebugOut(L"dung cong");
//}

void PlayScene::LoadBaseObjects()
{

	texturesFilePath = ToLPCWSTR("Resource\\SceneAndSpec\\base_playscene.txt");
	LoadBaseTextures();
#pragma region create_base_objects
	if (jason == NULL)
	{
		jason = new JASON(55, 100);
		DebugOut(L"[INFO] JASON CREATED!!! \n");
	}
	if (gameHUD == NULL)
	{
		gameHUD = new HUD(jason->GetHealth(), jason->GetgunDam());
		DebugOut(L"[INFO] HUD CREATED! %d \n", jason->GetHealth());
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
	sceneFilePath = listSceneFilePath[(Stage%10)-1];			//chỉnh lại id
	CGrid::GetInstance()->InitGrid(listWidth[(idStage % 10) - 1], listHeight[(idStage % 10) - 1]);
	LoadSceneObjects(sceneFilePath);
}

void PlayScene::CheckPlayerReachGate()
{
	if (jason->GetGateColliding())
	{
		Gate* gate = jason->GetGate();
		DebugOut(L"[Info] success\n");
		_SophiaType = gate->typePlayer;
		int tempMap = gate->GetIdScene();
		float tempx = gate->newPlayerx;
		float tempy = gate->newPlayery;
		int tempState = gate->newPlayerState;
		tempNeed = gate->directionCam;
		camMap1X = gate->camPosX;
		camMap1Y = gate->camPosY;
		Unload();

		ChooseMap(tempMap);
		jason->SetGateColliding(false);
		jason->ResetGate();
		jason->SetPosition(tempx, tempy);
		jason->Setvx(0);
		jason->Setvy(0);
		jason->SetState(tempState);
	}
}
void PlayScene::Update(DWORD dt)
{
#pragma region sceneswitching
	//CheckPlayerReachGate();
#pragma endregion
	//EraseInactiveObject();

#pragma region camera
	float cx, cy;
	jason->GetPosition(cx, cy);
	mapWidth = listWidth[(idStage% 10) - 1];	
	mapHeight= listHeight[(idStage % 10) - 1];

	//if (jason->Getx() + SCREEN_WIDTH / 2 >= mapWidth)
	//	cx = mapWidth - SCREEN_WIDTH;
	//else
	//{
	//	if (jason->Getx() < SCREEN_WIDTH / 2)
	//		cx = 0;
	//	else
	//		cx -= SCREEN_WIDTH / 2;
	//}
	//cy -= SCREEN_HEIGHT / 2;
	//gameCamera->SetCamPos(cx, cy);//cy khi muon camera move theo y player 
	jason->GetPosition(cx, cy);
	if (tempNeed)
	{
		//timeResetCam += dt;
		gameCamera->SetCamPos(camMap1X, camMap1Y);
		posY = camMap1Y;
		//if (timeResetCam > 1000)
		tempNeed = 0;
	}
	else
	{
		if (jason->Getx() + SCREEN_WIDTH / 2 >= mapWidth)
			cx = mapWidth - SCREEN_WIDTH;
		else
		{
			if (jason->Getx() < SCREEN_WIDTH / 2)
				cx = 0;
			else
				cx -= SCREEN_WIDTH / 2;
		}

		if (cy + SCREEN_HEIGHT >= mapHeight)
		{

			cy = mapHeight - SCREEN_HEIGHT;
			posY = cy;
		}
		else
		{
			if (jason->Gety() < SCREEN_HEIGHT / 3)
			{
				posY = 0;
			}
			else
			{
				//DebugOut(L"cy - posY %f \n", cy - posY);
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
		//DebugOut(L"toa do cam x %f \n ", cx);
		//DebugOut(L"toa do cam y %f \n ", posY);
		gameCamera->SetCamPos(cx, posY);
	}
		//move x follow camera
#pragma endregion

	
	//init coObjects
	vector<LPGAMEENTITY> coObjects = CGrid::GetInstance()->GetListUpdateObj();
	jason->Update(dt, &coObjects);
	//for (int i = 0; i < listObjects.size(); i++)
	//	coObjects.push_back(listObjects[i]);
	//for (int i = 0; i < listEnemies.size(); i++)
	//	coObjects.push_back(listEnemies[i]);	
	//for (int i = 0; i < listGates.size(); i++)
	//	coObjects.push_back(listGates[i]);

	//Kiểm tra có obj nào cần được thêm vào hay không
	//vector<CEntity*> addAfterUpdate = jason->GetAbjAddAfterUpdate();
	//for (int i = 0; i < addAfterUpdate.size(); i++)
	//{
	//	CGrid::GetInstance()->InsertGrid(addAfterUpdate.at(i));
	//	coObjects.push_back(addAfterUpdate.at(i));
	//}
	//for (int i = 0; i < listEnemies.size(); i++)
	//{
	//	listEnemies[i]->Update(dt, &listObjects);
	//}
	//for (int i = 0; i < listBullets.size(); i++)
	//	listBullets[i]->Update(dt, &coObjects);

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
		for (int i = coObjects.size()-1; i>=0; i--)
		{
			if ((coObjects.at(i)->isDeath() && coObjects.at(i)->GetType() != EntityType::ITEM && coObjects.at(i)->GetType() != EntityType::TAG_BRICK && coObjects.at(i)->GetType() != EntityType::TAG_GATE))
			{
				float xPos, yPos;
				coObjects.at(i)->GetPosition(xPos, yPos);
				Entity* backup = coObjects.at(i);

				coObjects.erase(coObjects.end() - i);
				float _xtemp, _ytemp;
				backup->GetPosition(_xtemp, _ytemp);
#pragma region add item into grid
				switch (backup->GetType())
				{
				default:
					break;
				}
#pragma endregion
				if (backup != nullptr)
					DebugOut(L"khasc nulll %d", coObjects.at(i)->GetType());
				CGrid::GetInstance()->RemoveObj(backup,true);
				k++;
			}
			//item effect
			else { }
		}
	}
	CGrid::GetInstance()->UpdateGrid(coObjects);
	//player
	gameHUD->Update(cx, HUD_Y + posY, jason->GetHealth(), jason->GetgunDam());
}

void PlayScene::Render()
{
	//idStage / STAGE_1 + 10
	
	LPDIRECT3DTEXTURE9 maptextures = CTextures::GetInstance()->Get(idStage);
	CGame::GetInstance()->OldDraw(0, 0, maptextures, 0, 0, mapWidth, mapHeight);
	vector<LPGAMEENTITY> coObjects = CGrid::GetInstance()->GetListRenderObj();
	for (int i = 0; i < coObjects.size(); i++)
		coObjects[i]->Render();
	//for (int i = 0; i < listObjects.size(); i++)
	//{
	//	listObjects[i]->Render();
	//}
	//for (int i = 0; i < listGates.size(); i++)
	//	listGates[i]->Render();
	//for (int i = 0; i < listEnemies.size(); i++)
	//	listEnemies[i]->Render();
	//for (int i = 0; i < listBullets.size(); i++)
	//	listBullets[i]->Render();
	jason->Render();
	gameHUD->Render(jason);

}

void PlayScene::Unload()
{
	CGrid::GetInstance()->UnLoadGrid();
	/*for (UINT i = 0; i < listObjects.size(); i++)
		delete listObjects[i];
	listObjects.clear();
	for (UINT i = 0; i < listGates.size(); i++)
		delete listGates[i];
	listGates.clear();
	for (UINT i = 0; i < listEnemies.size(); i++)
		delete listEnemies[i];
	listEnemies.clear();*/
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
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

void PlayScenceKeyHandler::KeyState(BYTE* states)
{
	int _SophiaType = ((PlayScene*)scence)->_SophiaType;
	JASON* player = ((PlayScene*)scence)->jason;
	//vector<LPGAMEENTITY> listEnemies = ((PlayScene*)scence)->listEnemies;
	if (player->GetState() == SOPHIA_STATE_DIE) return;
	
	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
	{
		if (_SophiaType == ID_JASON)
			player->SetState(SOPHIA_STATE_WALKING_RIGHT);
	}
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
	{
		if (_SophiaType == ID_JASON) {
			player->SetState(SOPHIA_STATE_WALKING_LEFT);
		}
	}
	else
	{
		if (_SophiaType == ID_JASON)
			player->SetState(SOPHIA_STATE_IDLE);
	}

	if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
	{
		if (_SophiaType == ID_JASON)
			player->SetPressSpace(true);
	}

	if (CGame::GetInstance()->IsKeyDown(DIK_UP))
	{
		if (_SophiaType == ID_JASON)
			player->SetPressUp(true);
	}
}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	int _SophiaType = ((PlayScene*)scence)->_SophiaType;
	JASON* player = ((PlayScene*)scence)->jason;
	//vector<LPGAMEENTITY> listEnemies = ((PlayScene*)scence)->listEnemies;
	//vector<LPBULLET> listBullets = ((PlayScene*)scence)->listBullets;
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	float xPos, yPos;
	bool isAimingTop;
	int nx, ny, dam;
	player->GetInfoForBullet(nx, isAimingTop, xPos, yPos);
	switch (KeyCode)
	{
	case DIK_ESCAPE:
		DestroyWindow(CGame::GetInstance()->GetWindowHandle());
		break;
	case DIK_SPACE:
		if (_SophiaType == ID_JASON)
			player->SetState(SOPHIA_STATE_JUMP);
		break;
	case DIK_A:
	{
		playScene->Unload();
		playScene->ChooseMap(ID_AREA1);
		player->SetPosition(30, 60);
		player->SetHealth(MAX_HEALTH);
		player->isDoneDeath = false;
		player->isDeath = false;
		playScene->_SophiaType = 1;
		break;
	}
	//case DIK_Z:
	//	//if (listBullets.size() < 3)
	//	//{
	//	//	Bullet* bullet = new JasonBullet(player->Getx(), player->Gety(), 0, nx, isAimingTop);
	//	//	((PlayScene*)scence)->listBullets.push_back(bullet);
	//	//}
	//	//break;

	//case DIK_X:
	//	//if (listBullets.size() < 3)
	//	//{
	//	//	Bullet* bullet = new JasonBullet(player->Getx(), player->Gety(), 1, nx, isAimingTop);
	//	//	((PlayScene*)scence)->listBullets.push_back(bullet);
	//	//}
	//	//break;
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

void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	JASON* player = ((PlayScene*)scence)->jason;
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	switch (KeyCode)
	{
	case DIK_UP:
		player->SetPressUp(false);
		player->SetState(SOPHIA_STATE_GUN_UNFLIP);
		break;
	case DIK_SPACE:
		player->SetPressSpace(false);
		break;
	}
} 

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
	//chưa done
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
//#pragma region oldcode
//
//
//
//	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
//
//	int object_type = atoi(tokens[0].c_str());
//	float x = atof(tokens[1].c_str());
//	float y = atof(tokens[2].c_str());
//	int ani_set_id = atoi(tokens[3].c_str());
//	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
//
//	Entity* obj = NULL;
//
//	switch (object_type)
//	{
//	case EntityType::TAG_WORM:		
//	{
//		obj = new Worm(x, y, jason);
//		//obj->SetPosition(x, y);
//		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//
//		obj->SetAnimationSet(ani_set);
//		listEnemies.push_back(obj);
//		DebugOut(L"[test] add worm !\n");
//		break;
//	}
//	case EntityType::TAG_BRICK:
//	{
//		obj = new Brick(atof(tokens[4].c_str()), atof(tokens[5].c_str()));
//		obj->SetPosition(x, y);
//		//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//
//		//obj->SetAnimationSet(ani_set);
//		listObjects.push_back(obj);
//		DebugOut(L"[test] add brick !\n");
//		break;
//	}
//	case EntityType::TAG_GATE:
//	{
//		int switchId = atoi(tokens[3].c_str());
//		float playerPosX = atoi(tokens[4].c_str());
//		float playerPosY = atoi(tokens[5].c_str());
//		int playerState = atoi(tokens[6].c_str());
//		int isResetCamera = atoi(tokens[7].c_str());
//		int typePlayer = atoi(tokens[8].c_str());
//		float camX = atoi(tokens[9].c_str());
//		int camY = atoi(tokens[10].c_str());
//		obj = new Gate(x, y, switchId, playerPosX, playerPosY, playerState, isResetCamera, typePlayer, camX, camY);
//		listGates.push_back(obj);
//		DebugOut(L"[test] add gate !\n");
//		break;
//	}
//	default:
//		DebugOut(L"[ERRO] Invalid object type: %d\n", object_type);
//		return;
//	}
//#pragma endregion
	CGrid::GetInstance()->LoadGrid(tokens);
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
void PlayScene::EraseInactiveObject()
{
	//int pos=-1;
	//for (int i = 0; i < listBullets.size(); i++)
	//{
	//	if (listBullets[i]->GetisActive() == false)
	//	{
	//		Bullet* p = listBullets[i];
	//		pos = i;
	//	}
	//}
	//if(pos!=-1)	listBullets.erase(listBullets.begin() + pos);
}