#pragma once
#include "DamageableEntity.h"
#include "Weapon.h"



class Enemy : virtual public DamageableEntity {
protected:

    int speed;
    virtual void die() = 0;

public:
    Enemy(int sp = 10) : speed(sp) {}
};



class Infantry : public Enemy{

protected:
    float range;

public:
    Infantry(int hp, int sp, float r)
        : DamageableEntity(hp), Enemy(sp), Soldier(), range(r) {
    }
};

class Rebel : public Infantry {
    Pistol pistol;

public:
    Rebel(int hp = 2, int sp = 10, float r = 200.f)
        : DamageableEntity(hp), Enemy(sp), Soldier(), Infantry(hp, sp, r) {
    }

    void die() override {}
};

class Shielded : public Infantry {
    Pistol pistol;

public:
    Shielded(int hp = 5, int sp = 7, float r = 200.f)
        : DamageableEntity(hp), Enemy(sp), Soldier(), Infantry(hp, sp, r) {
    }

    void die() override {}
};

class GrenadeSoldier : public Infantry {
    Pistol pistol;

public:
    GrenadeSoldier(int hp = 2, int sp = 8, float r = 250.f)
        : DamageableEntity(hp), Enemy(sp), Soldier(), Infantry(hp, sp, r) {
    }

    void die() override {}
};

class BazookaSoldier : public Infantry {
    Pistol pistol;

public:
    BazookaSoldier(int hp = 2, int sp = 6, float r = 350.f)
        : DamageableEntity(hp), Enemy(sp), Soldier(), Infantry(hp, sp, r) {
    }

    void die() override {}
};