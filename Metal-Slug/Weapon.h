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
	Weapon(): fireCooldown(0){}
	bool canFire(float fireRateMultiplier) const { return fireCooldown >= 1.0f / (fireRate * fireRateMultiplier); }
	Sprite& getSprite() { return sprite; }
	void setPosition(float x, float y) { sprite.setPosition(x, y); }
	virtual void attack(Position startPos, float dirX, float fireRateMultiplier) = 0;
	virtual void attackDirect(Position startPos, float dirX) = 0;
	virtual void update(float dt) { fireCooldown += dt; }
	virtual void drawBullets(RenderWindow& window, float cameraOffset){ }
	virtual void updateBullets(float dt, Level& level, int screenW, int screenH) = 0;
	virtual void setProjectileTexture(Texture& tex) {}

};

class WeaponManager {
protected:
	
public:
	
	virtual Weapon* createWeapon() = 0;
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

	//void updateBullets(float dt) override{
	//	Weapon::update(dt); //This will update the fire cooldown
	//	for (int i = 0; i < totalAmmo; i++) bullets[i].update(dt);
	//}

	void updateBullets(float dt, Level& level, int screenW, int screenH) override {
		Weapon::update(dt);
		for (int i = 0; i < totalAmmo; i++) {
			if (!bullets[i].isActive()) continue;

			bullets[i].update(dt);

			Position pos = bullets[i].getPosition();
			float bw = bullets[i].getWidth();
			float bh = bullets[i].getHeight();

			// Out of screen bounds — deactivate
			if (pos.x + bw < 0 || pos.x > screenW * 10 ||
				pos.y < 0 || pos.y + bh > screenH) {
				bullets[i].deactivate();
				continue;
			}

			// Level collision — deactivate on tile hit
			if (level.isSolid(pos.x, pos.y, bw, bh)) {
				bullets[i].deactivate();
			}
		}
	}

	void drawBullets(RenderWindow& window, float cameraOffset) override{
		//std::cout << "weap drew" << std::endl;
		for (int i = 0; i < totalAmmo; i++) bullets[i].draw(window, cameraOffset);
	}
	void attack(Position startPos, float dirX, float fireRateMultiplier) override {
		if (!canFire(fireRateMultiplier) || !projectileTexture) {
			std::cout << "cannot fire"; return;
		}
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage);
				fireCooldown = 0; // Reset cooldown after firing
				break;
			}
		}
	}
	void attackDirect(Position startPos, float dirX) override{ //for marco dual shooting
		if (!projectileTexture) return;
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage);
				break;
			}
		}
	}

	~ProjectileWeapon() { delete[] bullets; }
};

class Pistol : public ProjectileWeapon, public WeaponManager {

public:
	Pistol() : ProjectileWeapon(50, 1000.f) {
		fireRate = 4.0f;
		damage = 5;
	}

	Weapon* createWeapon() override {
		
		Weapon* w = new Pistol();
		return w;
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