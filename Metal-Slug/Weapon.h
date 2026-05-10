#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
#include"Projectile.h"


class Weapon : public Entity {
protected:
	float fireRate;
	float fireCooldown;
	float damage;
public:
	Weapon() : fireCooldown(0) {}
	bool canFire(float fireRateMultiplier) const { return fireCooldown >= 1.0f / (fireRate * fireRateMultiplier); }
	Sprite& getSprite() { return sprite; }
	void setPosition(float x, float y) { sprite.setPosition(x, y); }
	virtual void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) = 0;
	virtual void attackDirect(Position startPos, float dirX, float dirY) = 0;
	virtual void update(float dt) { fireCooldown += dt; }
	virtual void drawBullets(RenderWindow& window, float cameraOffset) {}
	virtual void updateBullets(float dt, Survival& level, int screenW, int screenH) = 0;
	virtual void stopFiring() {}

};


class ProjectileWeapon : public Weapon {

protected:
	StraightProjectile* bullets;
	Texture ammoTex;
	int totalAmmo;
	float projectileSpeed;
	float projectileScaleX = 1.0f;
	float projectileScaleY = 1.0f;

public:
	ProjectileWeapon(int maxP, float sp) : totalAmmo(maxP), projectileSpeed(sp), bullets(NULL){
		bullets = new StraightProjectile[totalAmmo];
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

	void drawBullets(RenderWindow& window, float cameraOffset) override {

		for (int i = 0; i < totalAmmo; i++) bullets[i].draw(window, cameraOffset);
	}
	void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) override {
		if (!canFire(fireRateMultiplier) ) {
			//std::cout << "cannot fire";
			return;
		}
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(ammoTex, startPos, projectileSpeed * dirX, projectileSpeed * dirY, (int)damage, projectileScaleX, projectileScaleY);
				fireCooldown = 0; // Reset cooldown after firing
				break;
			}
		}
	}
	void attackDirect(Position startPos, float dirX, float dirY) override { //for marco dual shooting
		for (int i = 0; i < totalAmmo; ++i) {
			if (!bullets[i].isActive()) {
				bullets[i].activate(ammoTex, startPos, projectileSpeed * dirX, projectileSpeed * dirY, (int)damage, projectileScaleX, projectileScaleY);
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
		ammoTex.loadFromFile("Data/items/bullet.png");
	}
	
	void draw(RenderWindow& window) {}
};

class MachineGun : public ProjectileWeapon {
public:
	MachineGun() : ProjectileWeapon(100, 1500.f) {
		fireRate = 8.0f;
		damage = 5;
		setProjectileScale(0.25f, 0.25f);
		ammoTex.loadFromFile("Data/items/bullet.png");
	}

	
	void draw(RenderWindow& window) {}

};

class RocketLauncher : public ProjectileWeapon {
public:
	RocketLauncher() : ProjectileWeapon(10, 800.f) {
		fireRate = 0.5f;
		damage = 20;
		setProjectileScale(0.25f, 0.25f);
		ammoTex.loadFromFile("Data/items/rocket.png");
	}

	void draw(RenderWindow& window) {}
};
class Flameshot : public Weapon {
private:
	Sprite flameSprite;
	Texture flameTexture;
	bool firing;
	float length;  // 5 blocks * 64px

public:
	Flameshot() : firing(false), length(5 * 64.0f) {
		flameTexture.loadFromFile("Data/items/flame.png");
		flameSprite.setTexture(flameTexture);
	}

	void attack(Position startPos, float dirX, float dirY, float fireRateMultiplier) override {
		firing = true;
		flameSprite.setPosition(startPos.x, startPos.y);
	}

	void stopFiring() { firing = false; }

	void attackDirect(Position startPos, float dirX, float dirY) override {}
	void updateBullets(float dt, Survival& level, int screenW, int screenH) override {}

	

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
	Texture laserTexture;
	bool firing;
	float timer;

public:
	LaserGun() : firing(false), timer(0.f) { fireRate = 1; 
	laserTexture.loadFromFile("Data/items/laser.png");
	laserSprite.setTexture(laserTexture);
	}

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


	void drawBullets(RenderWindow& window, float cameraOffset) override {
		if (!firing) return;
		laserSprite.setPosition(laserSprite.getPosition().x - cameraOffset, laserSprite.getPosition().y);
		window.draw(laserSprite);
	}
	void draw(RenderWindow& window) {}
};
//======================================GRANATAAA========================================

class FirePool: public Entity {
private:
	float radius;        //(3 blocks * 64 = 192)
	float timeLeft;
	float damageCooldown;
	float damageInterval;
	int damage;
public:
	

	FirePool() : radius(192.f), timeLeft(0.f),damageCooldown(0.f), damageInterval(0.25f),damage(3) {
		active = false;
		tex.loadFromFile("Data/items/flame.png");
		sprite.setTexture(tex);
		float diameter = radius * 2.f;
		sprite.setScale(diameter / tex.getSize().x, 1.0f);
	}

	void spawn(Position pos) {
		position = pos;
		timeLeft = 10.f;
		damageCooldown = 0.f;
		active = true;
		// centre the sprite on the explosion point
		sprite.setPosition(position.x - radius, position.y - radius);
	}
	

	
	void update(float dt) {
		if (!active) return;
		timeLeft -= dt;
		if (timeLeft <= 0.f) { active = false; return; }
		damageCooldown -= dt;
	}

	bool canDamage() {
		if (!active) return false;
		if (damageCooldown <= 0.f) {
			damageCooldown = damageInterval;
			return true;
		}
		return false;
	}
	//check if a point is within the pool radius, used for checking if enemy is standing in it
	bool contains(float px, float py) const {
		if (!active) return false;
		float dx = px - position.x;
		float dy = py - position.y;
		return (dx * dx + dy * dy) <= (radius * radius);
	}

	void draw(RenderWindow& window, float cameraOffset) {
		if (!active) return;
		sprite.setPosition(position.x - radius - cameraOffset, position.y);
		window.draw(sprite);
	}
};


struct BlastEvent {
	Position center;
	bool pending;
};

class GrenadeWeapon : public Weapon {
protected:
	const float BLAST_RADIUS;   // 3 * 64 = 192 px  
	static const int MAX_GRENADES = 10;   // max inflight at once
	BallisticProjectile grenades[MAX_GRENADES];
	Texture grenadeTex;
	float throwSpeed;
	int maxThrows;        // total grenades the character carries
	int throwsLeft;       // remaining count
public:
	GrenadeWeapon(float speed, int throws): throwSpeed(speed), maxThrows(throws), throwsLeft(throws), BLAST_RADIUS(192.0f) {
		fireRate = 1.2f;
		damage = 20;
		grenadeTex.loadFromFile("Data/items/grenade.png");
	}

	int  getThrowsLeft() const { return throwsLeft; }
	int  getMaxThrows() const { return maxThrows; }
	float getBlastRadius() const { return BLAST_RADIUS; }
	
	BlastEvent blasts[MAX_GRENADES];  

	virtual FirePool* getFirePools(int& outCount) {
		outCount = 0;
		return nullptr;   // base has no pools
	}

	void attack(Position startPos, float dirX, float dirY,
		float fireRateMultiplier) override {
		if (!canFire(fireRateMultiplier)) return;
		if (throwsLeft <= 0) return;          // out of grenades

		for (int i = 0; i < MAX_GRENADES; i++) {
			if (!grenades[i].isActive() && !grenades[i].hasExploded()) {
				grenades[i].activate(grenadeTex, startPos,throwSpeed * dirX,throwSpeed * dirY - 300.f, (int)damage, 0.15f, 0.15f);
				fireCooldown = 0;
				throwsLeft--;            
				break;
			}
		}
	}

	void attackDirect(Position startPos, float dirX, float dirY) override {}

	void updateBullets(float dt, Survival& level,
		int screenW, int screenH) override {
		Weapon::update(dt);
		for (int i = 0; i < MAX_GRENADES; i++) {
			if (!grenades[i].isActive()) continue;

			grenades[i].update(dt);

			Position pos = grenades[i].getPosition();
			float gw = grenades[i].getWidth();
			float gh = grenades[i].getHeight();

			// Hit solid terrain and explode
			if (level.isSolid(pos.x, pos.y, gw, gh)) {
				Position blast = pos;
				grenades[i].triggerExplosion();
				onExplode(blast, level); 
			}
		}
	}

	void drawBullets(RenderWindow& window, float cameraOffset) override {
		for (int i = 0; i < MAX_GRENADES; i++)
			grenades[i].draw(window, cameraOffset);
	}

	virtual void onExplode(Position blastCenter, Survival& level) = 0;

	//void processBlastDamage(Enemy** enemies, int enemyCount) {
	//	for (int i = 0; i < MAX_GRENADES; i++) {
	//		if (!grenades[i].hasExploded()) continue;
	//		Position blast = grenades[i].getPosition();					//..................E
	//		for (int e = 0; e < enemyCount; e++) {						//................/..|
	//			if (!enemies[e]) continue;								//..dx^2 + dy^2../...|
	//			float dx = enemies[e]->getPosition().x - blast.x;		//............../....| dy
	//			float dy = enemies[e]->getPosition().y - blast.y;		//............./.....|
	//			if (dx * dx + dy * dy <= BLAST_RADIUS * BLAST_RADIUS)	//..........==B==____|
	//				enemies[e]->takeDamage(damage);						//.................dx
	//		}
	//		grenades[i].clearExplosion();
	//	}
	//}
};


class HandGrenade : public GrenadeWeapon {
private:

public:
	HandGrenade(int throws = 10) : GrenadeWeapon(600.0f, throws) {
		grenadeTex.loadFromFile("Data/items/grenade.png");
	}
	void onExplode(Position blastCenter, Survival& level) {
		for (int i = 0; i < MAX_GRENADES; i++) {
			if (!blasts[i].pending) {
				blasts[i].center = blastCenter;
				blasts[i].pending = true;
				break;
			}
		}
	}
};

class FireBombGrenade : public GrenadeWeapon {
public:
	static const int MAX_POOLS = 10;
	FirePool pools[MAX_POOLS];

	FireBombGrenade(int throws = 10) : GrenadeWeapon(600.0f, throws) {
		grenadeTex.loadFromFile("Data/items/firegrenade.png");
		damage = 20;
		fireRate = 1.2f;
	}
	void updateBullets(float dt, Survival& level,int screenW, int screenH) override {
		GrenadeWeapon::updateBullets(dt, level, screenW, screenH);
		for (int p = 0; p < MAX_POOLS; p++)
			pools[p].update(dt);
	}

	void drawBullets(RenderWindow& window, float cameraOffset) override {
		GrenadeWeapon::drawBullets(window, cameraOffset);
		for (int p = 0; p < MAX_POOLS; p++)
			pools[p].draw(window, cameraOffset);
	}

	FirePool* getFirePools(int& outCount) override {
		outCount = MAX_POOLS;
		return pools;
	}

	void onExplode(Position blastCenter, Survival& level) override{
		for (int i = 0; i < MAX_GRENADES; i++) {
			if (!blasts[i].pending) {
				blasts[i].center = blastCenter;
				blasts[i].pending = true;
				break;
			}
		}

		//spawn a fire pool
		for (int p = 0; p < MAX_POOLS; p++) {
			if (!pools[p].isActive()) {
				pools[p].spawn(blastCenter);
				break;
			}
		}
	}

	/*void processFireDamage(Enemy** enemies, int enemyCount) {
		for (int p = 0; p < MAX_POOLS; p++) {
			if (!pools[p].isActive()) continue;
			if (!pools[p].canDamage()) continue;
			for (int e = 0; e < enemyCount; e++) {
				if (!enemies[e]) continue;
				float enemyX = enemies[e]->getPosition().x + enemies[e]->getWidth() / 2.f;
				float enemyY = enemies[e]->getPosition().y + enemies[e]->getHeight() / 2.f;
				if (pools[p].contains(enemyX, enemyY))
					enemies[e]->takeDamage(5);
			}
		}
	}*/
};












class WeaponFactory {
public:
	static Weapon* createPistol() { return new Pistol(); }
	static Weapon* createMachineGun() { return new MachineGun(); }
	static Weapon* createRocketLauncher() { return new RocketLauncher(); }
	static Weapon* createFlameshot() { return new Flameshot(); }
	static Weapon* createLaserGun() { return new LaserGun(); }
	static GrenadeWeapon* createHandGrenade(int throws) { return new HandGrenade(throws); }
	static GrenadeWeapon* createFireBombGrenade(int throws) { return new FireBombGrenade(throws); }
};
