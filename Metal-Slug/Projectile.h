#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
#include "Utility.h"


class Projectile: public Entity{

private:
   
    float velocityX;
    float velocityY;
    int damage;
    bool active;
    const Texture* texture;
    

public:
    Projectile(): velocityX(0),velocityY(0),
        damage(1), active(false), texture(nullptr) {
    }

   

    void activate(const Texture& tex, Position startPos, float velX, float velY, int dmg) {
        texture = &tex;
        sprite.setTexture(*texture);
        //sprite.setScale(20.0f, 20.0f);
        position = startPos;
        velocityX= velX;
        velocityY = velY;
        damage = dmg;
        active = true;
        sprite.setPosition(position.x, position.y);
    }

    void update(float dt) {
        if (!active) return;
        position.x += velocityX * dt;
        position.y += velocityY * dt;
        sprite.setPosition(position.x, position.y);
    }

    void draw(RenderWindow& window) override {
        if (!active) return;
        window.draw(sprite);
    }

    void draw(RenderWindow& window, float cameraOffset) {
        if (!active) return;
        sprite.setPosition(position.x - cameraOffset, position.y);
        window.draw(sprite);
    }

    void deactivate() { active = false; }
    bool isActive() const { return active; }
    float getWidth() const { return 123.0f; }
    float getHeight() const { return 12.0f; }
};