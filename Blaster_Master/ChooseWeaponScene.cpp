#include "ChooseWeaponScene.h"

ChooseWeaponScene::ChooseWeaponScene() : Scene()
{
	//player = _player;
	//weaponId = player->GetCurrentSpecialWeapon()-33;	//0: homing, 1: electric, 2: rocket
	//player->GetNoOfBulletLeft(noRocket, noHomingMissles, noElectric);	//laasy so luong dan
	weaponId = PlayerHandler::GetInstance()->GetSpecialWeaponId();
	PlayerHandler::GetInstance()->GetNoOfBulletLeft(noRocket, noHomingMissles, noElectric);

	DebugOut(L"Number of bullet: missle %d, electric: %d, rocket: %d", noHomingMissles, noElectric, noRocket);

	keyHandler = new ChooseWeaponSceneKeyHandler(this);
	CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	LoadBaseObjects();
	chooseWeaponScene_ani_set = CAnimationSets::GetInstance()->Get(52000);
	chooseWeaponScene_ani_setnumber = CAnimationSets::GetInstance()->Get(61005);
}
void ChooseWeaponScene::SetSpecialWeapon(int _weaponId)
{
	/*weaponId = _weaponId; 
	switch (weaponId)
	{
	case 0:
	{
		player->SetSpecialBulletType(JASON_HOMING_MISSLES);

		break;
	}
	case 1:
	{
		player->SetSpecialBulletType(JASON_ELECTRIC_BULLET);
		break;
	}
	case 2:
	{
		player->SetSpecialBulletType(JASON_ROCKET_BULLET);
		break;
	}
	}
	DebugOut(L"set weapon: %d\n", weaponId);*/
	PlayerHandler::GetInstance()->SetSpecialWeaponId(_weaponId);
}
void ChooseWeaponScene::LoadBaseObjects()
{
	texturesFilePath = ToLPCWSTR("Resource\\SceneAndSpec\\choose_weapon_scene.txt");
	LoadBaseTextures();
	
	Camera::GetInstance()->SetCamPos(0.0f, 0.0f);	//initial camera
}
void ChooseWeaponScene::LoadBaseTextures()
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

			section = SCENE_SECTION_ANIMATIONS;

			continue;

		}
		if (line == "[ANIMATION_SETS]")							//animationset section
		{
			DebugOut(L"[Info] animation set section");
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		DebugOut(L"[Info] animation section %d\n", section);
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading TEXTURES resources %s\n", texturesFilePath);
}


#pragma region keyhandler

void ChooseWeaponSceneKeyHandler::KeyState(BYTE* states)
{
	if (CGame::GetInstance()->IsKeyDown(DIK_N))
	{

	}
}

void ChooseWeaponSceneKeyHandler::OnKeyDown(int KeyCode)
{
	ChooseWeaponScene* chooseWeaponScene = dynamic_cast<ChooseWeaponScene*>(scence);
	if (KeyCode == DIK_RETURN )
	{ 
		chooseWeaponScene->SetSpecialWeapon(chooseWeaponScene->GetWeaponId());
		DebugOut(L"HoldScene %d\n", SceneManager::GetInstance()->GetHolderScene());
		//SceneManager::GetInstance()->SetScene(SceneManager::GetInstance()->GetHolderScene());
		chooseWeaponScene->SetIsFinished(true);
	}
	if (KeyCode==DIK_LEFT) {
		if (chooseWeaponScene->GetWeaponId() > 0)
			chooseWeaponScene->SetWeaponId(chooseWeaponScene->GetWeaponId() - 1);

	}
	if (KeyCode == DIK_RIGHT) {
		if (chooseWeaponScene->GetWeaponId() < 2)
			chooseWeaponScene->SetWeaponId(chooseWeaponScene->GetWeaponId() + 1);

	}
	DebugOut(L"HoldScene %d\n", chooseWeaponScene->GetWeaponId());
}

void ChooseWeaponSceneKeyHandler::OnKeyUp(int KeyCode)
{
	return;
}
#pragma endregion
#pragma region parseSection
void ChooseWeaponScene::_ParseSection_TEXTURES(string line)
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
void ChooseWeaponScene::_ParseSection_SPRITES(string line)
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
void ChooseWeaponScene::_ParseSection_ANIMATIONS(string line)
{

	vector<string> tokens = split(line);
	DebugOut(L"[Info] animation added: ");
	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	DebugOut(L"[Info] animation added: %d\n", ani_id);
	CAnimations::GetInstance()->Add(ani_id, ani);
}
void ChooseWeaponScene::_ParseSection_ANIMATION_SETS(string line)
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

void ChooseWeaponScene::_ParseSection_CLEARTEXTURES(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CTextures::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Texture %d!\n", idClear);
}
void ChooseWeaponScene::_ParseSection_CLEARSPRITES(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CSprites::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Sprite %d!\n", idClear);
}
void ChooseWeaponScene::_ParseSection_CLEARANIMATIONS(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CAnimations::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation %d!\n", idClear);
}
void ChooseWeaponScene::_ParseSection_CLEARANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);
	int idClear = atoi(tokens[0].c_str());
	CAnimationSets::GetInstance()->ClearAt(idClear);
	DebugOut(L"[INFO] Cleared Animation Set %d!\n", idClear);
}

#pragma endregion

ChooseWeaponScene::~ChooseWeaponScene()
{
}



void ChooseWeaponScene::Update(DWORD dt)
{
	if (isfished)
	{
		keyHandler = new PlayScenceKeyHandler(dynamic_cast<PlayScene*>(SceneManager::GetInstance()->GetHolderScene()));
		CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
		SceneManager::GetInstance()->SetScene(dynamic_cast<PlayScene*>(SceneManager::GetInstance()->GetHolderScene()));
		//dynamic_cast<PlayScene*>(SceneManager::GetInstance()->GetScene())->SetKeyhandler();
	}

#pragma region camera
	
	Camera::GetInstance()->SetCamPos(0, 0);
		
#pragma endregion
#pragma region sceneswitching

	//DebugOut(L"middle\n");
#pragma endregion


}	

void ChooseWeaponScene::Render()
{

	chooseWeaponScene_ani_set->at(0)->IntroRender(1, 0, 0);
	chooseWeaponScene_ani_set->at(1)->IntroRender(1, 54	 + 32 * weaponId, 176);
	chooseWeaponScene_ani_set->at(2)->IntroRender(1, 80 + 32 * weaponId, 176);
	//chooseWeaponScene_ani_setnumber->at(0)->IntroRender(1, 90, 176);
	
}
void ChooseWeaponScene::Unload()
{
	isUnloaded = true;
	delete this;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
