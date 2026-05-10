#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
using namespace sf;


class Projectile : public Entity {
private:
    float velocityX;
    int damage;
    bool active;
    const Texture* texture;

public:
    Projectile() : velocityX(0), damage(1), active(false), texture(nullptr) {}

    // REVERTED: No more velY or grav
    void activate(const Texture& tex, Position startPos, float velX, int dmg, float scaleX = 1.0f, float scaleY = 1.0f) {
        texture = &tex;
        sprite.setTexture(*texture);
        sprite.setScale(scaleX, scaleY);
        position = startPos;
        velocityX = velX;
        damage = dmg;
        active = true;
        sprite.setPosition(position.x, position.y);
    }

    void update(float dt) {
        if (!active) return;
        // REVERTED: Only moves on the X axis now
        position.x += velocityX * dt;
        sprite.setPosition(position.x, position.y);
    }

    Position getPosition() const { return position; }

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