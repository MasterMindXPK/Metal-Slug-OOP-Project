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
    Entity(const Entity& other) {
        position = other.position;
        isActive = other.isActive;
        tex = other.tex;          
        sprite = other.sprite;   
        sprite.setTexture(tex);  
    }
	virtual void draw(RenderWindow& window) = 0;
	
	
    Position getPosition() const {
        return position;
    }
    void setSprPos(float x, float y) {
        sprite.setPosition(x, y);
    }
    void setPosition(const Position& pos) {
        position = pos;

    }

    void setSprite(const Texture& tex) {
        sprite.setTexture(tex);
    }
    
};
