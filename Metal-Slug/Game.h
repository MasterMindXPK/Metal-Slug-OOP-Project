#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Level.h"
#include "Soldier.h"
#include "PlayerSoldier.h"
#include "Weapon.h"
#include "Projectile.h"


using namespace sf;

class Game {
private:
    RenderWindow window;
    Camera camera;
    Level level;
    Soldier Soldier;
    Pistol pistol;

    Texture bulletTex;
    Sprite bulletSp;
    Texture backgroundTex;
    Sprite backgroundSprite;

    static const int NUM_CHARACTERS = 4;
    int currentSoldierIndex;
    PlayerSoldier* soldiers[NUM_CHARACTERS];
    PlayerSoldier* currentPlayer;

	Clock clock;

    const int screenW = 1600;
    const int screenH = 900;

    const float camMinX = 300.0f;

    bool loadAssets() {
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
        
        
        if (!level.loadTextures("Data/blocks/grass_block_side.png",
            "Data/blocks/stone.png")) return false;
        //pistol.setProjectileTexture(bulletTex);
        
        return true;
    }
    void handleEvents() {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) window.close();

            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Z) {
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
       
        Soldier.applyGravity();
        Soldier.handleInput();
        float nextX = Soldier.getPosition().x + Soldier.getVelocityX();
        float correctedX = level.horizontalCollision(
            Soldier.getPosition().x,
            Soldier.getPosition().y,
            Soldier.getVelocityX(),
            Soldier.getWidth(),
            Soldier.getHeight()

        );
        if (correctedX != Soldier.getPosition().x + Soldier.getVelocityX()) {
            Soldier.applyHorizontalCollision(correctedX);
        }
        else {
            Soldier.setPosition({ correctedX, Soldier.getPosition().y });
        }

        float nextY = Soldier.getPosition().y + Soldier.getVelocityY();
        float groundY = level.verticalCollision(
            Soldier.getPosition().x,
            Soldier.getPosition().y,
            Soldier.getWidth(),
            Soldier.getHeight(),
            Soldier.getVelocityY()
        );
        Soldier.applyVerticalCollision(groundY, nextY);

        camera.update(Soldier.getPosition().x, camMinX,
            screenW - camMinX - Soldier.getWidth());

        Soldier.setSprPos(Soldier.getPosition().x - camera.getCameraOffset(),
            Soldier.getPosition().y);
        /*for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->updateBullets(0.016);
        }*/
        for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->updateBullets(0.016f, level, screenW, screenH);
        }
		currentPlayer->updateSpecialMove();
    }

    void render() {
        window.clear();
        window.draw(backgroundSprite);
        level.drawLevel(window, camera.getCameraOffset());
        window.draw(Soldier.getSprite());
        for (int i = 0; i < NUM_CHARACTERS; ++i) {
            soldiers[i]->getActiveWeapon()->drawBullets(window, camera.getCameraOffset());
        }

        window.display();
    }

public:
    Game(): window(VideoMode(1600, 900), "Metal Slug", Style::Close),
        camera(1600, 900),
        level(14, 350, 64, screenH, screenW),
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
        level.generateLevel();

        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};