#pragma once
#include "Survival.h"
#include "Enemy.h"
enum BiomeType { aquatic,Mountain,plain};



class BiomeSpawner {	//Abstract class
public:
	virtual ~BiomeSpawner() {}



	virtual void spawnEnemies(int safeStartX, int safeWidth, Enemy** activeEnemies,
		int& enemyCount, int maxEnemies, Enemy** prototypes) = 0;

	void addEnemy(Enemy* newEnemy, float posX, Enemy** activeEnemies, int& enemyCount, int maxEnemies) {
		if (enemyCount < maxEnemies) {


			newEnemy->setPosition({ posX, 100.0f });
			activeEnemies[enemyCount++] = newEnemy;



		}
		else {
			delete newEnemy;
		}
	}
};

class aquaticSpawner : public BiomeSpawner {
private:
	int aquaticZombiesSpawned = 0;
public:
	void spawnEnemies(int safeStartX, int safeWidth, Enemy** activeEnemies, int& enemyCount, int maxEnemies, Enemy** prototypes) override {
		if (aquaticZombiesSpawned < 2) {
			float randomX = safeStartX + (rand() % safeWidth);
	

			addEnemy(prototypes[4]->clone(), randomX, activeEnemies, enemyCount, maxEnemies);
			aquaticZombiesSpawned++;
		}
	}
};

class peakSpawner : public BiomeSpawner {
private:
	int aerialBatchesSpawned = 0;
	int alienBatchesSpawned = 0;



public:
	void spawnEnemies(int safeStartX, int safeWidth, Enemy** activeEnemies, int& enemyCount, int maxEnemies, Enemy** prototypes) override {
		if (aerialBatchesSpawned < 1) {
			float randomX = safeStartX + (rand() % safeWidth);



			addEnemy(prototypes[6]->clone(), randomX, activeEnemies, enemyCount, maxEnemies);
			aerialBatchesSpawned++;
		}



		if (alienBatchesSpawned < 1) {
			float randomX = safeStartX + (rand() % safeWidth);



			addEnemy(prototypes[7]->clone(), randomX, activeEnemies, enemyCount, maxEnemies);
			alienBatchesSpawned++;
		}
	}
};

class plainSpawner : public BiomeSpawner {
private:
	int plainsUndeadSpawned = 0;
public:
	void spawnEnemies(int safeStartX, int safeWidth, Enemy** activeEnemies, int& enemyCount, int maxEnemies, Enemy** prototypes) override {
		if (plainsUndeadSpawned < 2) {
			float randomX = safeStartX + (rand() % safeWidth);



			addEnemy(prototypes[5]->clone(), randomX, activeEnemies, enemyCount, maxEnemies);
			plainsUndeadSpawned++;
		}
	}
};






class EnemySpawn {

	static const int segments = 14;
	static const int segLen = 25;
	const int cellSize = 64;
	const int height = 14;
	const int width = 350;
	BiomeSpawner* segmentSpawners[segments];
	Texture rebelTex, grenadeTex, shieldTex, bazookaTex;
	Texture zombieTex, mummyTex, paratrooperTex, martianTex;



	//infantry

	bool rebelSpawned;
	bool grenadeSoldierSpawned;
	bool bazookaSoldierSpawned;
	bool shieldedSoldierSpawned;
	
	//Aerial

	bool paratroppersSpawned;
	
	//undeaad
	bool mummyWarriorSpawned;
	bool zombiesSpawned;

	//alien 

	bool martianSpawned;


	//batch tracking
	int infantryBatchesSpawned;
	int aquaticZombiesSpawned;
	int plainsUndeadSpawned;
	int aerialBatchesSpawned;
	int alienBatchesSpawned;

