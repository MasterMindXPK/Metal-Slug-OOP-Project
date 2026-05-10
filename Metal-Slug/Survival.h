#pragma once
#include<SFML/Graphics.hpp>
#include <cstdlib>
#include<ctime>

using namespace sf;


class Survival {
private:
	const int height;
	const int width;
	const int cell_size;
	const int screenHeight;
	const int screenWidth;
	char** lvl;


	Texture grassTex;
	Texture aquaTex;
	Texture stoneTex;
	Sprite grassSpr;
	Sprite aquaSpr;
	Sprite stoneSpr;


	

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


public:
	Survival(int height, int width, int cell_size, int scrHeight, int scrWidth): height(height), width(width), cell_size(cell_size),lvl(nullptr), screenHeight(scrHeight), screenWidth(scrWidth) {
		allocateLvl();
	}

	~Survival() {
		for (int i = 0; i < height; i++)
		{
			delete[] lvl[i];
		}
		delete[] lvl;
	}

	bool hasReachedEnd(float playerX, int cellSize) const {
		int col = static_cast<int>(playerX / cellSize);
		return col >= (width - 1);
	}


	void generateAquaticSeg(int startCol, int endCol, int& groundRow) {
		int hillPeak = height - (4 + rand() % 2);   
		int trenchMin = height - (2 + rand() % 2);  
		int progress = 0;




		for (int col = startCol; col < endCol; ++col) {
			progress++;
			if (progress >= 6)  progress = 0;



			if (groundRow < hillPeak) groundRow = hillPeak;
			if (groundRow > trenchMin) groundRow = trenchMin;

			for (int row = groundRow; row < height; ++row)  lvl[row][col] = 'a';
			
		}
	}
	
	void generatePeakSeg(int startCol, int endCol, int& groundRow) {
		int hillPeak = height - (6 + rand() % 4);  // higher
		int trenchMin = height - (4 + rand() % 3);
		int idx = 0, progress = 0;



		int slopePattern[4] = { -1, 1, -1, 0 };
	

		for (int col = startCol; col < endCol; ++col) {
			groundRow += slopePattern[idx];
			progress++;
			if (progress >= 6) { progress = 0; 
			idx = (idx + 1) % 4; 
			}



			if (groundRow < hillPeak) groundRow = hillPeak;
			if (groundRow > trenchMin) groundRow = trenchMin;

			for (int row = groundRow; row < height; ++row) {
				lvl[row][col] = 'g';
			}
		}
	}

	void generatePlainSeg(int startCol, int endCol, int& groundRow) {
		int hillPeak = height - (4 + rand() % 2);   
		int trenchMin = height - (3 + rand() % 2);

		int slopePattern[4] = { 0, 1, 0, -1 };
		int idx = 0;
		int progress = 0;

		for (int col = startCol; col < endCol; ++col) {
			groundRow += slopePattern[idx];
			progress++;
			if (progress >= 6) { progress = 0; 
			idx = (idx + 1) % 4; }



			if (groundRow < hillPeak) groundRow = hillPeak;
			if (groundRow > trenchMin) groundRow = trenchMin;



			for (int row = groundRow; row < height; ++row) {
				lvl[row][col] = 'g';
			}
		}
	}



	void generateSurvival() {
		int groundRow = height - 3;

		const int segments=14;
		const int segLen = 25;

		int count;
		for (int seg = 0; seg < segments; ++seg) {
			
			count = rand() % 3;

			int startCol = seg * segLen;
			int endCol = (seg + 1) * segLen;

			if (count==0) {
				generateAquaticSeg(startCol, endCol, groundRow);
			}
			else if (count == 1) {
				generatePeakSeg(startCol, endCol, groundRow);
			}
			else {
				generatePlainSeg(startCol, endCol, groundRow);
			}
		}

		for (int col = 0; col < width; ++col) {
			lvl[height - 1][col] = 's';
		}
	}


