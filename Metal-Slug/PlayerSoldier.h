#pragma once
#include"Soldier.h"
#include "Weapon.h"
#include "Camera.h"



class PlayerSoldier: public Soldier{
protected:
	int grenadeCount;
	float fireRateMultiplier;
	int activeWeapon;
	Weapon* currentWeapon;
	Weapon* inventory[2];
	bool activeState;
	void throwGrenade() {}
	void meleeAttack(){}
	void switchWeapon(){}
	void setState(){}
	//void boardVehicle(playerVehicle*){}
	//void ejectVehicle(playerVehicle*){}
	Clock specialMoveTimer;
	bool specialMoveActive;

public:
	PlayerSoldier(Weapon* w) {
		activeWeapon = 0;
		specialMoveActive = false;
		inventory[0] = w;
		activeState = true;
		currentWeapon = inventory[activeWeapon];
	}
	virtual void specialMove() = 0;
	virtual void updateSpecialMove() = 0;
	void equipWeapon(int index) {
		currentWeapon = inventory[index];
	}
	void addWeapon(Weapon* w) {
		inventory[1] = w; //0 is always the pistol, 1 is the special weapon
	}

	virtual void shoot(Position spawn) {
		
		currentWeapon->attack(spawn, 1.0f, getFireRateMultiplier());
		
	}
	// Getters
	int getGrenadeCount() const { return grenadeCount; }
	bool isActive() const { return activeState; }
	float getFireRateMultiplier() const { return fireRateMultiplier; }
	bool getSpecialMoveActive() { return specialMoveActive; }
	Weapon* getActiveWeapon() const { return inventory[activeWeapon]; }
	// Setters
	void setGrenadeCount(int count) { grenadeCount = count; }
	void setActiveWeapon(int weaponIndex) { activeWeapon = weaponIndex; }
	void setActiveState(bool state) { activeState = state; }

	virtual const char* getTexturePath() const = 0;
};

class Marco : public PlayerSoldier {
private:

public:
	Marco(Weapon* w): PlayerSoldier(w) {
		grenadeCount = 8; //2 less than normal
		fireRateMultiplier = 1.25; //25% higher fire rate
		
	}
	const char* getTexturePath() const override { return "Data/Characters/Marco.png"; }
	void specialMove() override {
		// Implement Marco's special move(multi directional shooting)
		
		if (!specialMoveActive) {
			std::cout << "Marco's special move activate" << std::endl;
			specialMoveActive = true;
			specialMoveTimer.restart();
		}

	}
	void updateSpecialMove() override {
		if (specialMoveActive && specialMoveTimer.getElapsedTime().asSeconds() >= 10.0f) {
			specialMoveActive = false;
		}
	}
	  
	void shoot(Position spawn) override {
		std::cout << "OVERRIDEN SHOOTING";
		currentWeapon->attack(spawn, 1.0f, getFireRateMultiplier());
		if (specialMoveActive) {
			currentWeapon->attackDirect(spawn, -1.0f); 
		}
	}
};


class Tarma : public PlayerSoldier {
public:
	Tarma(Weapon* w) : PlayerSoldier(w) {
		grenadeCount = 10;
		fireRateMultiplier = 1.0;

	}
	virtual void specialMove() override {
		// Implement Tarma's special move(rapid fire)
	}
	void updateSpecialMove() override {

	}
	const char* getTexturePath() const override { return "Data/Characters/Tarma.png"; }
};


class Eri : public PlayerSoldier {
public:
	Eri(Weapon* w) : PlayerSoldier(w) {
		grenadeCount = 10; 
		fireRateMultiplier = 0.80;

	}
	virtual void specialMove() override {
		// Implement Eri's special move(stealth)
	}
	void updateSpecialMove() override {
	
	}
	const char* getTexturePath() const override { return "Data/Characters/Eri.png"; }
};


class Fio : public PlayerSoldier {
public:
	Fio(Weapon* w) : PlayerSoldier(w) {
		grenadeCount = 10; 
		fireRateMultiplier = 1.0f;
	}
	void specialMove() override {
		// Implement Fio's special move (Fire rate increased by 100% for 10 seconds)
		
		if (!specialMoveActive) {
			std::cout << "Fio's special move activate" << std::endl;
			specialMoveActive = true;
			fireRateMultiplier = 2.0f; // Double the fire rate
			// Start a timer to reset the fire rate after 10 seconds
			specialMoveTimer.restart();
		}
		
	}
	void updateSpecialMove() override {
		if (specialMoveActive && specialMoveTimer.getElapsedTime().asSeconds() >= 10.0f) {
			specialMoveActive = false;
			fireRateMultiplier = 1.0f; // Reset to normal fire rate
		}
	}

	const char* getTexturePath() const override { return "Data/Characters/Fio.png"; }
};