	Enemy* prototypes[8];


public:
	EnemySpawn() {
		infantryBatchesSpawned = 0;
		aquaticZombiesSpawned = 0;
		plainsUndeadSpawned = 0;
		aerialBatchesSpawned = 0;
		alienBatchesSpawned = 0;




		prototypes[0] = new Rebel(2, 10, 200.f);
		prototypes[1] = new GrenadeSoldier(2, 8, 250.f);
		prototypes[2] = new ShieldedSoldier(5, 7, 200.f);
		prototypes[3] = new BazookaSoldier(2, 6, 350.f);
		prototypes[4] = new Zombie();
		prototypes[5] = new Mummy();
		prototypes[6] = new Martian();
		prototypes[7] = new Martian();
	}
	
	~EnemySpawn() {
		for (int i = 0; i < segments; i++) {
			if (segmentSpawners[i] != nullptr) {
				delete segmentSpawners[i];
				segmentSpawners[i] = nullptr;
			}
		}

		for (int i = 0; i < 8; i++) {
			delete prototypes[i];
		}
	}

	bool loadTextures() {


		if (!rebelTex.loadFromFile("Data/Enemies/enemy_images/rsIMAGE1.png")) return false;
		if (!grenadeTex.loadFromFile("Data/Enemies/enemy_images/rsIMAGE1.png")) return false;//loading
		if (!shieldTex.loadFromFile("Data/Enemies/enemy_images/ssIMAGE1.png")) return false;//loading
		if (!bazookaTex.loadFromFile("Data/Enemies/enemy_images/bazIMAGE1.png")) return false;//laooading


		if (!zombieTex.loadFromFile("Data/Enemies/enemy_images/zIMAGE1.png")) return false;//loading
		if (!mummyTex.loadFromFile("Data/Enemies/enemy_images/mummyIMAGE1.png")) return false;
		if (!paratrooperTex.loadFromFile("Data/Enemies/enemy_images/ptIMAGE1.png")) return false;//loading
		if (!martianTex.loadFromFile("Data/Enemies/enemy_images/mIMAGE1.png")) return false;//loading

		std::cout << "ALL ENEMY TEXTURES LOADED OK\n";

	

		prototypes[0]->setEnemyTexture(rebelTex);        // Rebel
		prototypes[1]->setEnemyTexture(grenadeTex);      // Grenade
		prototypes[2]->setEnemyTexture(shieldTex);       // Shield
		prototypes[3]->setEnemyTexture(bazookaTex);      // Bazooka
		prototypes[4]->setEnemyTexture(zombieTex);       // Zombie
		prototypes[5]->setEnemyTexture(mummyTex);        // Mummy
		prototypes[6]->setEnemyTexture(paratrooperTex);  // Paratrooper
		//prototypes[6]->setEnemyTexture(rebelTex);
		prototypes[7]->setEnemyTexture(martianTex);      // Martian

		for (int i = 0;i < 8;i++) {
			prototypes[i]->scaleEnemy(1.5f);
		}

		return true;
	}

	void SpawnLocationCheck(Survival& map) {


		int peak = height;

		for (int i = 0;i < segments;i++) {
			int colCheck= (i * segLen) + (segLen/ 2);	//center column of that specific seg
			float checkX = colCheck * cellSize;
			bool foundWater = false;


			
			for (int j = 0;j < height;j++) {//iterate in the checking column from up to down to scan water
				float checkY = j   * cellSize;
				if (map.isWaterAt(checkX, checkY)) {
					foundWater = true;
					break; 


				}
				else if (map.isSolid(checkX, checkY, 1, 1) ){	//else determine if it is a peak or a plain
					if (i < peak) peak = i;
						
					
				}
			
			}

			if (foundWater)  segmentSpawners[i] = new aquaticSpawner();
			else if (peak <= height- 5) segmentSpawners[i] = new peakSpawner();
			else segmentSpawners[i] = new plainSpawner();
			

		}

	}

