#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1600;
int screen_y = 900;

// prototypes
float checkBlockCollision(char** lvl, float player_x, float player_y, int Pwidth, int Pheight, int cell_size, int grid_height);
void draw_player(RenderWindow& window, Sprite& playerSprite, float player_x, float player_y);
void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, Sprite& wallSprite2);
void handleJump(float& velocityY, bool& onGround, bool& isJumping, const float jumpStrength);

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
	const int width = 110;
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

	lvl[13][4] = 'u';
	lvl[13][5] = 'u';
	lvl[13][6] = 'u';
	lvl[13][7] = 'u';
	lvl[13][8] = 'u';

	lvl[11][0] = 'g';
	lvl[11][1] = 'g';
	lvl[11][2] = 'g';
	lvl[11][3] = 'g';
	lvl[11][4] = 'g';
	lvl[11][5] = 'g';
	lvl[11][6] = 'g';
	lvl[11][7] = 'g';
	lvl[11][8] = 'g';
	lvl[11][9] = 'g';
	lvl[11][10] = 'g';

	lvl[11][11] = 'g';
	lvl[11][12] = 'g';
	lvl[11][13] = 'g';
	lvl[11][14] = 'g';
	lvl[11][15] = 'g';
	lvl[11][16] = 'g';
	lvl[11][17] = 'g';
	lvl[11][18] = 'g';
	lvl[11][19] = 'g';
	lvl[11][20] = 'g';
	lvl[11][21] = 'g';
	lvl[11][22] = 'g';
	lvl[11][23] = 'g';
	lvl[11][24] = 'g';





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

	////////////////////////////////////////////////////////

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
		if (player_x + Pwidth > screen_x) player_x = screen_x - Pwidth;
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

		float ground_level = checkBlockCollision(lvl, player_x, player_y, Pwidth, Pheight, cell_size, height);
		if (player_y >= ground_level) {
			player_y = ground_level;
			velocityY = 0;
			onGround = true;
		}

		display_level(window, height, width, lvl, wallSprite1, cell_size, undestructWall_sprite);
		draw_player(window, playerSprite, player_x, player_y);

		window.display();
	}

	return 0;
}


float checkBlockCollision(char** lvl, float player_x, float player_y, int Pwidth, int Pheight, int cell_size, int grid_height)
{

	float player_bottom = player_y + Pheight;
	int check_row = (int)(player_bottom / cell_size);
	int left_col = (int)(player_x / cell_size);
	int right_col = (int)((player_x + Pwidth) / cell_size);

	for (int row = check_row; row < grid_height; row++)
	{
		for (int col = left_col; col <= right_col; col++)
		{
			if (col >= 0 && col < 110 && row < grid_height && lvl[row][col] == 'g') return row * cell_size - Pheight;

		}
	}
	return screen_y - Pheight;
}

// functions




void draw_player(RenderWindow& window, Sprite& playerSprite, float player_x, float player_y)
{
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}

void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, Sprite& wallSprite2)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			if (lvl[i][j] == 'g')
			{
				wallSprite1.setPosition(j * cell_size, i * cell_size);
				window.draw(wallSprite1);
			}
			if (lvl[i][j] == 'u')
			{
				wallSprite2.setPosition(j * cell_size, i * cell_size);
				window.draw(wallSprite2);
			}

		}
	}
}
