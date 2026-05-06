#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Damageable.h"
#include "Utility.h"
using namespace sf;

class Entity {
protected:
    Position position;
    bool isActive;
    Sprite sprite;

public:
    Entity() :position{ 0.0f, 0.0f }, isActive(true) {}
    virtual ~Entity() {}

    virtual void update(float deltaTime) = 0;
    virtual void draw(RenderWindow& window) = 0;
    virtual void takeDamage(int amount) = 0;

    Position getPosition() const {
        return position;
    }

    void setPosition(const Position& pos) {
        position = pos;
        sprite.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
    }

    void setSprite(const Texture& tex) {
        sprite.setTexture(tex);
    }
};

#endif
