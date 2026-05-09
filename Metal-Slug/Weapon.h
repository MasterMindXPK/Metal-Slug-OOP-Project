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
	virtual void updateBullets(float dt, Survival& level, int screenW, int screenH) = 0;
	virtual void setProjectileTexture() {}
	virtual void stopFiring() {}

};


class ProjectileWeapon: public Weapon{

protected:
	Projectile* bullets;
	int totalAmmo;
	float projectileSpeed;
	Texture* projectileTexture;
	float projectileScaleX = 1.0f;
	float projectileScaleY = 1.0f;

public:
	ProjectileWeapon(int maxP, float sp) : totalAmmo(maxP), projectileSpeed(sp), bullets(NULL), projectileTexture(nullptr) {
		bullets = new Projectile[totalAmmo];
	}
	//void setProjectileTexture(Texture& tex) { projectileTexture = &tex;	sprite.setScale(20.0f, 20.0f);}

	//void updateBullets(float dt) override{
	//	Weapon::update(dt); //This will update the fire cooldown
	//	for (int i = 0; i < totalAmmo; i++) bullets[i].update(dt);
	//}

	void updateBullets(float dt, Survival& level, int screenW, int screenH) override {
		Weapon::update(dt);
		for (int i = 0; i < totalAmmo; i++) {
			if (!bullets[i].isActive()) continue;

			bullets[i].update(dt);

			Position pos = bullets[i].getPosition();
			float bw = bullets[i].getWidth();
			float bh = bullets[i].getHeight();

			// Out of screen bounds
			if (pos.x + bw < 0 || pos.x > screenW * 10 ||
				pos.y < 0 || pos.y + bh > screenH) {
				bullets[i].deactivate();
				continue;
			}

			// Level collision
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
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage, projectileScaleX, projectileScaleY);
				fireCooldown = 0; // Reset cooldown after firing
				break;
			}
		}
	}
	void attackDirect(Position startPos, float dirX) override{ //for marco dual shooting
		if (!projectileTexture) return;
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, 0.0f, (int)damage, projectileScaleX, projectileScaleY);
				break;
			}
		}
	}
	void setProjectileScale(float scaleX, float scaleY) {
		projectileScaleX = scaleX;
		projectileScaleY = scaleY;
	}
	~ProjectileWeapon() { delete[] bullets; }
};

class Pistol : public ProjectileWeapon {

public:
	Pistol() : ProjectileWeapon(50, 1000.f) {
		fireRate = 4.0f;
		damage = 5;
		setProjectileScale(20.0f, 20.0f);
	}
	void setProjectileTexture() override {
		projectileTexture = new Texture();
		projectileTexture->loadFromFile("Data/items/bullet.png");
	}
};

class MachineGun : public ProjectileWeapon {
public:
		MachineGun() : ProjectileWeapon(100, 1500.f) {
			fireRate = 8.0f;
			damage = 5;
			setProjectileScale(20.0f, 20.0f);
		}

		void setProjectileTexture() override {
			projectileTexture = new Texture();
			projectileTexture->loadFromFile("Data/items/bullet.png");
		}

	
};

class RocketLauncher : public ProjectileWeapon {
public:
		RocketLauncher() : ProjectileWeapon(10, 800.f) {
		fireRate = 0.5f;
		damage = 20;
		setProjectileScale(0.25f, 0.25f);
		}

		void setProjectileTexture() override {
			projectileTexture = new Texture();
			projectileTexture->loadFromFile("Data/items/rocket.png");
		}
};
class Flameshot : public Weapon {
private:
	RectangleShape flameShape;
	bool firing;
	float length;  // 5 blocks * 64px

public:
	Flameshot() : firing(false), length(5 * 64.0f) {
		flameShape.setSize(Vector2f(length, 20.0f));
		flameShape.setFillColor(Color(255, 100, 0, 200));
	}

	void attack(Position startPos, float dirX, float fireRateMultiplier) override {
		firing = true;
		flameShape.setPosition(
			dirX > 0 ? startPos.x : startPos.x - length,
			startPos.y
		);
	}

	void stopFiring() { firing = false; }

	void attackDirect(Position startPos, float dirX) override {}
	void updateBullets(float dt, Survival& level, int screenW, int screenH) override {}
	void setProjectileTexture() override {}

	void drawBullets(RenderWindow& window, float cameraOffset) override {
		if (!firing) return;
		RectangleShape temp = flameShape;
		temp.setPosition(flameShape.getPosition().x - cameraOffset,
			flameShape.getPosition().y);
		window.draw(temp);
	}
};

class LaserGun : public Weapon {
private:
	RectangleShape laserShape;
	bool firing;

public:
	LaserGun() : firing(false) {
		laserShape.setSize(Vector2f(0.f, 4.f));  // width set dynamically
		laserShape.setFillColor(Color(255, 0, 0, 230));
	}

	void attack(Position startPos, float dirX, float fireRateMultiplier) override {
		firing = true;
		float width = dirX > 0 ? 1600.f - startPos.x : startPos.x;
		laserShape.setSize(Vector2f(width, 4.f));
		laserShape.setPosition(
			dirX > 0 ? startPos.x : 0.f,
			startPos.y
		);
	}

	void stopFiring() override { firing = false; }

	void attackDirect(Position startPos, float dirX) override {}
	void updateBullets(float dt, Survival& level, int screenW, int screenH) override {}
	void setProjectileTexture() override {}

	void drawBullets(RenderWindow& window, float cameraOffset) override {
		if (!firing) return;
		window.draw(laserShape);
	}
};

class WeaponFactory {
public:
	static Weapon* createPistol() { return new Pistol(); }
	static Weapon* createMachineGun() { return new MachineGun(); }
	static Weapon* createRocketLauncher() { return new RocketLauncher(); }
	static Weapon* createFlameshot() { return new Flameshot(); }
	static Weapon* createLaserGun() { return new LaserGun(); }
};
