#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Survival.h"
#include "Soldier.h"
#include "PlayerSoldier.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Spawn.h"
#include "Weapon.h"
#include "DeveloperMode.h"
using namespace sf;

class Game {
private:
    Texture menuBgTex;
    Sprite menuBgSprite;
    enum class GameState { Menu, Survival, Campaign };
    GameState state = GameState::Menu;

    Font menuFont;
    Text menuTitle;
    Text menuItems[3];
    int menuIndex = 0;
    Clock menuInputClock;
    RenderWindow window;
    Camera camera;
    Survival survival;
    Soldier Soldier;
    Pistol pistol;
    Texture bulletTex;
    Sprite bulletSp;
    Texture backgroundTex;
    Sprite backgroundSprite;
    bool levelCleared = false;
    int clearFrames = 0;
    const int clearDurationFrames = 120;
    Text levelClearText;
    static const int players = 4;
    int currentSoldierIndex;
    PlayerSoldier* soldiers[players];
    PlayerSoldier* currentPlayer;
    static const int MAX_LEVEL_WEAPONS = 20;
    Weapon* levelWeapons[MAX_LEVEL_WEAPONS];
    int levelWeaponCount = 0;

    float aimDirX;
	float aimDirY;
    bool mousePressedBefore = false;
    int playerLives = 3;        
    float playerDamageCooldown = 0.0f; 

    bool youWin;
    Text youWinText;


    //Enemy Management:
    static const int maxEnemies = 150;
    Enemy* activeEnemies[maxEnemies];
    int enemyCount = 0;
    EnemySpawn enemyManager;
    int currentLevel = 1;

    const int screenW = 1600;
    const int screenH = 900;
    const float camMinX = 300.0f;

    void checkLevelProgression() {
        if (survival.hasReachedEnd(Soldier.getPosition().x, 64)) {
            currentLevel++;

            if (currentLevel > 3) {


                youWin = true;
                return;
            }

            levelCleared = true;
            clearFrames = clearDurationFrames;

            for (int i = 0; i < enemyCount; ++i) {
                if (activeEnemies[i] != nullptr) {



                    delete activeEnemies[i];
                    activeEnemies[i] = nullptr;
                }
            }
            enemyCount = 0;
            survival.generateSurvival();
            Soldier.setPosition({ 0.f, 0.f });
            camera.update(0.f, camMinX, screenW - camMinX - Soldier.getWidth());
            enemyManager.SpawnLocationCheck(survival);
            enemyManager.spawnEnemies(survival, activeEnemies, enemyCount, maxEnemies, currentLevel);
        }

        if (clearFrames > 0) {
            clearFrames--;
            if (clearFrames == 0) levelCleared = false;
        }
    }

