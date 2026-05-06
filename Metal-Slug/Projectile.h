#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile {
private:
	sf::Vector2f position;
	sf::Vector2f velocity;
	int damage;

public:
	Projectile(const sf::Vector2f& startPosition, const sf::Vector2f& projectileVelocity, int projectileDamage)
		: position(startPosition), velocity(projectileVelocity), damage(projectileDamage) {
	}

	void update(float deltaTime) {
		position.x += velocity.x * deltaTime;
		position.y += velocity.y * deltaTime;
	}

	const sf::Vector2f& getPosition() const { return position; }
	int getDamage() const { return damage; }
};

#endif
