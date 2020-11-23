#include "Grid.h"
#include <fstream>
#include <algorithm>
#include "Utils.h"
#include "PlayScene.h"
#include <assert.h>
#include "Insects.h"
#include "Skulls.h"
#include "Domes.h"
#include "Orbs.h"
#include "Jumpers.h"
#include "Eyeballs.h"
#include "Cannons.h"
#include "Teleporters.h"
vector<LPGAMEENTITY> CGrid::FilterObjectDuplicate(vector<LPGAMEENTITY> objs)
{
	std::sort(objs.begin(), objs.end());
	objs.erase(std::unique(objs.begin(), objs.end()), objs.end());
	return objs;
}
CGrid* CGrid::__instance = NULL;
CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}

void CGrid::InitGrid(int _mapW, int _mapH)
{
	cellW = CELL_SIZE.x;
	cellH = CELL_SIZE.y;

	columnGrid = 1 + _mapW / CELL_SIZE.x;
	rowGrid = 1 + _mapH / CELL_SIZE.y;
	DebugOut(L"number of rows: %d\n", rowGrid);
	DebugOut(L"number of columns: %d\n", columnGrid);

	cells = new vector<LPGAMEENTITY> * [rowGrid+1];
	for (int i = 0; i < rowGrid+1; i++)
	{
		cells[i] = new vector<LPGAMEENTITY>[columnGrid+1];
	}
}

CGrid::CGrid()
{
}

//CGrid::CGrid(int _mapW, int _mapH, string _fileobj)
//{
//	//CELL_SIZE.y = CELL_SIZE.y;
//	//CELL_SIZE.x = CELL_SIZE.x;
//	//columnGrid = 1 + _mapW / CELL_SIZE.x;
//	//rowGrid = 1 + _mapH / CELL_SIZE.y;
//	//fileobj = _fileobj;
//}

