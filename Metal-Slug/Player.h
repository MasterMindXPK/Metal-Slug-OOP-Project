#pragma once

#include "Camera.h"
#include "Damageable.h"
using namespace sf;

enum class PlayerState {
	Normal,
	Damaged,
	Mummy,
	Undead
};

class Player : public Damageable {
protected:
	const int height;
	const int width;
	const float jumpStrength; // Initial jump velocity
	const float gravity;  // Gravity acceleration
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
	PlayerState state;

public:
	Player() :max_speed(10.0f), acceleration(0.2), terminal_velocity(20), gravity(1.0f), height(94), width(118.6), jumpStrength(-18), state(PlayerState::Normal) {
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
		tex.loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");
		sprite.setTexture(tex);
		sprite.setScale(0.2f, 0.2f);

	}

	void handleInput() {
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			velocityX += acceleration;
			if (velocityX > max_speed) velocityX = max_speed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::A))
		{
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

	

	void takeDamage(int amount) override {
		hp -= amount;
		if (hp < 0) {
			hp = 0;
		}
		state = PlayerState::Damaged;
	}

	void moveX(float disX) {
		position.x += disX;
	}

	void moveY(float disY) {
		position.y += disY;
	}

	void applyGravity() {
		if (!onGround) {
			velocityY += gravity;
			if (velocityY > terminal_velocity) velocityY = terminal_velocity;
		}
		
	}

	void applyHorizontalCollision(float corrected) {
		/*if (corrected != position.x)	velocityX = 0;*/

		position.x = corrected;
	}

	void applyVerticalCollision(float corrected) {
		if (corrected < position.y) {  // collision pushed player up
			position.y = corrected;
			velocityY = 0;
			onGround = true;
		}
		else {
			onGround = false;  // no ground beneath
		}
	}
	void setSprPos(float x, float y) {
		sprite.setPosition(x, y);
	}
	void setSpriteScale(float x, float y) { sprite.setScale(x,y); }
	void setSpriteTexture(Texture t) { sprite.setTexture(t); }
	Sprite getSprite() const { return sprite; }
	float getVelocityX() const { return velocityX; }
	float getVelocityY() const { return velocityY; }
	float getHeight() const { return height; }
	float getWidth() const { return width; }
    float getScreenX(const Camera& cam) const { return position.x - cam.getCameraOffset(); }
	PlayerState getState() const { return state; }
	void setState(PlayerState newState) { state = newState; }
};


