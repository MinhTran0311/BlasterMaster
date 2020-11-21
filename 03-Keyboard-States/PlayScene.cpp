#pragma once
#include "Worms.h"
#include "Insects.h"
#include "PlayScene.h"
#define ID_SMALL_SOPHIA	0
#define ID_JASON		1
#define ID_BIG_SOPHIA	2

#define MAP2_SIDE	200

#define OBJECT_TYPE_BRICK		1
#define OBJECT_TYPE_GATE		2

#define OBJECT_TYPE_WORM 10
#define OBJECT_TYPE_DOMES 11
#define OBJECT_TYPE_FLOATER 12
#define OBJECT_TYPE_INSECT 13


#define HUD_Y (SCREEN_HEIGHT/11) 


//PlayScene* PlayScene::GetInstance()
//{
//	if (__instance == NULL)
//		__instance = new PlayScene();
//	return __instance;
//}

PlayScene::PlayScene() : Scene()
{
	keyHandler = new PlayScenceKeyHandler(this);
	_SophiaType = ID_JASON;
	LoadBaseObjects();
	//CTextures::GetInstance()->Add(ID_AREA1, L"Resource\\level2-side.png", D3DCOLOR_XRGB(176, 224, 248));
	ChooseMap(ID_AREA1);
}

PlayScene::~PlayScene()
{
}

void PlayScene::SwitchScene(int scene_id)
{
}

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
	if (ssophia == NULL)
	{
		ssophia = new Small_Sophia(55, 100);
		DebugOut(L"[INFO] SMALL SOPHIA CREATED!!! \n");
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
	sceneFilePath = listSceneFilePath[(ID_AREA1%10)-1];			//chỉnh lại id
	LoadSceneObjects(sceneFilePath);
}

void PlayScene::PlayerGotGate()
{
	//for (UINT i = 0; i < listGates.size(); i++)
	//{
	//	if (listGates[i]->GetType() == EntityType::TAG_GATE)
	//	{
	//		if (jason->IsCollidingObject(listGates[i]))
	//		{
	//			Gate* gate = dynamic_cast<Gate*>(listGates[i]);
	//			_SophiaType = gate->typePlayer;
	//			int ID_map = gate->GetIdScene();
	//			float temp_x = gate->newPlayerx;
	//			float temp_y = gate->newPlayery;
	//			int tempState = gate->newPlayerState;
	//			tempNeed = gate->directionCam;
	//			
	//		}
	//	}
	//}
}

void PlayScene::Update(DWORD dt)
{
#pragma region camera
	float cx, cy;

	mapWidth = listWidth[(ID_AREA1 % 10) - 1];	
	mapHeight= listHeight[(ID_AREA1 % 10) - 1];


	switch (_SophiaType)
	{
	case ID_JASON:
		jason->GetPosition(cx, cy);
		if (jason->Getx() + SCREEN_WIDTH / 2 >= mapWidth)
			cx = mapWidth - SCREEN_WIDTH;
		else
		{
			if (jason->Getx() < SCREEN_WIDTH / 2)
				cx = 0;
			else
				cx -= SCREEN_WIDTH / 2;
		}
		break;
	case ID_SMALL_SOPHIA:
		ssophia->GetPosition(cx, cy);
		if (ssophia->Getx() + SCREEN_WIDTH / 2 >= mapWidth)
			cx = mapWidth - SCREEN_WIDTH;
		else
		{
			if (ssophia->Getx() < SCREEN_WIDTH / 2)
				cx = 0;
			else
				cx -= SCREEN_WIDTH / 2;
		}
		break;
	}

	
	cy -= SCREEN_HEIGHT / 2;
	gameCamera->SetCamPos(cx, 0.0f);//cy khi muon camera move theo y player 
	gameHUD->Update(cx, HUD_Y, jason->GetHealth(), jason->GetgunDam());	//move x follow camera
#pragma endregion
	//init coObjects
	vector<LPGAMEENTITY> coObjects;
	for (int i = 0; i < listObjects.size(); i++)
		coObjects.push_back(listObjects[i]);
	for (int i = 0; i < listEnemies.size(); i++)
		coObjects.push_back(listEnemies[i]);

	for (int i = 0; i < listEnemies.size(); i++)
		listEnemies[i]->Update(dt, &listObjects);
	//player

	jason->Update(dt,&coObjects);
	ssophia->Update(dt,&coObjects);
}

void PlayScene::Render()
{
	//idStage / STAGE_1 + 10
	
	LPDIRECT3DTEXTURE9 maptextures = CTextures::GetInstance()->Get(ID_AREA1);
	CGame::GetInstance()->OldDraw(0, 0, maptextures, 0, 0, mapWidth, mapHeight);

	for (int i = 0; i < listObjects.size(); i++)
	{
		listObjects[i]->Render();
	}
	for (int i = 0; i < listEnemies.size(); i++)
		listEnemies[i]->Render();
	switch (_SophiaType)
	{
	case ID_JASON:
		jason->Render();
		break;
	case ID_SMALL_SOPHIA:
		jason->Render();
		ssophia->Render();

	}
	gameHUD->Render(jason);
}

