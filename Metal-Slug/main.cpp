#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Camera.h"

using namespace sf;
using namespace std;

int screen_x = 1600;
int screen_y = 900;

// prototypes
float checkBlockCollision(char** lvl, float player_x, float player_y, int Pwidth, int Pheight, int cell_size, int grid_height, int grid_width);
void draw_player(RenderWindow& window, Sprite& playerSprite, float player_x, float player_y, float camera_x);
void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, Sprite& wallSprite2, float camera_x);
void handleJump(float& velocityY, bool& onGround, bool& isJumping, const float jumpStrength);
void load_level(char** lvl, int height, int width);

int main()
{
	RenderWindow window(VideoMode(screen_x, screen_y), "Metal Slug", Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	/////////////////////////////////////////////////////////////////
	// a cell is 64 by 64 pixels
	// 'g' = grass tile

	const int cell_size = 64;
	const int height = 14;
	const int width = 350;
	const float jumpStrength = -18; // Initial jump velocity
	const float gravity = 1;  // Gravity acceleration
	bool isJumping = false;  // Track if jumping
	bool isMoving = false;
	bool facing_left = true;
	bool facing_right = false;
	float terminal_Velocity = 20;
	bool onGround = false;
	char** lvl = NULL;
	float max_speed = 5;
	float velocityX = 0;
	float velocityY = 0;
	float acceleration = 0.5;
	lvl = new char* [height];
	float player_x = 320;
	float player_y = 610;

	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width] {'\0'};
	}

	load_level(lvl, height, width);





	//normal block
	Texture wallTex1;
	Sprite wallSprite1;
	//the indestructible blocks at the bottom that will always exist/undestructible
	Texture undestructWall_texture;
	Sprite undestructWall_sprite;


	wallTex1.loadFromFile("Data/blocks/grass_block_side.png");
	wallSprite1.setTexture(wallTex1);

	undestructWall_texture.loadFromFile("Data/blocks/stone.png");
	undestructWall_sprite.setTexture(undestructWall_texture);


	////////////////////////////////////////////////////////

	float scale_x = 0.2;
	float scale_y = 0.2;

	int raw_img_x = 593;
	int raw_img_y = 470;

	int Pheight = raw_img_y * scale_y;	// 94
	int Pwidth = raw_img_x * scale_x;	// 119

	Texture playerTex;
	Sprite playerSprite;

	playerTex.loadFromFile("Data/Characters/Character.png");
	playerSprite.setTexture(playerTex);
	playerSprite.setScale(scale_x, scale_y);

	Texture backgroundTex;
	Sprite backgroundSprite;
	backgroundTex.loadFromFile("Data/Backgrounds/Bg1.jpg");
	backgroundSprite.setTexture(backgroundTex);
	unsigned int bgWidth = backgroundTex.getSize().x;
	unsigned int bgHeight = backgroundTex.getSize().y;
	if (bgWidth > 0 && bgHeight > 0)
	{
		backgroundSprite.setScale(
			static_cast<float>(screen_x) / bgWidth,
			static_cast<float>(screen_y) / bgHeight);
	}

	////////////////////////////////////////////////////////

	Camera camera(screen_x, screen_y);
	const float cameraMinX = 100.0f;
	const float cameraMaxX = screen_x - 100.0f;
	Event ev;
	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close();
			}

			if (ev.type == Event::KeyPressed)
			{
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			velocityX += acceleration;
			if (velocityX > max_speed) velocityX = max_speed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			velocityX -= acceleration;
			if (velocityX < -max_speed) velocityX = -max_speed;
		}
		else
		{
			velocityX = 0;
		}

		player_x += velocityX;
		if (player_x < 0) player_x = 0;
		camera.update(player_x, cameraMinX, cameraMaxX);
		float camera_x = camera.getXOffset();
		window.clear();

		if (Keyboard::isKeyPressed(Keyboard::Up) && (onGround)) {
			velocityY = jumpStrength;
			onGround = false;
			isJumping = true;
		}
		else {
			isJumping = false;
		}

		velocityY += gravity;
		if (velocityY > terminal_Velocity) velocityY = terminal_Velocity;
		player_y += velocityY;

		float ground_level = checkBlockCollision(lvl, player_x, player_y, Pwidth, Pheight, cell_size, height, width);
		if (player_y >= ground_level) {
			player_y = ground_level;
			velocityY = 0;
			onGround = true;
		}

		window.draw(backgroundSprite);
		display_level(window, height, width, lvl, wallSprite1, cell_size, undestructWall_sprite, camera_x);
		draw_player(window, playerSprite, player_x, player_y, camera_x);

		window.display();
	}

	return 0;
}


float checkBlockCollision(char** lvl, float player_x, float player_y, int Pwidth, int Pheight, int cell_size, int grid_height, int grid_width)
{

	float player_bottom = player_y + Pheight;
	int check_row = (int)(player_bottom / cell_size);
	int left_col = (int)(player_x / cell_size);
	int right_col = (int)((player_x + Pwidth) / cell_size);

	for (int row = check_row; row < grid_height; row++)
	{
		for (int col = left_col; col <= right_col; col++)
		{
			if (col >= 0 && col < grid_width && row < grid_height && lvl[row][col] == 'g') return row * cell_size - Pheight;

		}
	}
	return screen_y - Pheight;
}

// functions




void draw_player(RenderWindow& window, Sprite& playerSprite, float player_x, float player_y, float camera_x)
{
	playerSprite.setPosition(player_x - camera_x, player_y);
	window.draw(playerSprite);
}

void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, Sprite& wallSprite2, float camera_x)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			if (lvl[i][j] == 'g')
			{
				wallSprite1.setPosition(j * cell_size - camera_x, i * cell_size);
				window.draw(wallSprite1);
			}
			if (lvl[i][j] == 'u')
			{
				wallSprite2.setPosition(j * cell_size - camera_x, i * cell_size);
				window.draw(wallSprite2);
			}

		}
	}
}

void load_level(char** lvl, int height, int width)
{
	for (int row = 0; row < height; row += 1)
	{
		for (int col = 0; col < width; col += 1)
		{
			lvl[row][col] = '\0';
		}
	}

	int groundRow = height - 3;
	int minGroundRow = height - 7;
	int maxGroundRow = height - 2;

	const int segmentCount = 7;
	const int segmentLength[segmentCount] = { 30, 6, 6, 4, 4, 4, 10 };
	const int segmentSlope[segmentCount] = { 0, -1, 1, 1, 0, -1, 0 };
	int segmentIndex = 0;
	int segmentProgress = 0;

	for (int col = 0; col < width; col += 1)
	{
		groundRow += segmentSlope[segmentIndex];
		segmentProgress += 1;
		if (segmentProgress >= segmentLength[segmentIndex])
		{
			segmentProgress = 0;
			segmentIndex += 1;
			if (segmentIndex >= segmentCount)
			{
				segmentIndex = 0;
			}
		}

		if (groundRow < minGroundRow) groundRow = minGroundRow;
		if (groundRow > maxGroundRow) groundRow = maxGroundRow;

		for (int row = groundRow; row < height; row += 1)
		{
			lvl[row][col] = 'g';
		}
	}
}