void CGrid::LoadGrid(vector<string> tokens, JASON* &playscene_player)
{
	player = playscene_player;
	if (tokens.size() < 3)
	{
		DebugOut(L"Load tokens fail");
		return;
	}
	//vector<pair<int, int>> posGrid;
	//load type, x,y
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	//load values of grid positions
	vector<pair<int, int>> posGrid;
	int number_of_grid = atof(tokens[3].c_str());
	for (int i = 1; i <= number_of_grid; i++)
	{
		posGrid.push_back(make_pair(atof(tokens[3 + 2 * i-1].c_str()), atof(tokens[3 + 2 * i].c_str())));
	}
	int milestone =	3 + 2 * number_of_grid;
	int ani_set_id = atoi(tokens[milestone+1].c_str());
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	LPGAMEENTITY obj = NULL;
	switch (object_type)
	{
	case EntityType::TAG_WORM:
	{
		obj = new Worm(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add worm !\n");
		break;
	}
	//doi voi brick thi milestone bang 5
	case EntityType::TAG_FLOATER:
	{
		obj = new Floaters(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add floater !\n");
		break;
	}
	case EntityType::TAG_INSECT:
	{
		obj = new Insects(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add insect !\n");
		break;
	}
	case EntityType::TAG_ORBS:
	{
		obj = new Orbs(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add insect !\n");
		break;
	}
	case EntityType::TAG_JUMPERS:
	{
		obj = new Jumpers(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add insect !\n");
		break;
	}
	case EntityType::TAG_SKULLS:
	{
		obj = new Skulls(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add insect !\n");
		break;
	}
	case EntityType::TAG_DOMES:
	{
		obj = new Domes(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add domes !\n");
		break;
	}
	case EntityType::TAG_EYEBALLS:
	{
		obj = new Eyeballs(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add eyeballs !\n");
		break;
	}
	case EntityType::TAG_TELEPORTERS:
	{
		obj = new Teleporters(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add teleporters !\n");
		break;
	}
	case EntityType::TAG_CANNONS:
	{
		obj = new Cannons(x, y, player);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add cannons !\n");
		break;
	}
	case EntityType::TAG_BRICK:
	{
		obj = new Brick(atof(tokens[milestone + 2].c_str()), atof(tokens[milestone + 3].c_str()));
		obj->SetPosition(x, y);
		//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		//obj->SetAnimationSet(ani_set);
		DebugOut(L"[test] add brick !\n");
		break;
	}
	case EntityType::TAG_GATE:
	{

		int switchId = atoi(tokens[milestone + 1].c_str());
		
		float playerPosX = atoi(tokens[milestone + 2].c_str());
		
		
		float playerPosY = atoi(tokens[milestone + 3].c_str());
		int playerState = atoi(tokens[milestone + 4].c_str());

		int isResetCamera = atoi(tokens[milestone + 5].c_str());

		int typePlayer = atoi(tokens[milestone + 6].c_str());
		float camX = atoi(tokens[milestone + 7].c_str());
		DebugOut(L"Tạo gate %d", camX);
		int camY = atoi(tokens[milestone + 8].c_str());

		obj = new Gate(x, y, switchId, playerPosX, playerPosY, playerState, isResetCamera, typePlayer, camX, camY);
		DebugOut(L"[test] add gate !\n");
		break;
	}
	//case EntityType::TAG_JASON:
	//{
	//	obj = new JASON(x, y);
	//	DebugOut(L"[test] Jason create!\n");
	//	player = obj;
	//	playscene_player = static_cast<JASON*>(obj);
	//	break;
	//}
	default:
		obj = nullptr;
		DebugOut(L"[ERRO] Invalid object type: %d\n", object_type);
		return;
	}
//	DebugOut(L"number of grid pairs: %d\n", posGrid.size());
	for (int i = 0; i < posGrid.size(); i++)
		DebugOut(L"Entity added at grid row %d collumn %d \n", posGrid.at(i).first, posGrid.at(i).second);
	if (obj != nullptr)
	{
		InsertGrid(obj, posGrid);
	}
}
//void CGrid::LoadGrid()
//{
//	cells = new vector<Entity*> * [rowGrid + 1];
//	for (int i = 0; i < rowGrid + 1; i++)
//	{
//		cells[i] = new vector<Entity*>[columnGrid + 1];
//	}
//	ifstream ifs(fileobj);
//	int type;
//	float _x, _y, _w, _h;
//	int _type, _item, directStair, _direct;
//	int _xGrid, _yGrid;
//	int _countGrid;
//	vector<pair<int, int>> posGrid;
//	Entity* a;
//	while (true)
//	{
//		posGrid.clear();
//		ifs >> type;
//		if (type == -555) break;
//		ifs >> _countGrid;
//
//		for (int i = 0; i < _countGrid; i++)
//		{
//			ifs >> _xGrid >> _yGrid;
//			posGrid.push_back(make_pair(_xGrid, _yGrid));
//		}
//		switch (static_cast<EntityType>(type))
//		{
//		case EntityType::TAG_JASON:
//			ifs >> _x >> _y;
//			//CSimon::GetIntance()->SetPosition(_x, _y);
//			if (SceneManager::GetInstance()->GetCurrentSceneID() == 1)
//			{
//				CSimon::GetIntance()->SetPosition(_x, _y);
//			}
//			posSimonDefault = D3DXVECTOR2(_x, _y);
//			a = CSimon::GetIntance();
//			break;
//		case BRICK:
//			ifs >> _x >> _y >> _w >> _h >> _type;
//			if (_type != BRICK_MODEL_TRANSPARENT_1 && _type != BRICK_MODEL_TRANSPARENT_2)
//			{
//				ifs >> _item;
//				a = new Brick(_x, _y, _w, _h, _type, static_cast <ObjectType>(_item));
//			}
//			else
//				a = new Brick(_x, _y, _w, _h, _type);
//			break;
//		case BRIDGE:
//			ifs >> _x >> _y;
//			a = new CBridge(_x, _y);
//			break;
//		case CANDLE:
//			ifs >> _x >> _y >> _item >> _type;
//			a = new CCandle(_x, _y, static_cast <ObjectType>(_item), static_cast <ObjectType>(_type));
//			break;
//		case TRIGGER:
//			ifs >> _x >> _y >> _w >> _h >> _type >> directStair;
//			a = new CTrigger(_x, _y, _w, _h, static_cast <ObjectType>(_type), directStair);
//			if (_type == ITEM_HIDDEN_TRIGGER)
//			{
//				ifs >> _x >> _y >> _item;
//				static_cast<CTrigger*>(a)->SetItemHolder(new CItem(_x, _y, static_cast <ObjectType>(_item)));
//			}
//			break;
//		case ENEMY:
//			ifs >> _x >> _y >> _type;
//			switch (static_cast<ObjectType>(_type))
//			{
//			case BLACKKNIGHT:
//				a = new CBlackKnight(_x, _y);
//				break;
//			case BAT:
//				a = new CBat(_x, _y);
//				break;
//			case GHOST:
//				a = new CGhost(_x, _y);
//				break;
//			case RAVEN:
//				a = new CRaven(_x, _y);
//				break;
//			case FEAMAN:
//				a = new CFeaman(_x, _y);
//				break;
//			case WHITESKELETON:
//				a = new CWhiteSkeleton(_x, _y);
//				break;
//			case ZOMBIE:
//				a = new CZombie(_x, _y);
//				break;
//			case PHANTOMBAT:
//				a = new CPhantomBat(_x, _y);
//				break;
//			default:
//				a = nullptr;
//				break;
//			}
//			break;
//
//		case ENEMY_DOOR:
//			ifs >> _x >> _y >> _type >> _direct;
//			a = new CEnemyDoor(_x, _y, static_cast<ObjectType>(_type), _direct);
//			break;
//		default:
//			a = nullptr;
//			break;
//		}
//		if (static_cast<ObjectType>(type) != SIMON && a != nullptr)
//			InsertGrid(a, posGrid);
//	}
//	ifs.close();
//}

void CGrid::InsertGrid(LPGAMEENTITY obj, vector<pair<int, int>> posGrid)
{
	for (int i = 0; i < posGrid.size(); i++)
	{
		cells[posGrid.at(i).first][posGrid.at(i).second].push_back(obj);
		DebugOut(L"Entity inserted at grid row %d collumn %d\n", posGrid.at(i).first, posGrid.at(i).second);
	}
}
CGrid::~CGrid()
{
}

void CGrid::UpdateGrid(vector<LPGAMEENTITY> objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		//thieu cast trigger 
		if (!dynamic_cast<Brick*>(objects.at(i)) && !dynamic_cast<Gate*>(objects.at(i)))
		{
			RemoveObj(objects.at(i), 0);	//xoa doi tuuong trong cac grid
			InsertGrid(objects.at(i));		//them lai doi tuong vao grid moi
		}
	}
}

void CGrid::RemoveObj(LPGAMEENTITY obj, bool isDeletePointer)
{
	RECT rectObj = obj->GetBBox();
	int minRow = int(rectObj.top) / CELL_SIZE.y - 3;
	int maxRow = int(rectObj.bottom) / CELL_SIZE.y + 3;
	int minColumn = int(rectObj.left) / CELL_SIZE.x - 3;
	int maxColumn = int(rectObj.right) / CELL_SIZE.x + 3;	

	if (minRow < 0)
		minRow = 0;
	if (maxRow >= rowGrid)
		maxRow = rowGrid - 1;
	if (minColumn < 0)
		minColumn = 0;
	if (maxColumn >= columnGrid)
		maxColumn = columnGrid - 1;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			for (int m = 0; m < cells[i][j].size(); m++)
			{
				if (cells[i][j].at(m) == obj)
				{
					cells[i][j].erase(cells[i][j].begin() + m);
				}
			}
		}
	}
	DebugOut(L"REmove Object: %d\n",obj->GetType());
	if (isDeletePointer)
	{
		SAFE_DELETE(obj);
	}
}

void CGrid::UnLoadGrid()
{
	for (int i = 0; i < rowGrid; i++)
	{
		delete[] cells[i];
	}
	delete[] cells;
}

void CGrid::InsertGrid(LPGAMEENTITY obj)
{
	
	RECT rectObj = obj->GetBBox();
	int minRow = int(rectObj.top) / CELL_SIZE.y;
	int maxRow = int(rectObj.bottom) / CELL_SIZE.y;
	int minColumn = int(rectObj.left) / CELL_SIZE.x;
	int maxColumn = int(rectObj.right) / CELL_SIZE.x;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			cells[i][j].push_back(obj);
		}
	}
}

bool CGrid::CheckBulletLimitation(EntityType typebullet, float xPlayerPos, float yPlayerPos, int limit)
{
	int center_row = int(yPlayerPos) / CELL_SIZE.y;
	int center_column = int(xPlayerPos) / CELL_SIZE.y;
	int minRow = center_row - 3;
	int maxRow = center_row + 3;
	int minColumn = center_column-3;
	int maxColumn = center_column + 3;
	if (minRow < 0)
		minRow = 0;
	if (maxRow >= rowGrid)
		maxRow = rowGrid - 1;
	if (minColumn < 0)
		minColumn = 0;
	if (maxColumn >= columnGrid)
		maxColumn = columnGrid - 1;

	int bullet_count = 0;
	for (int i= minRow; i< maxRow; i++)
		for (int j= minColumn; j< maxColumn;j++)
			for (int k = 0; k < cells[i][j].size(); k++)
			{
				if (static_cast<Bullet*>(cells[i][j].at(k))->GetBulletType() == typebullet)
					bullet_count++;
			}
	if (bullet_count < limit)
		return true;
	return false;
}

D3DXVECTOR2 CGrid::GetPosPlayerDefault()
{
	return posPlayerDefault;
}

vector<LPGAMEENTITY> CGrid::GetListUpdateObj()
{
	RECT rectCam = Camera::GetInstance()->GetRectCam();
	int minRow = int(rectCam.top) / CELL_SIZE.y;
	//fix
	int maxRow = int(rectCam.bottom) / CELL_SIZE.y;

	//int minRow = (CCamera::GetInstance()->GetCurrentFloor()-1)*2;
	//int maxRow = minRow + 2; // sử dụng floor

	int minColumn = int(rectCam.left) / CELL_SIZE.x;
	int maxColumn = int(rectCam.right) / CELL_SIZE.x;

	vector<LPGAMEENTITY> result;
	//DebugOut(L"%d,%d   %d,%d\n", minRow, maxRow, minColumn, maxColumn);

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			for (int m = 0; m < cells[i][j].size(); m++)
				//if (!dynamic_cast<Brick*>(cells[i][j].at(m)) && !dynamic_cast<CTrigger*>(cells[i][j].at(m)))
				result.push_back(cells[i][j].at(m));
		}
	}
	result = FilterObjectDuplicate(result);

	return result;
}

vector<LPGAMEENTITY> CGrid::GetListRenderObj()
{
	RECT rectCam = Camera::GetInstance()->GetRectCam();
	int minRow = int(rectCam.top) / CELL_SIZE.y;
	int maxRow = int(rectCam.bottom) / CELL_SIZE.y; 

	int minColumn = int(rectCam.left) / CELL_SIZE.x;
	int maxColumn = int(rectCam.right) / CELL_SIZE.x;

	DebugOut(L"cot nho nhat %d,  camera x: %d\n", minColumn, rectCam.left);
	DebugOut(L"cot lon nhat %d,  camera x right: %d\n", maxColumn, rectCam.right);
	DebugOut(L"hang nho nhat %d, camera y: %d\n", minRow, rectCam.top);
	DebugOut(L"hang lon nhat %d, camera y bot: %d\n", maxRow, rectCam.bottom);
	
	vector<LPGAMEENTITY> render_result;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			for (int m = 0; m < cells[i][j].size(); m++)
				render_result.push_back(cells[i][j].at(m));
			DebugOut(L"hang %d , cot %d\n", i, j);
		}
	}
	DebugOut(L"size before: %d \n", render_result.size());
	render_result = FilterObjectDuplicate(render_result);
	DebugOut(L"size: %d \n", cells[3][1].size());
	DebugOut(L"size result: %d \n", render_result.size());
	return render_result;
}