	void setupSpawnDecks(int lvl, int* infantryDeck, int& totalInfantry,
		int* otherDeck, int& totalOther, bool* spawnInSegment) {

		int batchesPerType = (lvl == 1) ? 2 : 3;
		int otherBatches = (lvl == 1) ? 1 : 2;
		int specialTypes[] = { 4, 5, 6, 7 };
		int numSpecialTypes = 4;
		totalOther = otherBatches * numSpecialTypes;
		totalInfantry = batchesPerType * 4;


		//form deck
		int deckIndex = 0;
		for (int type = 0; type < 4; type++) {	//{0, 0, 1, 1, 2, 2, 3, 3}
			for (int b = 0; b < batchesPerType; b++) {
				infantryDeck[deckIndex++] = type;
			}
		}



		//shuffling
		for (int i = 0; i < totalInfantry; i++) { //{1, 3, 0, 2, 0, 3, 1, 2}
			int swapIdx = rand() % totalInfantry;
			int temp = infantryDeck[i];
			infantryDeck[i] = infantryDeck[swapIdx];
			infantryDeck[swapIdx] = temp;
		}


		//form deck
		int otherDeckIndex = 0;
		for (int i = 0; i < numSpecialTypes; i++) {
			for (int b = 0; b < otherBatches; b++) {


				otherDeck[otherDeckIndex++] = specialTypes[i];
			
			
			}
		}

		//shuffle
		for (int i = 0; i < totalOther; i++) {


			int swapIdx = rand() % totalOther;
			int temp = otherDeck[i];
			otherDeck[i] = otherDeck[swapIdx];
			otherDeck[swapIdx] = temp;


		}



		for (int i = 0; i < segments; i++) 
			spawnInSegment[i] = false;
		
		//true for infantry segs
		for (int i = 0; i < totalInfantry; i++) 
			spawnInSegment[i] = true;
		



		for (int i = 0; i < segments; i++) {
			int swapIdx = rand() % segments;


			bool temp = spawnInSegment[i];
			spawnInSegment[i] = spawnInSegment[swapIdx];
			spawnInSegment[swapIdx] = temp;


		}

		for (int i = 0; i < segments; i++) {
			int swapIdx = rand() % segments;


			bool temp = spawnInSegment[i];
			spawnInSegment[i] = spawnInSegment[swapIdx];
			spawnInSegment[swapIdx] = temp;


		}
	}




	void spawnEnemies(Survival& map, Enemy** activeEnemies, int& enemyCount, int maxEnemies, int lvl) {
		int segWidth = segLen * cellSize; // in px

		int totalInfantry = 0;
		int infantryDeck[12];
		int totalOther = 0;
		int otherDeck[20];



		bool spawnInSegment[14];



		setupSpawnDecks(lvl, infantryDeck, totalInfantry, otherDeck, totalOther, spawnInSegment);

		int spawnedCount = 0;



		for (int i = 0; i < segments; i++) {


			int startX = i * segWidth;
			int safeWidth = segWidth - (4 * cellSize);
			int safeStartX = startX + (2 * cellSize);

			if (spawnInSegment[i] && spawnedCount < totalInfantry) {
				float randomX = safeStartX + (rand() % (safeWidth - 150));



				int randomIdx = infantryDeck[spawnedCount];
				int batchSize = 1;




				if (randomIdx == 0) batchSize = 2 + (rand() % 3);
				else batchSize = 1 + (rand() % 2);

				for (int b = 0; b < batchSize; b++) {
		


					Enemy* newEnemy = prototypes[randomIdx]->clone();
					float clusteredX = randomX + (b * 50.0f);

					if (enemyCount < maxEnemies) {
						newEnemy->setPosition({ clusteredX, 100.0f });
						activeEnemies[enemyCount++] = newEnemy;


					}
					else delete newEnemy;
				}
				spawnedCount++;
			}


			if (segmentSpawners[i] != nullptr) {
	
				segmentSpawners[i]->spawnEnemies(safeStartX, safeWidth, activeEnemies, enemyCount, maxEnemies, prototypes);


			}
		}
	}
};






