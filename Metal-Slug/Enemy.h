#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
	virtual ~Enemy() {}
	virtual void attack() = 0;
	virtual void die() = 0;
};

#endif
