#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "Damageable.h"

enum class PlayerState {
	Normal,
	Damaged,
	Mummy,
	Undead
};

class Player: public Damageable {
protected:
    float worldX; 
    float worldY;
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
    Player() : worldX(0), worldY(300), max_speed(20.0f), acceleration(0.5), terminal_velocity(20), gravity(1.0f), height(39), width(24), jumpStrength(-18), state(PlayerState::Normal){
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

	void update(float deltaTime) override {
		(void)deltaTime;
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
        worldX += disX;
    }

    void moveY(float disY) {
        worldY += disY;
    }
	

    float getWorldX() const { return worldX; }
    float getWorldY() const { return worldY; }
    float getScreenX(const Camera& cam) const { return worldX - cam.getCameraOffset(); }
	PlayerState getState() const { return state; }
	void setState(PlayerState newState) { state = newState; }
};

#endif