    //Grenade - Enemy Collision Management
    void processGrenades() {
        for (int i = 0; i < players; i++) {
            GrenadeWeapon* g = soldiers[i]->getGrenade();

            // blast damage
            for (int j = 0; j < g->getMaxThrows(); j++) {
                if (!g->blasts[j].pending) continue;
                Position blast = g->blasts[j].center;
                g->blasts[j].pending = false;

                for (int e = 0; e < enemyCount; e++) {
                    if (!activeEnemies[e]) continue;
                    float dx = activeEnemies[e]->getPosition().x - blast.x;
                    float dy = activeEnemies[e]->getPosition().y - blast.y;
                    if (dx * dx + dy * dy <= g->getBlastRadius() * g->getBlastRadius())
                        activeEnemies[e]->takeDamage(20);
                }
            }

            // fire pool damage 
            int poolCount = 0;
            FirePool* pools = g->getFirePools(poolCount);
            for (int p = 0; p < poolCount; p++) {
                if (!pools[p].isActive())    continue;
                if (!pools[p].canDamage()) continue;
                for (int e = 0; e < enemyCount; e++) {
                    if (!activeEnemies[e]) continue;


                    float ex = activeEnemies[e]->getPosition().x + activeEnemies[e]->getWidth() / 2.f;
                    float ey = activeEnemies[e]->getPosition().y + activeEnemies[e]->getHeight() / 2.f;


                    if (pools[p].contains(ex, ey))
                        activeEnemies[e]->takeDamage(5);
                }
            }
        }
    }
    void processBulletCollisions() {
        for (int p = 0; p < players; p++) {
            Weapon* w = soldiers[p]->getActiveWeapon();
           
            
            int total = w->getTotalAmmo();
            if (total == 0) continue;

            for (int i = 0; i < total; i++) {
                
                
                StraightProjectile* b = w->getBulletPtr(i);
                if (!b || !b->isActive()) continue;

                Position bPos = b->getPosition();
               
                
                
                float bw = b->getWidth();
                float bh = b->getHeight();

                for (int e = 0; e < enemyCount; e++) {
                    if (!activeEnemies[e]) continue;
                    
                    
                    
                    float ex = activeEnemies[e]->getPosition().x;
                    float ey = activeEnemies[e]->getPosition().y;
                    float ew = activeEnemies[e]->getWidth();
                    float eh = activeEnemies[e]->getHeight();

                    bool hit = (bPos.x < ex + ew) &&
                        (bPos.x + bw > ex) &&
                        (bPos.y < ey + eh) &&
                        (bPos.y + bh > ey);

                    if (hit) {
                        activeEnemies[e]->takeDamage((int)w->getDamage());
                        b->deactivate();
                        break;
                    }
                }
            }
        }
    }
public:
    void updateMenuColors() {
        for (int i = 0; i < 3; ++i) {
            menuItems[i].setFillColor(i == menuIndex ? Color::Yellow : Color::White);
        }
    }

    void Menu() {
        menuFont.loadFromFile("Data/Font/arial.ttf");
        menuTitle.setFont(menuFont);
        menuTitle.setString("METAL SLUG");
        menuTitle.setCharacterSize(60);
        menuTitle.setFillColor(Color(255, 140, 0)); // orange
        menuTitle.setPosition(520, 200);

        const char* labels[3] = { "1. Survival Mode", "2. Campaign Mode", "3. Exit" };
        for (int i = 0; i < 3; ++i) {
            menuItems[i].setFont(menuFont);
            menuItems[i].setString(labels[i]);
            menuItems[i].setCharacterSize(30);
            menuItems[i].setPosition(600, 350 + i * 50);
        }
        updateMenuColors();
    }

    bool loadAssets() {
        if (!bulletTex.loadFromFile(("Data/items/bullet.png")))     std::cout << "ERROR: Missing bullet image!" << std::endl;
        
        
        youWinText.setFont(menuFont);
        youWinText.setString("YOU WIN!");
        youWinText.setCharacterSize(120);
        youWinText.setFillColor(Color::Yellow);
        youWinText.setPosition(500, 350);
        //Background
        if (!menuBgTex.loadFromFile("Data/Backgrounds/Intro.jpg")) return false;
        menuBgSprite.setTexture(menuBgTex);
        menuBgSprite.setScale(
            static_cast<float>(screenW) / menuBgTex.getSize().x,
            static_cast<float>(screenH) / menuBgTex.getSize().y
        );

        if (!backgroundTex.loadFromFile("Data/Backgrounds/Bg1.jpg")) return false;
        backgroundSprite.setTexture(backgroundTex);

        unsigned int bgWidth = backgroundTex.getSize().x;
        unsigned int bgHeight = backgroundTex.getSize().y;
        if (bgWidth > 0 && bgHeight > 0)
        {
            backgroundSprite.setScale(
                static_cast<float>(screenW) / bgWidth,
                static_cast<float>(screenH) / bgHeight);
        }

        levelClearText.setFont(menuFont);
        levelClearText.setString("LEVEL CLEAR");
        levelClearText.setCharacterSize(90);
        levelClearText.setFillColor(Color::Yellow);
        levelClearText.setPosition(520, 200);

        if (!survival.loadTextures("Data/blocks/grass_block_side.png",
            "Data/blocks/aqua.jpg", "Data/blocks/stone.png")) return false;
        Menu();


        if (!enemyManager.loadTextures()) {
            std::cout << "ERROR: Failed to load Enemy textures!" << std::endl;
            return false;
        }
        return true;
    }

