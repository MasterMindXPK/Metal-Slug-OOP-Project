#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
using namespace sf;

class Entity {
private:
	Position position;
	bool isActive;
	Sprite sprite;


public:
	
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