void PlayScene::Unload()
{
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

	JASON* jason = ((PlayScene*)scence)->jason;
	Small_Sophia* ssophia = ((PlayScene*)scence)->ssophia;

	if (jason->GetState() == SOPHIA_STATE_DIE) return;
	
	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
	{
		if (_SophiaType == ID_JASON)
			jason->SetState(SOPHIA_STATE_WALKING_RIGHT);
		if (_SophiaType == ID_SMALL_SOPHIA)
			ssophia->SetState(SMALL_SOPHIA_STATE_WALKING_RIGHT);

	}
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
	{
		if (_SophiaType == ID_JASON) {
			jason->SetState(SOPHIA_STATE_WALKING_LEFT);
		}
		if (_SophiaType == ID_SMALL_SOPHIA) {
			ssophia->SetState(SMALL_SOPHIA_STATE_WALKING_LEFT);
		}

	}
	else
	{
		if (_SophiaType == ID_JASON)
			jason->SetState(SOPHIA_STATE_IDLE);
		if (_SophiaType == ID_SMALL_SOPHIA)
			ssophia->SetState(SMALL_SOPHIA_STATE_IDLE);

	}

	if (CGame::GetInstance()->IsKeyDown(DIK_SPACE))
	{
		if (_SophiaType == ID_JASON)
			jason->SetPressSpace(true);
		if (_SophiaType == ID_SMALL_SOPHIA)
		{
			ssophia->SetPressSpace(true);
		}
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


}

void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	int _SophiaType = ((PlayScene*)scence)->_SophiaType;
	Entity* ssophia = ((PlayScene*)scence)->ssophia;
	JASON* jason = ((PlayScene*)scence)->jason;
	vector<LPGAMEENTITY> listEnemies = ((PlayScene*)scence)->listEnemies;
	float x, y;
	int direction, directionY, isTargetTop, dame;
	switch (KeyCode)
	{
	case DIK_ESCAPE:
		DestroyWindow(CGame::GetInstance()->GetWindowHandle());
		break;
	case DIK_SPACE:
		switch (_SophiaType)
		{
		case ID_JASON:
			jason->SetState(SOPHIA_STATE_JUMP);
			break;
		case ID_SMALL_SOPHIA:
			ssophia->SetState(SMALL_SOPHIA_STATE_JUMP);
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

	default:
		break;
	}

}

void PlayScene::changePlayer()
{
	//if (abs(jason->x - ssophia->x)< DISTANCE_TO_OUT&& abs(jason->y - ssophia->y) < DISTANCE_TO_OUT)
	//{
		if (_SophiaType == ID_JASON)
		{
			this->jason->SetState(SOPHIA_STATE_OUT);
			this->ssophia->x = jason->x;
			this->ssophia->y = jason->y;
			this->_SophiaType = ID_SMALL_SOPHIA;
			this->ssophia->SetState(SMALL_SOPHIA_STATE_OUT);
		}

		else if (_SophiaType == ID_SMALL_SOPHIA)
		{
			if (abs(jason->x - ssophia->x) < DISTANCE_TO_OUT && abs(jason->y - ssophia->y) < DISTANCE_TO_OUT)
			{
				this->ssophia->SetState(SMALL_SOPHIA_STATE_IDLE);
				this->ssophia->SetState(SMALL_SOPHIA_STATE_OUT);
				this->_SophiaType = ID_JASON;
				
			}
			
		}
	//}
	
	//this->_SophiaType = ID_SMALL_SOPHIA;
}

void PlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	JASON* jason = ((PlayScene*)scence)->jason;
	Small_Sophia* ssophia = ((PlayScene*)scence)->ssophia;
	PlayScene* playScene = dynamic_cast<PlayScene*>(scence);
	switch (KeyCode)
	{
	case DIK_UP:
		switch (((PlayScene*)scence)->_SophiaType)
		{
		case ID_JASON:
			jason->SetPressUp(false);
			jason->SetState(SOPHIA_STATE_GUN_UNFLIP);
			break;

		}
		
		break;
	case DIK_SPACE:
		switch (((PlayScene*)scence)->_SophiaType)
		{
			case ID_JASON:
				jason->SetPressSpace(false);
				break;
			case ID_SMALL_SOPHIA:
				ssophia->SetPressSpace(false);
				break;
		}
		
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
}

void PlayScene::_ParseSection_OBJECTS(string line)
{
	//chưa done
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	Entity* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_WORM:		
	{
		obj = new Worm(x, y, jason);
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		listEnemies.push_back(obj);
		DebugOut(L"[test] add worm !\n");
		break;
	}
	case OBJECT_TYPE_FLOATER:
	{
		obj = new Floaters(x, y, jason);
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		listEnemies.push_back(obj);
		DebugOut(L"[test] add floater !\n");
		break;
	}

	case OBJECT_TYPE_INSECT:
	{
		obj = new Insects(x, y, jason);
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		listEnemies.push_back(obj);
		DebugOut(L"[test] add insect !\n");
		break;
	}



	case OBJECT_TYPE_BRICK:
	{
		obj = new Brick(atof(tokens[4].c_str()), atof(tokens[5].c_str()));
		obj->SetPosition(x, y);
		//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		//obj->SetAnimationSet(ani_set);
		listObjects.push_back(obj);
		DebugOut(L"[test] add brick !\n");
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		int switchId = atoi(tokens[3].c_str());
		float playerPosX = atoi(tokens[4].c_str());
		float playerPosY = atoi(tokens[5].c_str());
		int playerState = atoi(tokens[6].c_str());
		int isResetCamera = atoi(tokens[7].c_str());
		int typePlayer = atoi(tokens[8].c_str());
		float camX = atoi(tokens[9].c_str());
		int camY = atoi(tokens[10].c_str());
		obj = new Gate(x, y, switchId, playerPosX, playerPosY, playerState, isResetCamera, typePlayer, camX, camY);
		listObjects.push_back(obj);
		DebugOut(L"[test] add gate !\n");
		break;
	}
	default:
		DebugOut(L"[ERRO] Invalid object type: %d\n", object_type);
		return;
	}
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
