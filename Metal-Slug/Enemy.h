#pragma once
#include "DamageableEntity.h"
#include "Weapon.h"
#include "Survival.h"
#include "Soldier.h"
#include <cmath>

class Enemy : virtual public DamageableEntity {
protected:
    float velocityX;
    float velocityY;
    float width;
    float height;
    bool facing_left;
    bool facing_right;
    int speed;
    float gravity;
    float terminal_velocity;
    bool onGround;
 




public:

    virtual void setWeaponTexture(const Texture& tex) {
    }
    void scaleEnemy(float multiplier) {
        width = width * multiplier;
        height = height * multiplier;



        sprite.setScale(sprite.getScale().x * multiplier, sprite.getScale().y * multiplier);
    }

    void applyGravity(float gravityMultiplier = 1.0f) {
        if (!onGround) {
            velocityY += gravity * gravityMultiplier;
            if (velocityY > terminal_velocity) {


                velocityY = terminal_velocity;
            }
        }
    }

    void applyVerticalCollision(float correctedY, float nextY) {
        if (correctedY < nextY) {
            position.y = correctedY;
            velocityY = 0;
            onGround = true;
        }
        else {


            position.y = nextY;
            onGround = false;
        }
    }




    Enemy(int sp = 3) : speed(sp), velocityX(sp), velocityY(0), width(50), height(70), gravity(1.0f), terminal_velocity(20.0f), onGround(false) {
        facing_left = false;
        facing_right = true;
     
    }

    void setEnemyTexture(const Texture& newTex) {
        this->tex = newTex;




        sprite.setTexture(this->tex);



        float imageWidth = static_cast<float>(this->tex.getSize().x);
        float imageHeight = static_cast<float>(this->tex.getSize().y);




        float scaleX = width / imageWidth;
        float scaleY = height / imageHeight;


        sprite.setScale(scaleX, scaleY);
    }
    virtual void setLandedTexture(const Texture& tex) {

    }

    void draw(RenderWindow& window) override {
        window.draw(sprite);
    }

    void randomMoveX() { position.x += velocityX; }

    void detectCollision(Survival& map) {
        float nextX = position.x + velocityX;
        float correctedX = map.horizontalCollision(position.x, position.y, velocityX, width, height);


        if (correctedX != nextX) {
            velocityX = -velocityX;
            facing_left = !facing_left;
            facing_right = !facing_right;
            position.x = correctedX;
        }


        else if (gravity > 0.0f && onGround) {

            
            float probeX = (velocityX > 0) ? (nextX + width + 5.0f) : (nextX - 5.0f);



            float floorAhead = map.verticalCollision(probeX, position.y, 1.0f, height, 0.0f);




            if (floorAhead > position.y + 15.0f) {
                velocityX = -velocityX;
                facing_left = !facing_left;
                facing_right = !facing_right;
                return;
            }
        }
        else {
            position.x = nextX;
        }
    }
    virtual void attack(float dt, float playerX) = 0;
    virtual Enemy* clone() const = 0;
    virtual ~Enemy() {}
    virtual void die() = 0;
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getVelocityY() const { return velocityY; }


    virtual void updateProjectiles(float dt, Survival& map, int screenW, int screenH) {}
    virtual void drawProjectiles(RenderWindow& window, float cameraOffset) {}
};

class Infantry : public Enemy {


protected:
    float range;
    float attackTimer;

public:
    Infantry(int hp, int sp, float r=650)
        : Enemy(sp), range(r), attackTimer(0.0f) {
    }




    bool readyToAttack(float dt, float cooldownDelay) {
        attackTimer += dt;
        if (attackTimer >= cooldownDelay) {
            attackTimer = 0.0f;
            return true;
        }



        return false;
    }
};

class Rebel : public Infantry {
private:
    Pistol pistol;

public:
    Rebel(int hp = 2, int sp = 5, float r = 650.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
       // setDebugColor(Color::Red);
    }

    void attack(float dt, float playerX) override {


        float distanceToPlayer = std::abs(position.x - playerX);
        if (distanceToPlayer > range) return;
        if (!readyToAttack(dt, 1.5f)) return;




        facing_right = (playerX > position.x);


        facing_left = !facing_right;

        Position spawn;




        spawn.x = position.x + (width / 2.0f);

        spawn.y = position.y + (height / 2.0f);
        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 0.0f, 1.0f);

    }

    void updateProjectiles(float dt, Survival& map, int screenW, int screenH) override {
        pistol.updateBullets(dt, map, screenW, screenH);
    }

    void drawProjectiles(RenderWindow& window, float cameraOffset) override {


        pistol.drawBullets(window, cameraOffset);
    }
    void die() override {}
    Enemy* clone() const override { return new Rebel(*this); }
};

class ShieldedSoldier : public Infantry {
private:
    Pistol pistol;
public:
    ShieldedSoldier(int hp = 2, int sp = 4, float r = 650.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
        //setDebugColor(Color::Blue);
    }
    void updateProjectiles(float dt, Survival& map, int screenW, int screenH) override {


        pistol.updateBullets(dt, map, screenW, screenH);


    }

