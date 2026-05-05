#ifndef DAMAGEBALE_H
#define DAMAGEBALE_H

class Damageable {
protected:
	int hp;

public:
	Damageable(int startingHp = 100) : hp(startingHp) {}
	virtual ~Damageable() {}

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

#endif
