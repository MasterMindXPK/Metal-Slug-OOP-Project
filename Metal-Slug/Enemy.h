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
    RectangleShape debugShape;
    float gravity;
    float terminal_velocity;
    bool onGround;

public:


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
        debugShape.setSize(Vector2f(width, height));
        debugShape.setFillColor(Color::Red);
    }

    void draw(RenderWindow& window) override {
        debugShape.setPosition(sprite.getPosition());
        window.draw(debugShape);
    }

    void setDebugColor(Color color) { debugShape.setFillColor(color); }
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
    Infantry(int hp, int sp, float r)
        : DamageableEntity(hp), Enemy(sp), range(r), attackTimer(0.0f) {
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
    Rebel(int hp = 2, int sp = 5, float r = 200.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
        setDebugColor(Color::Red);
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
        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 1.0f);
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
    ShieldedSoldier(int hp = 2, int sp = 4, float r = 250.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
        setDebugColor(Color::Blue);
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

        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 1.0f);
    }

    void die() override {}
    Enemy* clone() const override { return new ShieldedSoldier(*this); }
};

class GrenadeSoldier : public Infantry {
public:
    GrenadeSoldier(int hp = 2, int sp = 4, float r = 250.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
        setDebugColor(Color::Green);
    }
    void attack(float dt, float playerX) override {}
    void die() override {}
    Enemy* clone() const override { return new GrenadeSoldier(*this); }
};

class BazookaSoldier : public Infantry {
public:
    BazookaSoldier(int hp = 2, int sp = 3, float r = 350.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
        setDebugColor(Color::Black);
    }
    void attack(float dt, float playerX) override {}
    void die() override {}
    Enemy* clone() const override { return new BazookaSoldier(*this); }
};