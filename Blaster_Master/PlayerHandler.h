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
	void GetJasonPosition(float& x, float& y) { x = jasonXPos; y = jasonYPos; };
	int GetSophiaHealth() { return sophiaHealth; };
	int GetSophiaGunDam() { return sophiaGundam; };
	int GetSophiaStage() { return sophiaStage; };
	void GetSophiaPosition(float& x, float& y) { x = sophiaXPos; y = sophiaYPos; };
	int GetSpecialWeaponId() { return specialWeaponId; };
	int GetNoMissles() { return noOfHomingMisslesWeaponLeft; };
	int GetNoElectric() { return noOfElectricWeaponLeft; };
	int GetNoRocket() { return noOfRocketsWeaponLeft; };
	void GetNoOfBulletLeft(int& rocket, int& missles, int& electric) {
		rocket = noOfRocketsWeaponLeft;
		missles = noOfHomingMisslesWeaponLeft;
		electric = noOfElectricWeaponLeft;
	}

	void SetLife(int lifeset) { life = lifeset; };
	void SetPlayerDirectionBeforePassGate(int direction) { playerDirectionBeforePassGate = direction; };
	void SetJasonHealth(int healthset) { jasonHealth = healthset; };
	void SetJasonGunDam(int damset) { jasonGundam = damset; };
	void SetJasonStage(int stageset) { jasonStage = stageset; };
	void SetJasonPosition(float& x, float& y) { jasonXPos = x; jasonYPos = y; };
	void SetSophiaHealth(int healthset) { sophiaHealth = healthset; };
	void SetSophiaGunDam(int damset) { sophiaGundam = damset; };
	void SetSophiaStage(int stageset) { sophiaStage = stageset; };
	void SetSophiaPosition(float& x, float& y) { sophiaXPos = x; sophiaYPos = y; };
	void SetSpecialWeaponId(int id) { specialWeaponId = id; };
	void SetNoMissles(int no) { noOfHomingMisslesWeaponLeft = no; };
	void SetNoElectric(int no) { noOfElectricWeaponLeft = no; };
	void SetNoRocket(int no) { noOfRocketsWeaponLeft = no; };
	//void SetSpecialBulletType(EntityType _specialbullettype) { specialWeaponId = _specialbullettype; };
#pragma endregion

	PlayerHandler();
	~PlayerHandler();
	static PlayerHandler* GetInstance();
	void Init() {};
	void SetJasonInfor(int, float, float, int, int,int=0);
	void SetSophiaInfor(int, float, float, int, int,int=0);
	void Restart();
};

