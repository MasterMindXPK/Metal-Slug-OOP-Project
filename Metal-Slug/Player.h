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

class Player : public Damageable {
protected:
	float worldX;
	float worldY;
	const int height;
	const int width;
	const float jumpStrength;
	const float gravity;
	bool isJumping;
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
	Player() : worldX(100), worldY(300), max_speed(20.0f), acceleration(0.5), terminal_velocity(20), gravity(1.0f), height(39), width(24), jumpStrength(-18), state(PlayerState::Normal) {
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
		// Sync position struct with worldX/worldY
		position.x = worldX;
		position.y = worldY;
		sprite.setPosition(worldX, worldY);
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

	float getSpriteWidth() const {
		return width;
	}
	float getWorldX() const { return worldX; }
	float getWorldY() const { return worldY; }
	float getScreenX(const Camera& cam) const { return worldX - cam.getCameraOffset(); }
	float getSpriteHeight() const { return height; }
	float getVelocityX() const { return velocityX; }
	float getVelocityY() const { return velocityY; }
	PlayerState getState() const { return state; }
	void setState(PlayerState newState) { state = newState; }
	void setTexture(const sf::Texture& tex) {
		sprite.setTexture(tex);
	}

	void applyGravity() {
		velocityY += gravity;
		if (velocityY > terminal_velocity) velocityY = terminal_velocity;
	}

	void applyHorizontalMovement() {
		worldX += velocityX;
	}
	void setScale(float scaleX, float scaleY) {
		sprite.setScale(scaleX, scaleY);
	}
	void resolveHorizontal(float corr) {
		worldX = corr;
	}

	void resolveVertical(float corr) {
		worldY = corr;
		velocityY = 0;
		onGround = true;
	}

	void handleInput() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			velocityX += acceleration;
			if (velocityX > max_speed) velocityX = max_speed;
			facing_right = true;
			facing_left = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			velocityX -= acceleration;
			if (velocityX < -max_speed) velocityX = -max_speed;
			facing_left = true;
			facing_right = false;
		}
		else {
			velocityX = 0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && onGround) {
			velocityY = jumpStrength;
			onGround = false;
			isJumping = true;
		}
	}
};

#endif