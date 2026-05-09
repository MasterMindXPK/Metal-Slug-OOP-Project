#pragma once
#include "DamageableEntity.h"
#include "Weapon.h"
#include "Survival.h"
#include "Soldier.h"



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
    Enemy(int sp = 10) : speed(sp), velocityX(sp), velocityY(0), width(50), height(70),gravity(1.0f), terminal_velocity(20.0f), onGround(false) {
        facing_left = false;
        facing_right = true;

//remove later when sprite added
        debugShape.setSize(Vector2f(width, height));
        debugShape.setFillColor(Color::Red); // Default color
    }

    void draw(RenderWindow& window) override {
        // Your Game.cpp loop already calls setSprPos(), which updates the 'sprite' variable.
        // We just steal those screen coordinates and apply them to our rectangle!
        debugShape.setPosition(sprite.getPosition());
        window.draw(debugShape);
    }

    // Helper to let child classes change their color
    void setDebugColor(Color color) {
        debugShape.setFillColor(color);
    }
    void randomMoveX() {
        position.x += velocityX;
    }

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
    

    
    virtual Enemy* clone() const = 0;
    virtual ~Enemy() {}
    virtual void attack() = 0;
    virtual void die() = 0;
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getVelocityY() const { return velocityY; }
    virtual void updateProjectiles(float dt, Survival& map, int screenW, int screenH) {}
    virtual void drawProjectiles(RenderWindow& window, float cameraOffset) {}

};



class Infantry : public Enemy{

protected:
    float range;


public:

    Infantry(int hp, int sp, float r)
        : DamageableEntity(hp), Enemy(sp), range(r) {
    }



};

class Rebel : public Infantry {
private:
    Pistol pistol; 

public:
    Rebel(int hp = 2, int sp = 10, float r = 200.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {
        setDebugColor(Color::Red);
    }
    void attack() override {

        Position spawn; 
        spawn.x = position.x + (width / 2.0f);
        spawn.y = position.y + (height / 2.0f);
        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 0.0f); 
    }
    void updateProjectiles(float dt, Survival& map, int screenW, int screenH) override {
        pistol.updateBullets(dt, map, screenW, screenH);
    }

    void drawProjectiles(RenderWindow& window, float cameraOffset) override {
        pistol.drawBullets(window, cameraOffset);
    }
    void die() override {

    }
    Enemy* clone() const override {
        return new Rebel(*this); 
    }
};

class GrenadeSoldier : public Infantry {
public:
    GrenadeSoldier(int hp = 2, int sp = 8, float r = 250.f)
        : DamageableEntity(hp), Infantry(hp, sp, r) {

        setDebugColor(Color::Green);
    }

    void attack() override {

        //grenade class needed first
        // Spawn a grenade projectile that applies arc physics
        // e.g., grenade.throwArc(...);
    }

    void die() override {
        // Play Grenade Soldier death animation
    }
    Enemy* clone() const override {
        return new GrenadeSoldier(*this);
    }
};
class ShieldedSoldier : public Infantry {
    Pistol pistol;
public:
    ShieldedSoldier(int hp = 2, int sp = 8, float r = 250.f)
        : DamageableEntity(hp),  Infantry(hp, sp, r) {


        setDebugColor(Color::Blue);
    }
    void updateProjectiles(float dt, Survival& map, int screenW, int screenH) override {
        pistol.updateBullets(dt, map, screenW, screenH);
    }

    void drawProjectiles(RenderWindow& window, float cameraOffset) override {
        pistol.drawBullets(window, cameraOffset);
    }
    void attack() override {
        Position spawn;

        // Spawn from the center
        spawn.x = position.x + (width / 2.0f);
        spawn.y = position.y + (height / 2.0f);
        pistol.attack(spawn, facing_right ? 1.0f : -1.0f, 0.0f);
    }

    void die() override {
        // Play Grenade Soldier death animation
    }
    Enemy* clone() const override {
        return new ShieldedSoldier(*this);
    }
};
// --- BAZOOKA SOLDIER ---
class BazookaSoldier : public Infantry {
public:
    BazookaSoldier(int hp = 2, int sp = 6, float r = 350.f)
        : DamageableEntity(hp),Infantry(hp, sp, r) {

        setDebugColor(Color::Black);
    }

    void attack() override {
        //bazooka needed first
    }

    void die() override {}
    Enemy* clone() const override {
        return new BazookaSoldier(*this);
    }
};