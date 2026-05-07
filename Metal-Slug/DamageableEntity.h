#pragma once
#include "Entity.h"

class DamageableEntity : public Entity {
protected:
	int hp;

public:
	DamageableEntity(int startingHp = 100) : hp(startingHp) {}
	virtual ~DamageableEntity() {}

	virtual void takeDamage(int amount) {
		hp -= amount;
		if (hp < 0) {
			hp = 0;
		}
	}

	int getHp() const {
		return hp;
	}
};