    void drawProjectiles(RenderWindow& window, float cameraOffset) override {
        pistol.drawBullets(window, cameraOffset);
    }
    void attack(float dt, float playerX) override {


        float distanceToPlayer = std::abs(position.x - playerX);


        if (distanceToPlayer > range) return;
        if (!readyToAttack(dt, 1.5f)) return;

        facing_right = (playerX > position.x);
        facing_left = !facing_right;

        Position spawn;
        spawn.x = position.x + (width / 2.0f);
        spawn.y = position.y + (height / 2.0f);

        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 0.0f, 1.0f);
    }

    void die() override {}
    Enemy* clone() const override { return new ShieldedSoldier(*this); }
};

class GrenadeSoldier : public Infantry {
public:
    GrenadeSoldier(int hp = 2, int sp = 4, float r = 650.f)


        : DamageableEntity(hp), Infantry(hp, sp, r) {
    }
    void attack(float dt, float playerX) override {}
    void die() override {}
    Enemy* clone() const override { return new GrenadeSoldier(*this); }
};

class BazookaSoldier : public Infantry {
public:
    BazookaSoldier(int hp = 2, int sp = 3, float r = 350.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {

    }
    void attack(float dt, float playerX) override {}
    void die() override {}
    Enemy* clone() const override { return new BazookaSoldier(*this); }
};



class Undead :public Enemy {

public:
    Undead(int hp, int sp, float pDmg)
        : DamageableEntity(hp), Enemy(sp) {
    }

};

class Zombie : public Undead {
    bool isColliding;

public:
    Zombie(int hp = 3, int sp = 2) : DamageableEntity(hp), Undead(hp, sp, 2.0f) {

    }

    void attack(float dt, float playerX) override {

    }
    void die() override {

    }
    Enemy* clone() const override { return new Zombie(*this); }
};

class Mummy : public Undead {
    float poisonGasRadius;
    float breathTimer;

public:
    Mummy(int hp = 4, int sp = 1) : DamageableEntity(hp), Undead(hp, sp, 3.0f), poisonGasRadius(100.f), breathTimer(0.f) {
    }

    void attack(float dt, float playerX) override {
    }
    void die() override {
    }
    Enemy* clone() const override { return new Mummy(*this); }
};

class Aerial : public Infantry {

    float diveSpeed;       // Speed when swooping down
    bool isDiving;


public:
    Aerial(int hp, int sp, float altitude, int r)
        : DamageableEntity(hp), isDiving(false), Infantry(hp, sp, r) {


    }
    
    virtual void hoverAndPatrol(float dt) {
        if (!isDiving) {
           
        }
    }
    
};

class Paratrooper : public Aerial {
private:
    bool parachuteActive;
    Texture landedTex;

public:
    Paratrooper(int hp = 2, int sp = 3, float r = 650.f)
        : DamageableEntity(hp), Aerial(hp, sp, 200.f, r), parachuteActive(true) {
        gravity = 0.0f;
        velocityY = 1.5f;
    }
    void setLandedTexture(const Texture& tex) {
        landedTex = tex;
    }
    void attack(float dt, float playerX) override {
    }
    virtual void applyVerticalCollision(float correctedY, float nextY) {
        Enemy::applyVerticalCollision(correctedY, nextY); 

        if (onGround && parachuteActive) {
            parachuteActive = false;        
            setEnemyTexture(landedTex);     
            gravity = 1.0f;                
        }
    }
  

    void die() override { /* Parachute burns up, enemy falls */ }
    Enemy* clone() const override { return new Paratrooper(*this); }
};

class Alien :public Enemy {

protected:
    int shieldCapacity;
    float teleportCooldown;
    float teleportTimer;

public:
    Alien(int hp, int sp, int shields)
        : DamageableEntity(hp), Enemy(sp), shieldCapacity(shields), teleportCooldown(5.0f), teleportTimer(0.f) {
    }

    virtual void teleport(float safeStartX, float safeWidth) {
        position.x = safeStartX + (rand() % (int)safeWidth);
    }

   

};

class Martian : public Alien {
private:
    Pistol pistol;
    float velY;
    float attackTimer;
    float range;

public:
    Martian(int hp = 4, int sp = 3)
        : DamageableEntity(hp), Alien(hp, sp, 5), attackTimer(0), range(650.0f) {
  
        gravity = 0.0f; 
        velocityY= 1.5f;
    }
    bool readyToAttack(float dt, float cooldownDelay) {
        attackTimer += dt;
        
        
        if (attackTimer >= cooldownDelay) {


                attackTimer = 0.0f;  return true;


        }
        return false;
    }


    void attack(float dt, float playerX) override {
        float distanceToPlayer = std::abs(position.x - playerX);
        if (distanceToPlayer > range) return;
        if (!readyToAttack(dt, 1.5f)) return; // Shoots every 1.5 seconds

                facing_right = (playerX > position.x);
                facing_left = !facing_right;
                Position spawn;
        spawn.x = position.x + (width / 2.0f);
        spawn.y = position.y + (height / 2.0f);




        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 0.0f, 1.0f);
    }




    void updateProjectiles(float dt, Survival& map, int screenW, int screenH) override {
        pistol.updateBullets(dt, map, screenW, screenH);
    }
    void drawProjectiles(RenderWindow& window, float cameraOffset) override {
        pistol.drawBullets(window, cameraOffset);
    }

    void die() override {
    }
    Enemy* clone() const override { return new Martian(*this); }
};