	float verticalCollision(float Soldier_x, float Soldier_y, float Pwidth, float Pheight, float velocityY)
	{
		float Soldier_bottom = Soldier_y + velocityY + Pheight;
		int check_row = (int)(Soldier_bottom / cell_size);
		int left_col = (int)(Soldier_x / cell_size);
		int right_col = (int)((Soldier_x + Pwidth) / cell_size);

		for (int row = check_row; row < height; row++)
		{
			for (int col = left_col; col <= right_col; col++)
			{
				if (col >= 0 && col < width && row < height && lvl[row][col] == 'g' || col >= 0 && col < width && row < height && lvl[row][col] == 's') 
					return row * cell_size - Pheight;
				
			}
		}
		return screenHeight - Pheight;
	}

	float horizontalCollision(float Soldier_x, float Soldier_y, float velocityX, float Pwidth, float Pheight)
	{
		float newX = Soldier_x+velocityX;
		if (velocityX == 0)
		{
			return newX;
		}

		int topRow = (int)(Soldier_y / cell_size);
		int bottomRow = (int)((Soldier_y + Pheight - 1) / cell_size);
		if (topRow < 0) topRow = 0;
		if (bottomRow >= height) bottomRow = height - 1;
		if (velocityX > 0)
		{
			int rightCol = (int)((newX + Pwidth - 1) / cell_size);




			for (int row = topRow; row <= bottomRow; row += 1)
			{
				if (rightCol >= 0 && rightCol < width && lvl[row][rightCol] == 'g' || rightCol >= 0 && rightCol < width && lvl[row][rightCol] == 's')
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
				if (leftCol >= 0 && leftCol < width && lvl[row][leftCol] == 'g' || leftCol >= 0 && leftCol < width && lvl[row][leftCol] == 's')
				{
					newX = (leftCol + 1) * cell_size;
					break;
				}
			}
		}

		return newX;
	}
	bool loadTextures(const char* grassPath, const char* aquaPath, const char* stonePath) {
		if (!grassTex.loadFromFile(grassPath)) return false;
		if (!aquaTex.loadFromFile(aquaPath))  return false;
		if (!stoneTex.loadFromFile(stonePath))  return false;
		grassSpr.setTexture(grassTex);
		aquaSpr.setTexture(aquaTex);
		stoneSpr.setTexture(stoneTex);
		return true;
	}

	bool isWaterAt(float x, float y) const {
		int col = static_cast<int>(x / cell_size);
		int row = static_cast<int>(y / cell_size);
		if (row < 0 || row >= height || col < 0 || col >= width) return false;
		return lvl[row][col] == 'a';
	}


	void drawsurvival(RenderWindow& window, float camera_x)
	{
		for (int i = 0; i < height; i ++)
		{
			for (int j = 0; j < width; j ++)
			{
				if (lvl[i][j] == 'g')
				{
					grassSpr.setPosition(j * cell_size - camera_x, i * cell_size);
					window.draw(grassSpr);
				}
				else if (lvl[i][j] == 'a')
				{
					aquaSpr.setPosition(j * cell_size - camera_x, i * cell_size);
					window.draw(aquaSpr);
				}
				else if (lvl[i][j] == 's')
				{
					stoneSpr.setPosition(j * cell_size - camera_x, i * cell_size);
					window.draw(stoneSpr);
				}

			}
		}
	}
	int getWidth() const { return width; }
	int getCellSize() const { return cell_size; }



	bool isSolid(float x, float y, float w, float h) const {
		int left = (int)(x / cell_size);
		int right = (int)((x + w - 1) / cell_size);
		int top = (int)(y / cell_size);
		int bottom = (int)((y + h - 1) / cell_size);

		for (int row = top; row <= bottom; row++) {
			for (int col = left; col <= right; col++) {
				if (row >= 0 && row < height &&
					col >= 0 && col < width &&
					lvl[row][col] == 'g') {
					return true;
				}
			}
		}
		return false;
	}
};