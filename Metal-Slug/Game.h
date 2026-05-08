#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Survival.h"
#include "Soldier.h"
#include "PlayerSoldier.h"
#include "Weapon.h"
#include "Projectile.h"


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

    static const int NUM_CHARACTERS = 4;
    int currentSoldierIndex;
    PlayerSoldier* soldiers[NUM_CHARACTERS];
    PlayerSoldier* currentPlayer;

	Clock clock;

    const int screenW = 1600;
    const int screenH = 900;
    const float camMinX = 300.0f;


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
        if (!menuBgTex.loadFromFile("Data/Backgrounds/Intro.jpg")) return false;
        menuBgSprite.setTexture(menuBgTex);
        menuBgSprite.setScale(
            static_cast<float>(screenW) / menuBgTex.getSize().x,
            static_cast<float>(screenH) / menuBgTex.getSize().y
        );

        if (!backgroundTex.loadFromFile("Data/Backgrounds/Bg1.jpg")) return false;
        if (!bulletTex.loadFromFile("Data/Items/bullet.png")) return false;
        for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->setProjectileTexture(bulletTex);
        }
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
        pistol.setProjectileTexture(bulletTex);
        Menu();
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
                    else if (menuIndex == 1) state = GameState::Campaign; // do nothing for now
                    else window.close();
                }

                upHeld = upNow;
                downHeld = downNow;
                enterHeld = enterNow;
                return;
            }

            if (Keyboard::isKeyPressed(Keyboard::Z)) {
                    currentSoldierIndex = (currentSoldierIndex + 1) % NUM_CHARACTERS;
                    currentPlayer = soldiers[currentSoldierIndex];
					Soldier.setTextureFromFile(currentPlayer->getTexturePath());
                    
                }
                else if (ev.key.code == Keyboard::Escape) {
                    window.close();
                }
                else if (Keyboard::isKeyPressed(Keyboard::F)) {
                    currentPlayer->specialMove();
                }
                else if (Keyboard::isKeyPressed(Keyboard::Space)) {
                    Position spawn;
                    spawn.x = Soldier.getPosition().x + Soldier.getWidth() / 2;
                    spawn.y = Soldier.getPosition().y + Soldier.getHeight() / 2;
				    currentPlayer->shoot(spawn);
                }
                
        }
    }

    void update() {
        bool inWater;
        float gravityStrength, speedMultiplier;
        if (state == GameState::Menu) return;
        inWater = survival.isWaterAt(Soldier.getPosition().x + Soldier.getWidth() * 0.5f, Soldier.getPosition().y + Soldier.getHeight() * 0.5f);
        if (inWater) {
            gravityStrength = 1.3;
            speedMultiplier = 0.65;
        }
        else {
            gravityStrength = 1;
            speedMultiplier = 1;
        }
       
        
        if (state == GameState::Menu) return;
        inWater = survival.isWaterAt(Soldier.getPosition().x + Soldier.getWidth() * 0.5f,  Soldier.getPosition().y + Soldier.getHeight() * 0.5f);
        
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
        float groundY = survival.verticalCollision( Soldier.getPosition().x, Soldier.getPosition().y, Soldier.getWidth(), Soldier.getHeight(), Soldier.getVelocityY());
        Soldier.applyVerticalCollision(groundY, nextY);

        int col = static_cast<int>(Soldier.getPosition().x / survival.getCellSize());
        if (col >= survival.getWidth() - 1) {
            levelCleared = true;
            clearFrames = clearDurationFrames;

            survival.generateSurvival();
            Soldier.setPosition({ 0.f, 0.f }); 
           
        }
        if (clearFrames > 0) {
            clearFrames--;
            if (clearFrames == 0) levelCleared = false;
        }
        camera.update(Soldier.getPosition().x, camMinX,
            screenW - camMinX - Soldier.getWidth());

        Soldier.setSprPos(Soldier.getPosition().x - camera.getCameraOffset(),
            Soldier.getPosition().y);
        /*for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->updateBullets(0.016);
        }*/
        for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->updateBullets(0.016f, survival, screenW, screenH);
        }
		currentPlayer->updateSpecialMove();
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
        window.draw(Soldier.getSprite());
        for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->drawBullets(window, camera.getCameraOffset());
        }

        window.display();
    }


    Game(): window(VideoMode(1600, 900), "Metal Slug", Style::Close),
        camera(1600, 900),
        survival(14, 350, 64, screenH, screenW),
        currentSoldierIndex(0)
    {
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);

        soldiers[0] = new Marco(pistol.createWeapon());
        soldiers[1] = new Tarma(pistol.createWeapon());
        soldiers[2] = new Eri(pistol.createWeapon());
        soldiers[3] = new Fio(pistol.createWeapon());

        currentPlayer = soldiers[currentSoldierIndex];
        Soldier.setTextureFromFile(currentPlayer->getTexturePath());
    }

    ~Game() {
        for (int i = 0; i < NUM_CHARACTERS; ++i) {
            delete soldiers[i];
            soldiers[i] = nullptr;
        }
    }

    void run() {
        (loadAssets()) ;
        survival.generateSurvival();

        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};