    void handleEvents() {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) window.close();

            if (state == GameState::Menu) {
                bool upHeld = false;
                bool downHeld = false;
                bool enterHeld = false;

                bool upNow = Keyboard::isKeyPressed(Keyboard::Up);
                bool downNow = Keyboard::isKeyPressed(Keyboard::Down);
                bool enterNow = Keyboard::isKeyPressed(Keyboard::Enter);

                if (upNow && !upHeld) {
                    menuIndex = (menuIndex + 2) % 3;
                    updateMenuColors();
                }
                if (downNow && !downHeld) {
                    menuIndex = (menuIndex + 1) % 3;
                    updateMenuColors();
                }
                if (enterNow && !enterHeld) {
                    if (menuIndex == 0) state = GameState::Survival;
                    else if (menuIndex == 1) state = GameState::Campaign;
                    else window.close();
                }

                upHeld = upNow;
                downHeld = downNow;
                enterHeld = enterNow;
                return;
            }
            

			if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Z) {
                    currentSoldierIndex = (currentSoldierIndex + 1) % players;
                    currentPlayer = soldiers[currentSoldierIndex];
					Soldier.setTextureFromFile(currentPlayer->getTexturePath());
                    
                }
                else if (ev.key.code == Keyboard::Escape) {
                    window.close();
                }
                else if (Keyboard::isKeyPressed(Keyboard::F)) {
                    currentPlayer->specialMove();
                }
                else if (Keyboard::isKeyPressed(Keyboard::G)) {
                    Position spawn;
                    if(Soldier.getFacingDir() == 1) spawn.x = Soldier.getPosition().x + Soldier.getWidth()/2.0f; 
					else spawn.x = Soldier.getPosition().x - Soldier.getWidth()/2.0f -20; 
                    
                    spawn.y = Soldier.getPosition().y + Soldier.getHeight() / 2;
                    currentPlayer->throwGrenade(spawn, aimDirX, aimDirY);
                }
                else if(Keyboard::isKeyPressed(Keyboard::P)) {
                    DeveloperMode::toggle();
			    }
                else if(Keyboard::isKeyPressed(Keyboard::Num1)) {
                    if(DeveloperMode::isActive())
				        currentPlayer->dropWeapon(currentPlayer->getActiveWeapon());
			    }
                else if (Keyboard::isKeyPressed(Keyboard::Num2)){
                    if (DeveloperMode::isActive()) {
                        Weapon* w = WeaponFactory::createMachineGun();
                        currentPlayer->equipWeapon(w);
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
                    if (DeveloperMode::isActive()) {
                        Weapon* w = WeaponFactory::createRocketLauncher();
                        currentPlayer->equipWeapon(w);
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Num4)) {
                    if (DeveloperMode::isActive()) {
                        Weapon* w = WeaponFactory::createFlameshot();
                        currentPlayer->equipWeapon(w);
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Num5)) {
                    if (DeveloperMode::isActive()) {
                        Weapon* w = WeaponFactory::createLaserGun();
                        currentPlayer->equipWeapon(w);
                    }
                }
        }
    }

    void update() {
        if (youWin) return;
        bool inWater;
        float gravityStrength, speedMultiplier;
        if (state == GameState::Menu) return;

        if (playerDamageCooldown > 0.0f) {
            playerDamageCooldown -= 0.016f;
        }

        inWater = survival.isWaterAt(Soldier.getPosition().x + Soldier.getWidth() * 0.5f, Soldier.getPosition().y + Soldier.getHeight() * 0.5f);
        if (inWater) {
            gravityStrength = 1.3;
            speedMultiplier = 0.65;
        }
        else {
            gravityStrength = 1;
            speedMultiplier = 1;
        }

        Soldier.applyGravity(gravityStrength);
        Soldier.handleInput(speedMultiplier);
        float nextX = Soldier.getPosition().x + Soldier.getVelocityX();
        float correctedX = survival.horizontalCollision(Soldier.getPosition().x, Soldier.getPosition().y, Soldier.getVelocityX(), Soldier.getWidth(), Soldier.getHeight());

        if (correctedX != Soldier.getPosition().x + Soldier.getVelocityX()) Soldier.applyHorizontalCollision(correctedX);
        else Soldier.setPosition({ correctedX, Soldier.getPosition().y });

        float nextY = Soldier.getPosition().y + Soldier.getVelocityY();
        float groundY = survival.verticalCollision(Soldier.getPosition().x, Soldier.getPosition().y, Soldier.getWidth(), Soldier.getHeight(), Soldier.getVelocityY());
        Soldier.applyVerticalCollision(groundY, nextY);

        checkLevelProgression();
        camera.update(Soldier.getPosition().x, camMinX,
            screenW - camMinX - Soldier.getWidth());

        Soldier.setSprPos(Soldier.getPosition().x - camera.getCameraOffset(),
            Soldier.getPosition().y);

        for (int i = 0; i < players; ++i) {
            soldiers[i]->getActiveWeapon()->updateBullets(0.016f, survival, screenW, screenH);
			soldiers[i]->updateGrenade(0.016f, survival, screenW, screenH);
            currentPlayer->getKnife()->updateBullets(0.016f, survival, screenW, screenH);
        }

        //============================Shooting===============================================
		//---------------------------Aiming with Mouse----------------

        int mouseDirY = Mouse::getPosition(window).y;
        int mouseDirX = Mouse::getPosition(window).x;
        float gunPosY = Soldier.getPosition().y + Soldier.getHeight()/2.0f;
        float gunPosX = Soldier.getPosition().x + Soldier.getWidth()/2.0f - camera.getCameraOffset();
		float dy = mouseDirY - gunPosY;
		float dx = mouseDirX - gunPosX;

        
		float angle = std::atan2(-dy, (dx < 0 ? -dx : dx));        //using -dy because in SFML, Y axis is inverted 
        if (angle < 0.0f) angle = 0.0f;
        else if(angle > 3.141592f / 2.0f)  angle = 3.141592f / 2.0f;

		aimDirY = -std::sin(angle);                //again using -sin because of inverted Y axis
        aimDirX = Soldier.getFacingDir() * std::cos(angle);

		currentPlayer->updateSpecialMove();

        bool mousePressedNow = Mouse::isButtonPressed(Mouse::Left);
        if (mousePressedNow) {
            Position spawn;
            if (Soldier.getFacingDir() == 1) spawn.x = Soldier.getPosition().x + Soldier.getWidth() / 2.0f;
            else spawn.x = Soldier.getPosition().x - Soldier.getWidth() / 2.0f - 20;
            spawn.y = Soldier.getPosition().y + Soldier.getHeight() / 2;
            currentPlayer->shoot(spawn, aimDirX, aimDirY);
        }
        else {
            currentPlayer->getActiveWeapon()->stopFiring();
        }
        mousePressedBefore = mousePressedNow;
		//=============================Melee Attack============================================
        if (Keyboard::isKeyPressed(Keyboard::V)) {
        //   processMelee();
        }
		//============================Enemy Logic===============================================
        float dt = 0.016f;
        float playerX = Soldier.getPosition().x;
        for (int i = 0;i < maxEnemies;i++) {
            if (activeEnemies[i] != nullptr) {
               
                activeEnemies[i]->detectCollision(survival);
                activeEnemies[i]->applyGravity(1.0f);

                float nextY = activeEnemies[i]->getPosition().y + activeEnemies[i]->getVelocityY();
                float groundY = survival.verticalCollision(
                    activeEnemies[i]->getPosition().x,
                    activeEnemies[i]->getPosition().y,



                    activeEnemies[i]->getWidth(),
                    activeEnemies[i]->getHeight(),
                    activeEnemies[i]->getVelocityY()
                );

                activeEnemies[i]->applyVerticalCollision(groundY, nextY);

                activeEnemies[i]->attack(dt, playerX);

                activeEnemies[i]->updateProjectiles(0.016f, survival, screenW, screenH);
                activeEnemies[i]->setSprPos(
                    activeEnemies[i]->getPosition().x - camera.getCameraOffset(),
                    activeEnemies[i]->getPosition().y
                );
                bool isColliding = (
                    Soldier.getPosition().x < activeEnemies[i]->getPosition().x + activeEnemies[i]->getWidth() &&
                    Soldier.getPosition().x + Soldier.getWidth() > activeEnemies[i]->getPosition().x &&
                    Soldier.getPosition().y < activeEnemies[i]->getPosition().y + activeEnemies[i]->getHeight() &&
                    Soldier.getPosition().y + Soldier.getHeight() > activeEnemies[i]->getPosition().y
                    );
                if (isColliding && playerDamageCooldown <= 0.0f) {





                    currentPlayer->takeDamage(1);
                    playerDamageCooldown = 2.0f;
                    std::cout << "Player was hit by an enemy! Cooldown activated." << std::endl;


                }
            }
        }
        //===========================grenade================================
        processBulletCollisions();
        processGrenades();
        
   
        for (int i = 0; i < maxEnemies; i++) {
            if (activeEnemies[i] != nullptr) {
                // If the enemy's health is 0 or less
                if (activeEnemies[i]->getHp() <= 0) {

                    activeEnemies[i]->die();     // Trigger their death function
                    delete activeEnemies[i];     // Free up the RAM safely
                    activeEnemies[i] = nullptr;  // Empty the slot so a new enemy can spawn later

                }
            }
        }
    }

    void render() {
        if (state == GameState::Menu) {
            window.draw(menuBgSprite);
            window.draw(menuTitle);
            for (int i = 0; i < 3; ++i) window.draw(menuItems[i]);
            window.display();
            return;
        }

        window.clear();
        window.draw(backgroundSprite);
        if (levelCleared) window.draw(levelClearText);
        survival.drawsurvival(window, camera.getCameraOffset());
        
        for (int i = 0; i < players; ++i) {
            soldiers[i]->getActiveWeapon()->drawBullets(window, camera.getCameraOffset());
			soldiers[i]->drawGrenade(window, camera.getCameraOffset());
            currentPlayer->getKnife()->drawBullets(window, camera.getCameraOffset());
            // draw fire pools
            int poolCount = 0;
            FirePool* pools = soldiers[i]->getGrenade()->getFirePools(poolCount);
            for (int p = 0; p < poolCount; p++)
                pools[p].draw(window, camera.getCameraOffset());
        }

        for (int i = 0; i < maxEnemies; ++i) {
            if (activeEnemies[i] != nullptr) {
 
                activeEnemies[i]->draw(window);
                activeEnemies[i]->drawProjectiles(window, camera.getCameraOffset());
            }
        }
        if (youWin)   window.draw(youWinText);
        window.draw(Soldier.getSprite());
        window.display();
    }

    Game() : window(VideoMode(1600, 900), "Metal Slug", Style::Close),
        camera(1600, 900),
        survival(14, 350, 64, screenH, screenW),
        currentSoldierIndex(0)
    {
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);

        //player
        soldiers[0] = new Marco();
        soldiers[1] = new Tarma();
        soldiers[2] = new Eri();
        soldiers[3] = new Fio();

        currentPlayer = soldiers[currentSoldierIndex];
        Soldier.setTextureFromFile(currentPlayer->getTexturePath());

        //enemies
        for (int i = 0; i < maxEnemies; i++) {
            activeEnemies[i] = nullptr;
        }
    }

    ~Game() {
        for (int i = 0; i < players; ++i) {
            delete soldiers[i];
            soldiers[i] = nullptr;
        }
        for (int i = 0; i < enemyCount; ++i) {
            if (activeEnemies[i] != nullptr) {
                delete activeEnemies[i];
            }
        }
    }

    void run() {
        loadAssets();
        survival.generateSurvival();
        enemyManager.SpawnLocationCheck(survival);
        enemyManager.spawnEnemies(survival, activeEnemies, enemyCount, maxEnemies, currentLevel);

        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};