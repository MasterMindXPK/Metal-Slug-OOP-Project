#pragma once
#pragma once
#include "Soldier.h"
#include "Survival.h" 
#include "Weapon.h"  
#include "Camera.h"
#include <iostream>

class PlayerSoldier : public Soldier {
protected:
	int grenadeCount;
	float fireRateMultiplier;
	Weapon* ownPistol;
	Weapon* equippedWeapon;
	GrenadeWeapon* ownGrenade;
	MeleeWeapon* knife;
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
	PlayerSoldier() {
		ownPistol = WeaponFactory::createPistol();
		equippedWeapon = ownPistol;
		specialMoveActive = false;
		activeState = true;
		ownGrenade = WeaponFactory::createHandGrenade(10);	//10 grenades by default
		knife = WeaponFactory::createMeleeKnife();
	}
	~PlayerSoldier() {
		delete ownPistol;
		delete ownGrenade;
		delete knife;
	}
	void equipWeapon(Weapon* w) {
		equippedWeapon = w;
	}
	void equipWeaponDev(Weapon* w) {
		if (equippedWeapon != ownPistol) {
			delete equippedWeapon;
		}
		equippedWeapon = w;
	}
	void dropWeapon(Weapon* w) {
		if (equippedWeapon == w) {
			equippedWeapon = ownPistol;
		}
	}

	virtual void shoot(Position spawn, float dirX, float dirY) {
		equippedWeapon->attack(spawn, dirX, dirY, getFireRateMultiplier());
	}
	
	virtual const char* getTexturePath() const = 0;
	virtual void specialMove() = 0;
	virtual void updateSpecialMove() = 0;

	//==============GrenadeLogic=================
	void throwGrenade(Position spawn, float dirX, float dirY) {
		ownGrenade->attack(spawn, dirX, dirY, 1.0f);
	}
	void updateGrenade(float dt, Survival& level, int screenW, int screenH) {
		ownGrenade->updateBullets(dt, level, screenW, screenH);
	}
	void drawGrenade(RenderWindow& window, float cameraOffset) {
		ownGrenade->drawBullets(window, cameraOffset);
	}
	bool swingKnife(Position pos, float dirX) {
		return knife->swing(pos, dirX);
	}
	// Getters
	int getGrenadeCount() const { return grenadeCount; }
	bool isActive() const { return activeState; }
	float getFireRateMultiplier() const { return fireRateMultiplier; }
	bool getSpecialMoveActive() { return specialMoveActive; }
	Weapon* getActiveWeapon() const { return equippedWeapon; }
	GrenadeWeapon* getGrenade() const { return ownGrenade; }
	MeleeWeapon* getKnife() const { return knife; }
	// Setters
	void setActiveState(bool state) { activeState = state; }

	
};

class Marco : public PlayerSoldier {
public:
	Marco(){
		grenadeCount = 8; //2 less than normal
		fireRateMultiplier = 1.25; //25% higher fire rate
		delete ownGrenade;
		ownGrenade = WeaponFactory::createHandGrenade(grenadeCount);
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

	void shoot(Position spawn, float dirX, float dirY) override {
		std::cout << "OVERRIDEN SHOOTING";
		equippedWeapon->attack(spawn, dirX, dirY, getFireRateMultiplier());
		if (specialMoveActive) {
			equippedWeapon->attackDirect(spawn, -dirX, dirY);
		}
	}
};

class Tarma : public PlayerSoldier {
public:
	Tarma(){
		grenadeCount = 10;
		fireRateMultiplier = 1.0;
		delete ownGrenade;
		ownGrenade = WeaponFactory::createHandGrenade(grenadeCount);
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
	Eri(){
		grenadeCount = 10; 
		fireRateMultiplier = 0.80;
		delete ownGrenade;
		ownGrenade = WeaponFactory::createFireBombGrenade(grenadeCount);
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
	Fio(){
		grenadeCount = 10; 
		fireRateMultiplier = 1.0f;
		delete ownGrenade;
		ownGrenade = WeaponFactory::createHandGrenade(grenadeCount);
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