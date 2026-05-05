#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
protected:
	int damage;

public:
	Weapon(int weaponDamage) : damage(weaponDamage) {}
	virtual ~Weapon() {}

	virtual void use() = 0;
	int getDamage() const { return damage; }
};

class Pistol : public Weapon {
public:
	Pistol() : Weapon(10) {}
	void use() override {}
};

class Knife : public Weapon {
public:
	Knife() : Weapon(5) {}
	void use() override {}
};

class HeavyMachineGun : public Weapon {
public:
	HeavyMachineGun() : Weapon(15) {}
	void use() override {}
};

class Rocket : public Weapon {
public:
	Rocket() : Weapon(40) {}
	void use() override {}
};

class Grenade : public Weapon {
public:
	Grenade() : Weapon(30) {}
	void use() override {}
};

#endif
