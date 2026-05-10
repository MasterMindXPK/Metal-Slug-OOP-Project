#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "Entity.h"
using namespace sf;



class Projectile : public Entity {

protected:

    float velocityX;
    float velocityY;
    int damage;


public:
    Projectile() : velocityX(0), velocityY(0), damage(0) {
        active = false;
    }
    virtual void activate(Texture itex, Position startPos, float velX, float velY, int dmg, float scaleX = 1.0f, float scaleY = 1.0f) = 0;

    void update(float dt) {
        if (!active) return;
        position.x += velocityX * dt;
        position.y += velocityY * dt;
        sprite.setPosition(position.x, position.y);
    }

    void draw(RenderWindow& window) override {  //we dont use this fn anywhere, its just here to override the pure virtual fn in Entity, we use the other draw fn with camera offset instead
        if (!active) return;
        window.draw(sprite);
    }

    void draw(RenderWindow& window, float cameraOffset) {
        if (!active) return;




        sprite.setPosition(position.x - cameraOffset, position.y);
        window.draw(sprite);
    }

    void deactivate() { active = false; }
    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;
};

class StraightProjectile : public Projectile {
public:
    void activate(Texture itex, Position startPos, float velX, float velY, int dmg, float scaleX = 1.0f, float scaleY = 1.0f) {
        tex = itex;
        sprite.setTexture(tex);
        sprite.setScale(scaleX, scaleY);
        position = startPos;
        velocityX = velX;
        velocityY = velY;
        damage = dmg;
        active = true;
        sprite.setPosition(position.x, position.y);
    }

    float getWidth() const override { return 123.0f; }
    float getHeight() const override { return 12.0f; }
};

class BallisticProjectile : public Projectile {
private:
    float gravity;
    bool exploded;

public:
    BallisticProjectile() : gravity(600.f), exploded(false) {}

    void activate(Texture itex, Position startPos, float velX, float velY,
        int dmg, float scaleX = 1.f, float scaleY = 1.f) override {
        tex = itex;
        sprite.setTexture(tex);
        sprite.setScale(scaleX, scaleY);
        position = startPos;
        velocityX = velX;
        velocityY = velY;
        damage = dmg;
        exploded = false;
        active = true;
        sprite.setPosition(position.x, position.y);
    }

    void update(float dt) {
        if (!active) return;
        velocityY += gravity * dt;
        position.x += velocityX * dt;
        position.y += velocityY * dt;
        sprite.setPosition(position.x, position.y);
    }

    void triggerExplosion() {
        exploded = true;
        active = false;
    }

    void clearExplosion() { exploded = false; }

    int  getDamage()     const { return damage; }
    bool hasExploded() const { return exploded; }
    float getHeight() const override { return 24.f; }
    float getWidth()  const override { return 24.f; }
};