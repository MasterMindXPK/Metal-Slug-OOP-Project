#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
#include"Projectile.h"


class Weapon {
protected: 
	float fireRate;
	float fireCooldown;
	Texture texture;
	Sprite sprite;
	float damage;
public:

	virtual void attack(Position startPos, float dirX, float fireRateMultiplier) = 0;
	virtual void update(float dt) { fireCooldown += dt; }
	bool canFire(float fireRateMultiplier) const { return fireCooldown >= 1.0f / (fireRate * fireRateMultiplier); }
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
	void setProjectileTexture(Texture& tex) { projectileTexture = &tex;	sprite.setScale(20.0f, 20.0f);}

	void updateBullets(float dt) {
		Weapon::update(dt); //This will update the fire cooldown
		for (int i = 0; i < totalAmmo; i++) bullets[i].update(dt);
	}

	void drawBullets(RenderWindow& window, float cameraOffset) {
		for (int i = 0; i < totalAmmo; i++) bullets[i].draw(window, cameraOffset);
	}
	void attack(Position startPos, float dirX, float fireRateMultiplier) override {
		if (!canFire(fireRateMultiplier) || !projectileTexture) return;
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage);
				fireCooldown = 0; // Reset cooldown after firing
				break;
			}
		}
	}

	~ProjectileWeapon() { delete[] bullets; }
};

class Pistol : public ProjectileWeapon {

public:
	Pistol() : ProjectileWeapon(10, 100.f) {
		fireRate = 4.0f;
		damage = 5;
	}
	/*void attack(Position startPos, float dirX, float fireRateMultiplier) override {
		if (!projectileTexture) return;
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage);
				break;
			}
		}
	}*/

};