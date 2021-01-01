#include "PlayerHandler.h"
PlayerHandler::PlayerHandler()
{
}

PlayerHandler::~PlayerHandler()
{
}

PlayerHandler* PlayerHandler::instance = 0;
PlayerHandler* PlayerHandler::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PlayerHandler();
	}
	return instance;
}