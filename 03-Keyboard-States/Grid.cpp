//#include "Grid.h"
//#include <fstream>
//#include <algorithm>
//#include "Utils.h"
//
//vector<Entity*> CGrid::FilterObjectDuplicate(vector<Entity*> objs)
//{
//	std::sort(objs.begin(), objs.end());
//	objs.erase(std::unique(objs.begin(), objs.end()), objs.end());
//	return objs;
//}
//
//void CGrid::InitGrid(int _mapW, int _mapH)
//{
//	cellH = CELL_SIZE.y;
//	cellW = CELL_SIZE.x;
//	columnGrid = 1 + _mapW / cellW;
//	rowGrid = 1 + _mapH / cellH;
//	cells = new vector<Entity*> * [rowGrid + 1];
//	for (int i = 0; i < rowGrid + 1; i++)
//	{
//		cells[i] = new vector<Entity*>[columnGrid + 1];
//	}
//}
//
//CGrid::CGrid(int _mapW, int _mapH, string _fileobj)
//{
//	//cellH = CELL_SIZE.y;
//	//cellW = CELL_SIZE.x;
//	//columnGrid = 1 + _mapW / cellW;
//	//rowGrid = 1 + _mapH / cellH;
//	//fileobj = _fileobj;
//}
//
//void CGrid::LoadGrid(vector<string> tokens)
//{
//	if (tokens.size() < 3)
//	{
//		DebugOut(L"Load tokens fail");
//		return;
//	}
//	//vector<pair<int, int>> posGrid;
//	int object_type = atoi(tokens[0].c_str());
//	float x = atof(tokens[1].c_str());
//	float y = atof(tokens[2].c_str());
//
//	int grid_x = atof(tokens[3].c_str());
//	int grid_y = atof(tokens[4].c_str());
//	int ani_set_id = atoi(tokens[5].c_str());
//	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
//
//	Entity* obj = NULL;
//	switch (object_type)
//	{
//	case EntityType::TAG_WORM:
//	{
//		obj = new Worm(x, y, JASON::GetInstance());
//		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//
//		obj->SetAnimationSet(ani_set);
//		//listEnemies.push_back(obj);
//		//InsertGrid(obj, posGrid);
//		DebugOut(L"[test] add worm !\n");
//		break;
//	}
//	case EntityType::TAG_BRICK:
//	{
//		obj = new Brick(atof(tokens[6].c_str()), atof(tokens[7].c_str()));
//		obj->SetPosition(x, y);
//		//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//
//		//obj->SetAnimationSet(ani_set);
//		//listObjects.push_back(obj);
//		//InsertGrid(obj, posGrid);
//		DebugOut(L"[test] add brick !\n");
//		break;
//	}
//	case EntityType::TAG_GATE:
//	{
//		int switchId = atoi(tokens[5].c_str());
//		float playerPosX = atoi(tokens[6].c_str());
//		float playerPosY = atoi(tokens[7].c_str());
//		int playerState = atoi(tokens[8].c_str());
//		int isResetCamera = atoi(tokens[9].c_str());
//		int typePlayer = atoi(tokens[10].c_str());
//		float camX = atoi(tokens[11].c_str());
//		int camY = atoi(tokens[12].c_str());
//		obj = new Gate(x, y, switchId, playerPosX, playerPosY, playerState, isResetCamera, typePlayer, camX, camY);
//		//listGates.push_back(obj);
//		//InsertGrid(obj, posGrid);
//		DebugOut(L"[test] add gate !\n");
//		break;
//	}
//	default:
//		obj = nullptr;
//		DebugOut(L"[ERRO] Invalid object type: %d\n", object_type);
//		return;
//	}
//	if (obj != nullptr)
//	{
//		InsertGrid(obj, make_pair(grid_x, grid_y));
//	}
//}
////void CGrid::LoadGrid()
////{
////	cells = new vector<Entity*> * [rowGrid + 1];
////	for (int i = 0; i < rowGrid + 1; i++)
////	{
////		cells[i] = new vector<Entity*>[columnGrid + 1];
////	}
////	ifstream ifs(fileobj);
////	int type;
////	float _x, _y, _w, _h;
////	int _type, _item, directStair, _direct;
////	int _xGrid, _yGrid;
////	int _countGrid;
////	vector<pair<int, int>> posGrid;
////	Entity* a;
////	while (true)
////	{
////		posGrid.clear();
////		ifs >> type;
////		if (type == -555) break;
////		ifs >> _countGrid;
////
////		for (int i = 0; i < _countGrid; i++)
////		{
////			ifs >> _xGrid >> _yGrid;
////			posGrid.push_back(make_pair(_xGrid, _yGrid));
////		}
////		switch (static_cast<EntityType>(type))
////		{
////		case EntityType::TAG_JASON:
////			ifs >> _x >> _y;
////			//CSimon::GetIntance()->SetPosition(_x, _y);
////			if (SceneManager::GetInstance()->GetCurrentSceneID() == 1)
////			{
////				CSimon::GetIntance()->SetPosition(_x, _y);
////			}
////			posSimonDefault = D3DXVECTOR2(_x, _y);
////			a = CSimon::GetIntance();
////			break;
////		case BRICK:
////			ifs >> _x >> _y >> _w >> _h >> _type;
////			if (_type != BRICK_MODEL_TRANSPARENT_1 && _type != BRICK_MODEL_TRANSPARENT_2)
////			{
////				ifs >> _item;
////				a = new Brick(_x, _y, _w, _h, _type, static_cast <ObjectType>(_item));
////			}
////			else
////				a = new Brick(_x, _y, _w, _h, _type);
////			break;
////		case BRIDGE:
////			ifs >> _x >> _y;
////			a = new CBridge(_x, _y);
////			break;
////		case CANDLE:
////			ifs >> _x >> _y >> _item >> _type;
////			a = new CCandle(_x, _y, static_cast <ObjectType>(_item), static_cast <ObjectType>(_type));
////			break;
////		case TRIGGER:
////			ifs >> _x >> _y >> _w >> _h >> _type >> directStair;
////			a = new CTrigger(_x, _y, _w, _h, static_cast <ObjectType>(_type), directStair);
////			if (_type == ITEM_HIDDEN_TRIGGER)
////			{
////				ifs >> _x >> _y >> _item;
////				static_cast<CTrigger*>(a)->SetItemHolder(new CItem(_x, _y, static_cast <ObjectType>(_item)));
////			}
////			break;
////		case ENEMY:
////			ifs >> _x >> _y >> _type;
////			switch (static_cast<ObjectType>(_type))
////			{
////			case BLACKKNIGHT:
////				a = new CBlackKnight(_x, _y);
////				break;
////			case BAT:
////				a = new CBat(_x, _y);
////				break;
////			case GHOST:
////				a = new CGhost(_x, _y);
////				break;
////			case RAVEN:
////				a = new CRaven(_x, _y);
////				break;
////			case FEAMAN:
////				a = new CFeaman(_x, _y);
////				break;
////			case WHITESKELETON:
////				a = new CWhiteSkeleton(_x, _y);
////				break;
////			case ZOMBIE:
////				a = new CZombie(_x, _y);
////				break;
////			case PHANTOMBAT:
////				a = new CPhantomBat(_x, _y);
////				break;
////			default:
////				a = nullptr;
////				break;
////			}
////			break;
////
////		case ENEMY_DOOR:
////			ifs >> _x >> _y >> _type >> _direct;
////			a = new CEnemyDoor(_x, _y, static_cast<ObjectType>(_type), _direct);
////			break;
////		default:
////			a = nullptr;
////			break;
////		}
////		if (static_cast<ObjectType>(type) != SIMON && a != nullptr)
////			InsertGrid(a, posGrid);
////	}
////	ifs.close();
////}
//
//void CGrid::InsertGrid(Entity* obj, pair<int, int> posGrid)
//{
//	cells[posGrid.first][posGrid.second].push_back(obj);
//}
//CGrid::~CGrid()
//{
//}
//
//void CGrid::UpdateGrid(vector<Entity*> objects)
//{
//	for (int i = 0; i < objects.size(); i++)
//	{
//		//thieu cast trigger 
//		if (!dynamic_cast<Brick*>(objects.at(i)) && !dynamic_cast<Gate*>(objects.at(i)))
//		{
//			RemoveObj(objects.at(i), 0);
//			InsertGrid(objects.at(i));
//		}
//	}
//}
//
//void CGrid::RemoveObj(Entity* obj, bool isDeletePointer)
//{
//	RECT rectObj = obj->GetBBox();
//	int minRow = int(rectObj.top) / cellH - 3;
//	int maxRow = int(rectObj.bottom) / cellH + 3;
//	int minColumn = int(rectObj.left) / cellW - 3;
//	int maxColumn = int(rectObj.right) / cellW + 3;
//
//	if (minRow < 0)
//		minRow = 0;
//	if (maxRow >= rowGrid)
//		maxRow = rowGrid - 1;
//	if (minColumn < 0)
//		minColumn = 0;
//	if (maxColumn >= columnGrid)
//		maxColumn = columnGrid - 1;
//
//	for (int i = minRow; i <= maxRow; i++)
//	{
//		for (int j = minColumn; j <= maxColumn; j++)
//		{
//			for (int m = 0; m < cells[i][j].size(); m++)
//			{
//				if (cells[i][j].at(m) == obj)
//				{
//					cells[i][j].erase(cells[i][j].begin() + m);
//				}
//			}
//		}
//	}
//	if (isDeletePointer)
//		SAFE_DELETE(obj);
//}
//
//void CGrid::UnLoadGrid()
//{
//	for (int i = 0; i < rowGrid; i++)
//	{
//		delete[] cells[i];
//	}
//	delete[] cells;
//}
//
//void CGrid::InsertGrid(Entity* obj)
//{
//	RECT rectObj = obj->GetBBox();
//	int minRow = int(rectObj.top) / cellH;
//	int maxRow = int(rectObj.bottom) / cellH;
//	int minColumn = int(rectObj.left) / cellW;
//	int maxColumn = int(rectObj.right) / cellW;
//
//	for (int i = minRow; i <= maxRow; i++)
//	{
//		for (int j = minColumn; j <= maxColumn; j++)
//		{
//			cells[i][j].push_back(obj);
//		}
//	}
//}
//
//D3DXVECTOR2 CGrid::GetPosPlayerDefault()
//{
//	return posPlayerDefault;
//}
//
//vector<Entity*> CGrid::GetListUpdateObj()
//{
//	RECT rectCam = Camera::GetInstance()->GetRectCam();
//	int minRow = int(rectCam.top) / cellH;
//	int maxRow = int(rectCam.bottom) / cellH - 1; // sử dụng floor
//
//	//int minRow = (CCamera::GetInstance()->GetCurrentFloor()-1)*2;
//	//int maxRow = minRow + 2; // sử dụng floor
//
//	int minColumn = int(rectCam.left) / cellW;
//	int maxColumn = int(rectCam.right) / cellW;
//
//	vector<Entity*> result;
//	//DebugOut(L"%d,%d   %d,%d\n", minRow, maxRow, minColumn, maxColumn);
//
//	for (int i = minRow; i <= maxRow; i++)
//	{
//		for (int j = minColumn; j <= maxColumn; j++)
//		{
//			for (int m = 0; m < cells[i][j].size(); m++)
//				//if (!dynamic_cast<Brick*>(cells[i][j].at(m)) && !dynamic_cast<CTrigger*>(cells[i][j].at(m)))
//				result.push_back(cells[i][j].at(m));
//		}
//	}
//	result = FilterObjectDuplicate(result);
//
//	return result;
//}
//
//vector<Entity*> CGrid::GetListRenderObj()
//{
//	RECT rectCam = Camera::GetInstance()->GetRectCam();
//	int minRow = int(rectCam.top) / cellH;
//	int maxRow = int(rectCam.bottom) / cellH; // sử dụng floor
//
//	//int minRow = (Camera::GetInstance()->GetCurrentFloor()-1)*2;
//	//int maxRow = minRow + 2; // sử dụng floor
//
//	int minColumn = int(rectCam.left) / cellW;
//	int maxColumn = int(rectCam.right) / cellW;
//
//	vector<Entity*> result;
//
//	for (int i = minRow; i <= maxRow; i++)
//	{
//		for (int j = minColumn; j <= maxColumn; j++)
//		{
//			for (int m = 0; m < cells[i][j].size(); m++)
//				result.push_back(cells[i][j].at(m));
//		}
//	}
//	result = FilterObjectDuplicate(result);
//
//	return result;
//}
