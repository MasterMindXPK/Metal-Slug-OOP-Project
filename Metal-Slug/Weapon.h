#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
#include"Projectile.h"


class Weapon {
protected: 
	Texture texture;
	Sprite sprite;
	float damage;
public:
	Weapon() : damage(0.0f) {}

	virtual void attack() {};
	virtual void update(float dt) {}
	Sprite& getSprite() { return sprite; }
	void setPosition(float x, float y) { sprite.setPosition(x, y); }


};

class ProjectileWeapon: public Weapon{

protected:
	Projectile* bullets;
	int totalAmmo;
	float projectileSpeed;
	Texture* projectileTexture;

public:
	ProjectileWeapon(int maxP, float sp) : totalAmmo(maxP), projectileSpeed(sp), bullets(NULL), projectileTexture(nullptr) {
		bullets = new Projectile[totalAmmo];
	}
	void setProjectileTexture(Texture& tex) { projectileTexture = &tex; }

	void updateBullets(float dt) {
		for (int i = 0; i < totalAmmo; i++) bullets[i].update(dt);
	}

	void drawBullets(RenderWindow& window, float cameraOffset) {
		for (int i = 0; i < totalAmmo; i++) bullets[i].draw(window, cameraOffset);
	}
	void attack (Position startPos, float dirX)  {
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage);
				sprite.setScale(20.0f, 20.0f);
				break;
			}
		}
	}

	~ProjectileWeapon() { delete[] bullets; }
};

class Pistol : public ProjectileWeapon {

public:
	Pistol() : ProjectileWeapon(100000, 100.f) {
		damage = 5;
	}
	void attack(Position startPos, float dirX) {
		if (!projectileTexture) return;
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage);
				break;
			}
		}
	}

};