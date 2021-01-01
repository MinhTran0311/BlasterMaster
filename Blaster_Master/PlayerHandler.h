#pragma once
#include "global.h"
class PlayerHandler
{
private:
	static PlayerHandler* instance;
	int life = 2;
	int playerDirectionBeforePassGate = 1;
	int jasonHealth = PLAYER_MAX_HEALTH;
	int jasonGundam = PLAYER_DEFAULT_GUNDAM;
	int jasonStage = 1;
	float jasonXPos, jasonYPos;
	int sophiaHealth = PLAYER_MAX_HEALTH;
	int sophiaGundam = PLAYER_DEFAULT_GUNDAM;
	int sophiaStage = 1;
	float sophiaXPos, sophiaYPos;
	int specialWeaponId = 1;

	int noOfHomingMisslesWeaponLeft = 3;
	int noOfElectricWeaponLeft = 3;
	int noOfRocketsWeaponLeft = 3;
public:
#pragma region getter setter
	int GetLife() { return life; };
	int GetPlayerDirectionBeforePassGate() { return playerDirectionBeforePassGate; };
	int GetJasonHealth() { return jasonHealth; };
	int GetJasonGunDam() { return jasonGundam; };
	int GetJasonStage() { return jasonStage; };
	float GetJasonPosition(float& x, float& y) { x = jasonXPos; y = jasonYPos; };
	int GetSophiaHealth() { return sophiaHealth; };
	int GetSophiaGunDam() { return sophiaGundam; };
	int GetSophiaStage() { return sophiaStage; };
	float GetJasonPosition(float& x, float& y) { x = sophiaXPos; y = sophiaYPos; };
	int GetSpecialWeaponId() { return specialWeaponId; };
	void GetNoOfBulletLeft(int& rocket, int& missles, int& electric) {
		rocket = noOfRocketsWeaponLeft;
		missles = noOfHomingMisslesWeaponLeft;
		electric = noOfElectricWeaponLeft;
	}

	int SetLife(int lifeset) { life = lifeset; };
	int SetJasonHealth(int healthset) { return jasonHealth = healthset; };
	int SGetJasonGunDam(int damset) { return jasonGundam = damset; };
	int SetJasonStage(int stageset) { return jasonStage = stageset; };
	float SetJasonPosition(float& x, float& y) { jasonXPos = x; jasonYPos = y; };
	int SGetSophiaHealth(int healthset) { sophiaHealth = healthset; };
	int SetSophiaGunDam(int damset) { sophiaGundam = damset; };
	int SetSophiaStage(int stageset) { return sophiaStage = stageset; };
	float SetJasonPosition(float& x, float& y) { sophiaXPos = x; sophiaYPos = y; };
	int SetSpecialWeaponId(int id) { specialWeaponId = id; };
	void SetSpecialBulletType(EntityType _specialbullettype) { specialWeaponId = _specialbullettype; };
#pragma endregion

	PlayerHandler();
	~PlayerHandler();
	static PlayerHandler* GetInstance();
	
};

