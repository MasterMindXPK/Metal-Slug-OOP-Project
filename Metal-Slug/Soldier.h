#pragma once

#include "Camera.h"
#include "DamageableEntity.h"
#include "Utility.h"
using namespace sf;

enum class SoldierState { // someone saw this code jaani, 11 may full release, 10 may bug fixes, 9 may animations, 8 may more animations, 7 may more animations, 6 may more animations, 5 may more animations, 4 may more animations, 3 may more animations, 2 may more animations, 1 may more animations
	Normal,
	Damaged,
	Mummy,
	Undead
};

class Soldier : public DamageableEntity {
protected:
	const float height;
	const float width;
	const float jumpStrength;// Initial jump velocity
	const float gravity;// Gravity acceleration
	bool isJumping; // Track if jumping
	bool isMoving;
	bool facing_left;
	bool facing_right;
	bool onGround;
	float terminal_velocity;
	float max_speed;
	float velocityX = 0;
	float velocityY = 0;
	float acceleration;
	IntRect hitBox;
	SoldierState state;

public:
	Soldier() :max_speed(70.0f), acceleration(0.35), terminal_velocity(20), gravity(1.0f), height(0.2f * 438), width(0.2f * 347), jumpStrength(-18), state(SoldierState::Normal) {
		position.x = 60;
		position.y = 22;
		isJumping = false;
		isMoving = false;
		facing_left = false;
		facing_right = true;
		onGround = true;
		hitBox.height = height;
		hitBox.width = width;
		hitBox.left = 7;
		hitBox.top = 1138;
		

	}

	void handleInput(float speedMultiplier = 1.0f) {
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			if (velocityX < 0)	velocityX = 0;
			velocityX += acceleration;
			if (velocityX > max_speed) velocityX = max_speed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::A))
		{
			if (velocityX > 0)	velocityX = 0;
			velocityX -= acceleration;
			if (velocityX < -max_speed) velocityX = -max_speed;
		}
		else
		{
			velocityX = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::W) && onGround) {
			velocityY = jumpStrength; 
			onGround = false;
			isJumping = true;
		}

	}

	void draw(RenderWindow& window) override {
		window.draw(sprite);
	}

	void moveX(float disX) {
		position.x += disX;
	}

	void moveY(float disY) {
		position.y += disY;
	}

	void applyGravity(float gravityMultiplier = 1.0f) {
		if (!onGround) {
			velocityY += gravity * gravityMultiplier;
			if (velocityY > terminal_velocity) velocityY = terminal_velocity;
		}
		
	}

	bool setTextureFromFile(const char* path) {
		if (!tex.loadFromFile(path)) return false;
		setSprite(tex);

		Position texSize;
		texSize.x = tex.getSize().x;
		texSize.y = tex.getSize().y;

		float scaleX = width / static_cast<float>(texSize.x);
		float scaleY = height / static_cast<float>(texSize.y);
		sprite.setScale(scaleX, scaleY);


		// Update hitbox to match drawn size
		hitBox.width = static_cast<int>(width);
		hitBox.height = static_cast<int>(height);
		return true;
	}

	void applyHorizontalCollision(float corrected) {
		/*if (corrected != position.x)	velocityX = 0;*/

		position.x = corrected;
		velocityX = 0;
	}

	void applyVerticalCollision(float corrected, float nextY) {
		if (corrected < nextY) {  
			position.y = corrected;
			velocityY = 0;
			onGround = true;
		}
		else {
			position.y = nextY;   
			onGround = false;
		}
	}
	void setSprPos(float x, float y) {
		sprite.setPosition(x, y);
	}
	void setSpriteScale(float x, float y) { sprite.setScale(x,y); }
	Sprite getSprite() const { return sprite; }
	float getVelocityX() const { return velocityX; }
	float getVelocityY() const { return velocityY; }
	float getHeight() const { return height; }
	float getWidth() const { return width; }
    float getScreenX(const Camera& cam) const { return position.x - cam.getCameraOffset(); }
	SoldierState getState() const { return state; }
	void setState(SoldierState newState) { state = newState; }
};


