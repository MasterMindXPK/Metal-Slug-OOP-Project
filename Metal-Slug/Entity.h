#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
using namespace sf;

class Entity{
protected:
	Position position;
	bool active;
	Sprite sprite;
    Texture tex;

public:
	Entity() :position{ 0.0f, 0.0f }, active(true) {}
	virtual ~Entity() {}

    virtual void draw(RenderWindow& window) {};
	
	
    Position getPosition() const {
        return position;
    }
    void setSprPos(float x, float y) {
        sprite.setPosition(x, y);
    }
    void setPosition(const Position& pos) {
        position = pos;
        //sprite.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
    }

    void setSprite(const Texture& tex) {
        sprite.setTexture(tex);
    }

	bool isActive() const { return active; }
};
