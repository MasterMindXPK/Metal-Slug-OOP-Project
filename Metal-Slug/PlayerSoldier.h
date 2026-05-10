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
    bool activeState;
    void throwGrenade() {}
    void meleeAttack() {}
    void switchWeapon() {}
    void setState() {}
    Clock specialMoveTimer;
    bool specialMoveActive;

public:
    PlayerSoldier() {
        ownPistol = WeaponFactory::createMachineGun();
        equippedWeapon = ownPistol;
        specialMoveActive = false;
        activeState = true;
    }
    ~PlayerSoldier() {
        delete ownPistol;
    }
    virtual void specialMove() = 0;
    virtual void updateSpecialMove() = 0;
    void equipWeapon(Weapon* w) {
        equippedWeapon = w;
    }
    void dropWeapon(Weapon* w) {
        if (equippedWeapon == w) {
            equippedWeapon = ownPistol;
        }
    }

    virtual void shoot(Position spawn, float dirX, float dirY) {
  


        equippedWeapon->attack(spawn, dirX, getFireRateMultiplier());
    }

    // Getters
    int getGrenadeCount() const { return grenadeCount; }
    bool isActive() const { return activeState; }
    float getFireRateMultiplier() const { return fireRateMultiplier; }
    bool getSpecialMoveActive() { return specialMoveActive; }
    Weapon* getActiveWeapon() const { return equippedWeapon; }

    // Setters
    void setGrenadeCount(int count) { grenadeCount = count; }
    void setActiveState(bool state) { activeState = state; }

    virtual const char* getTexturePath() const = 0;
};

class Marco : public PlayerSoldier {
public:
    Marco() {
        grenadeCount = 8;
        fireRateMultiplier = 1.25;
    }
    const char* getTexturePath() const override { return "Data/Characters/Marco.png"; }

    void specialMove() override {
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



        equippedWeapon->attack(spawn, dirX, getFireRateMultiplier());
        if (specialMoveActive) {
            equippedWeapon->attackDirect(spawn, -dirX);
        }
    }
};

class Tarma : public PlayerSoldier {
public:
    Tarma() {
        grenadeCount = 10;
        fireRateMultiplier = 1.0;
    }
    virtual void specialMove() override {}
    void updateSpecialMove() override {}
    const char* getTexturePath() const override { return "Data/Characters/Tarma.png"; }
};

class Eri : public PlayerSoldier {
public:
    Eri() {
        grenadeCount = 10;
        fireRateMultiplier = 0.80;
    }
    virtual void specialMove() override {}
    void updateSpecialMove() override {}
    const char* getTexturePath() const override { return "Data/Characters/Eri.png"; }
};

class Fio : public PlayerSoldier {
public:
    Fio() {
        grenadeCount = 10;
        fireRateMultiplier = 1.0f;
    }
    void specialMove() override {
        if (!specialMoveActive) {
            std::cout << "Fio's special move activate" << std::endl;
            specialMoveActive = true;
            fireRateMultiplier = 2.0f;
            specialMoveTimer.restart();
        }
    }
    void updateSpecialMove() override {
        if (specialMoveActive && specialMoveTimer.getElapsedTime().asSeconds() >= 10.0f) {
            specialMoveActive = false;
            fireRateMultiplier = 1.0f;


        }
    }
    const char* getTexturePath() const override { return "Data/Characters/Fio.png"; }
};