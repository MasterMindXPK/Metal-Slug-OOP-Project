#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
#include"Projectile.h"


class Weapon: public Entity {
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
	virtual void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) = 0;
	virtual void attackDirect(Position startPos, float dirX, float dirY) = 0;
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
	void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) override {
		if (!canFire(fireRateMultiplier) || !projectileTexture) {
			std::cout << "cannot fire"; return;
		}
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, projectileSpeed * dirY, (int)damage, projectileScaleX, projectileScaleY);
				fireCooldown = 0; // Reset cooldown after firing
				break;
			}
		}
	}
	void attackDirect(Position startPos, float dirX, float dirY) override{ //for marco dual shooting
		if (!projectileTexture) return;
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(*projectileTexture, startPos, projectileSpeed * dirX, projectileSpeed * dirY, (int)damage, projectileScaleX, projectileScaleY);
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
		setProjectileScale(0.25f, 0.25f);
	}
	void setProjectileTexture() override {
		projectileTexture = new Texture();
		projectileTexture->loadFromFile("Data/items/bullet.png");
	}
	void draw(RenderWindow& window) {}
};

class MachineGun : public ProjectileWeapon {
public:
		MachineGun() : ProjectileWeapon(100, 1500.f) {
			fireRate = 8.0f;
			damage = 5;
			setProjectileScale(0.25f, 0.25f);
		}

		void setProjectileTexture() override {
			projectileTexture = new Texture();
			projectileTexture->loadFromFile("Data/items/bullet.png");
		}
		void draw(RenderWindow& window) {}
	
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
		void draw(RenderWindow& window) {}
};
class Flameshot : public Weapon {
private:
	Sprite flameSprite;
	Texture* flameTexture;
	bool firing;
	float length;  // 5 blocks * 64px

public:
	Flameshot() : firing(false), length(5 * 64.0f) {
		
	}

	void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) override {
		firing = true;
		flameSprite.setPosition(startPos.x, startPos.y);
	}

	void stopFiring() { firing = false; }

	void attackDirect(Position startPos, float dirX, float dirY) override {}
	void updateBullets(float dt, Survival& level, int screenW, int screenH) override {}
	
	void setProjectileTexture() override {
		flameTexture = new Texture();
		flameTexture->loadFromFile("Data/items/flame.png");
		flameSprite.setTexture(*flameTexture);
	}

	void drawBullets(RenderWindow& window, float cameraOffset) override {
		if (!firing) return;
		flameSprite.setPosition(flameSprite.getPosition().x - cameraOffset, flameSprite.getPosition().y - 10.0f);
		window.draw(flameSprite);
	}
	void draw(RenderWindow& window) {}
};


class LaserGun : public Weapon {
private:
	Sprite laserSprite;
	Texture* laserTexture;
	bool firing;
	float timer;

public:
	LaserGun() : firing(false), timer(0.f) { fireRate = 1; }

	void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) override {
		if (!firing) {
			firing = true;
			timer = fireRate;
			laserSprite.setPosition(startPos.x, startPos.y);
		}
	}

	void stopFiring() override { firing = false; }

	void attackDirect(Position startPos, float dirX, float dirY) override {}

	void updateBullets(float dt, Survival& level, int screenW, int screenH) override {
		if (!firing) return;
		timer -= dt;
		if (timer <= 0.f) stopFiring();
	}

	void setProjectileTexture() override {
		laserTexture = new Texture();
		laserTexture->loadFromFile("Data/items/laser.png");
		laserSprite.setTexture(*laserTexture);
	}

	void drawBullets(RenderWindow& window, float cameraOffset) override {
		if (!firing) return;
		laserSprite.setPosition(laserSprite.getPosition().x - cameraOffset, laserSprite.getPosition().y);
		window.draw(laserSprite);
	}
	void draw(RenderWindow& window) {}
};

class WeaponFactory {
public:
	static Weapon* createPistol() { return new Pistol(); }
	static Weapon* createMachineGun() { return new MachineGun(); }
	static Weapon* createRocketLauncher() { return new RocketLauncher(); }
	static Weapon* createFlameshot() { return new Flameshot(); }
	static Weapon* createLaserGun() { return new LaserGun(); }
};
