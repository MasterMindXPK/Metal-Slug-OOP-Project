#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
using namespace sf;

class Entity{
protected:
	Position position;
	bool isActive;
	Sprite sprite;
    Texture tex;

public:
	Entity() :position{ 0.0f, 0.0f }, isActive(true) {}
	virtual ~Entity() {}

	virtual void draw(RenderWindow& window) = 0;
	
	
    Position getPosition() const {
        return position;
    }

    void setPosition(const Position& pos) {
        position = pos;
        //sprite.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
    }

    void setSprite(const Texture& tex) {
        sprite.setTexture(tex);
    }
    
};
