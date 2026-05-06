#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;

class Level {
private:
	const int height;
	const int width;
	const int cell_size;
	const int screenHeight;
	const int screenWidth;
	char** lvl;
	

	void allocateLvl() {
		lvl = new char* [height];
		for(int i = 0; i < height; i++)
		{
			lvl[i] = new char[width];
			for (int j = 0; j < width; j++)
			{
				lvl[i][j] = '\0';
			}
		}
	}

	Texture grassTex;
	Texture stoneTex;
	Sprite grassSpr;
	Sprite stoneSpr;
	
public:
	Level(int height, int width, int cell_size, int scrHeight, int scrWidth): height(height), width(width), cell_size(cell_size),lvl(nullptr), screenHeight(scrHeight), screenWidth(scrWidth) {
		allocateLvl();
	}

	~Level() {
		for (int i = 0; i < height; i++)
		{
			delete[] lvl[i];
		}
		delete[] lvl;
	}

	void generateLevel() {
		int groundRow = height - 3;//base
		int hillPeak = height - 7;//max height of terrain
		int trenchMin = height - 2;//min height of terrain

		const int sets = 7;
		const int repLen[sets] = { 15,6,6,4,4,4,10 };	//length of each repition of blocks
		const int repSlope[sets] = { 0,-1,1,1,0,-1,0 };//flat/up/down

		int idx = 0;//current set of blocks/idx of repLen
		int progress = 0;//columns in current set

		for (int col = 0; col < width; col += 1)
		{
			groundRow += repSlope[idx];//slope of current of set
			progress += 1;
			if (progress >= repLen[idx])
				if (progress >= repLen[idx])
				{
					progress = 0;
					idx += 1;
					if (idx >= sets) idx = 0;
				}

			if (groundRow < hillPeak) groundRow = hillPeak;
			if (groundRow > trenchMin) groundRow = trenchMin;

			for (int row = groundRow; row < height; row += 1) lvl[row][col] = 'g';
		}
	}

	float verticalCollision(float player_x, float player_y, int Pwidth, int Pheight)
	{

		float player_bottom = player_y + Pheight;
		int check_row = (int)(player_bottom / cell_size);
		int left_col = (int)(player_x / cell_size);
		int right_col = (int)((player_x + Pwidth) / cell_size);

		for (int row = check_row; row < height; row++)
		{
			for (int col = left_col; col <= right_col; col++)
			{
				if (col >= 0 && col < width && row < height && lvl[row][col] == 'g') return row * cell_size - Pheight;
			}
		}
		return screenHeight - Pheight;
	}

	float horizontalCollision(float player_x, float player_y, float velocityX, int Pwidth, int Pheight)
	{
		float newX = player_x;
		if (velocityX == 0)
		{
			return newX;
		}

		int topRow = (int)(player_y / cell_size);
		int bottomRow = (int)((player_y + Pheight - 1) / cell_size);
		if (topRow < 0) topRow = 0;
		if (bottomRow >= height) bottomRow = height - 1;
		if (velocityX > 0)
		{
			int rightCol = (int)((newX + Pwidth - 1) / cell_size);




			for (int row = topRow; row <= bottomRow; row += 1)
			{
				if (rightCol >= 0 && rightCol < width && lvl[row][rightCol] == 'g')
				{
					newX = rightCol * cell_size - Pwidth;
					break;
				}
			}
		}
		else
		{
			int leftCol = (int)(newX / cell_size);
			for (int row = topRow; row <= bottomRow; row += 1)
			{
				if (leftCol >= 0 && leftCol < width && lvl[row][leftCol] == 'g')
				{
					newX = (leftCol + 1) * cell_size;
					break;
				}
			}
		}

		return newX;
	}
	bool loadTextures(const char* grassPath, const char* stonePath) {
		if (!grassTex.loadFromFile(grassPath)) return false;
		if (!stoneTex.loadFromFile(stonePath))  return false;
		grassSpr.setTexture(grassTex);
		stoneSpr.setTexture(stoneTex);
		return true;
	}
	void drawLevel(RenderWindow& window, float camera_x)
	{
		for (int i = 0; i < height; i += 1)
		{
			for (int j = 0; j < width; j += 1)
			{
				if (lvl[i][j] == 'g')
				{
					grassSpr.setPosition(j * cell_size - camera_x, i * cell_size);
					window.draw(grassSpr);
				}
				if (lvl[i][j] == 'u')
				{
					stoneSpr.setPosition(j * cell_size - camera_x, i * cell_size);
					window.draw(stoneSpr);
				}

			}
		}
	}
};