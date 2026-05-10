#pragma once
#include "Survival.h"
#include "Enemy.h"
enum BiomeType { aquatic,Mountain,plain};



class BiomeSpawner {	//Abstract class
public:
	virtual ~BiomeSpawner() {}



	virtual void spawnEnemies(int safeStartX, int safeWidth, Enemy** activeEnemies,
								int& enemyCount, int maxEnemies) = 0;


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
	void spawnEnemies(int safeStartX, int safeWidth, Enemy** activeEnemies, int& enemyCount,
		
		int maxEnemies) override {
		if (aquaticZombiesSpawned < 2) {
				float randomX = safeStartX + (rand() % safeWidth);



				// addEnemy(new Zombie(...), randomX, activeEnemies, enemyCount, maxEnemies);
						aquaticZombiesSpawned++;
		
		
		}
	}
};

class peakSpawner : public BiomeSpawner {
private:
	int aerialBatchesSpawned = 0;
	int alienBatchesSpawned = 0;



public:
	void spawnEnemies(int safeStartX, int safeWidth,
		Enemy** activeEnemies, int& enemyCount, int maxEnemies) override {
		if (aerialBatchesSpawned < 1) {
					float randomX = safeStartX + (rand() % safeWidth);
					// addEnemy(new Paratrooper(...), randomX, activeEnemies, enemyCount, maxEnemies);
					aerialBatchesSpawned++;
		}
		if (alienBatchesSpawned < 1) {
					float randomX = safeStartX + (rand() % safeWidth);
					// addEnemy(new Martian(...), randomX, activeEnemies, enemyCount, maxEnemies);
					alienBatchesSpawned++;
		
		
		}
	}
};

class plainSpawner : public BiomeSpawner {
private:
	int plainsUndeadSpawned = 0;
public:
	void spawnEnemies(int safeStartX, int safeWidth,
		Enemy** activeEnemies, int& enemyCount, int maxEnemies) override {



		if (plainsUndeadSpawned < 2) {


					float randomX = safeStartX + (rand() % safeWidth);
					// addEnemy(new Mummy(...), randomX, activeEnemies, enemyCount, maxEnemies);
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

	Enemy* infantryPrototypes[4];


public:
	EnemySpawn() {
		infantryBatchesSpawned = 0;
		aquaticZombiesSpawned = 0;
		plainsUndeadSpawned = 0;
		aerialBatchesSpawned = 0;
		alienBatchesSpawned = 0;


		infantryPrototypes[0] = new Rebel(2, 10, 200.f);
		infantryPrototypes[1] = new GrenadeSoldier(2, 8, 250.f);
		infantryPrototypes[2] = new ShieldedSoldier(5, 7, 200.f);
		infantryPrototypes[3] = new BazookaSoldier(2, 6, 350.f);
	}
	
	~EnemySpawn() {
		for (int i = 0; i < segments; i++) {
			if (segmentSpawners[i] != nullptr) {
				delete segmentSpawners[i];
				segmentSpawners[i] = nullptr;
			}
		}

		for (int i = 0; i < 4; i++) {
			delete infantryPrototypes[i];
		}
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

	void setupSpawnDecks(int lvl, int* infantryDeck, bool* spawnInSegment, int& totalInfantry) {

		int batchesPerType = (lvl == 1) ? 2 : 3;
		totalInfantry = batchesPerType * 4;



		int deckIndex = 0;
		for (int type = 0; type < 4; type++) {	//{0, 0, 1, 1, 2, 2, 3, 3}
			for (int b = 0; b < batchesPerType; b++) {
				infantryDeck[deckIndex] = type;
				deckIndex++;
			}
		}




		for (int i = 0; i < totalInfantry; i++) { //{1, 3, 0, 2, 0, 3, 1, 2}
			int swapIdx = rand() % totalInfantry;
			int temp = infantryDeck[i];
			infantryDeck[i] = infantryDeck[swapIdx];
			infantryDeck[swapIdx] = temp;
		}




		for (int i = 0; i < segments; i++) {
			spawnInSegment[i] = false;	
		}



		for (int i = 0; i < totalInfantry; i++) {
			spawnInSegment[i] = true;
		} // { T, T, T, T, T, T, T, T, F, F, F, F, F, F }

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
		bool spawnInSegment[14];




		setupSpawnDecks(lvl, infantryDeck, spawnInSegment, totalInfantry);




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
					Enemy* newEnemy = infantryPrototypes[randomIdx]->clone();
					float clusteredX = randomX + (b * 50.0f);
					segmentSpawners[i]->addEnemy(newEnemy, clusteredX, activeEnemies, enemyCount, maxEnemies);
				}

				spawnedCount++;
			}

			// BIOME SPAWNING
			if (segmentSpawners[i] != nullptr)  segmentSpawners[i]->spawnEnemies(safeStartX, safeWidth, activeEnemies, enemyCount, maxEnemies);
			
		}
	}
};

class VehicleSpawn {




};

