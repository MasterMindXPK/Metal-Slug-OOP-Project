#pragma once
#include"Soldier.h"

class PlayerSoldier{
protected:
	int grenadeCount;
	float fireRateMultiplier;
	int activeWeapon;
	//Weapon* currentWeapon;
	bool activeState;

	void shoot(){}
	void throwGrenade() {}
	void meleeAttack(){}
	void switchWeapon(){}
	void setState(){}
	//void boardVehicle(playerVehicle*){}
	//void ejectVehicle(playerVehicle*){}
	virtual void specialMove() = 0;

public:
	// Getters
	int getGrenadeCount() const { return grenadeCount; }
	int getActiveWeapon() const { return activeWeapon; }
	bool isActive() const { return activeState; }

	// Setters
	void setGrenadeCount(int count) { grenadeCount = count; }
	void setActiveWeapon(int weaponIndex) { activeWeapon = weaponIndex; }
	void setActiveState(bool state) { activeState = state; }

	virtual const char* getTexturePath() const = 0;
};

class Marco : public PlayerSoldier {
private:

public:
	Marco() {
		grenadeCount = 8; //2 less than normal
		fireRateMultiplier = 0.25;
		activeWeapon = 0;
		activeState = true;

		
	}
		const char* getTexturePath() const override { return "Data/Characters/Marco.png"; }
		void specialMove() override {
		// Implement Marco's special move(multi directional shooting)
	}
};

class Tarma : public PlayerSoldier {
public:
	Tarma() {
		grenadeCount = 10;
		fireRateMultiplier = 1.0;
		activeWeapon = 0;
		activeState = true;

	}
	virtual void specialMove() override {
		// Implement Tarma's special move(rapid fire)
	}
	const char* getTexturePath() const override { return "Data/Characters/Tarma.png"; }
};

class Eri : public PlayerSoldier {
public:
	Eri() {
		grenadeCount = 10; 
		fireRateMultiplier = 0.75;
		activeWeapon = 0;
		activeState = true;

	}
	virtual void specialMove() override {
		// Implement Eri's special move(stealth)
	}
	const char* getTexturePath() const override { return "Data/Characters/Eri.png"; }
};

class Fio : public PlayerSoldier {
public:
	Fio() {
		grenadeCount = 10; 
		fireRateMultiplier = 1;
		activeWeapon = 0;
		activeState = true;

	}
	virtual void specialMove() override {
		// Implement Fio's special move(tech support)
	}
	const char* getTexturePath() const override { return "Data/Characters/Fio.png"; }
};