#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include "Level.h"
using namespace sf;

class Game {
private:
    RenderWindow window;
    Camera camera;
    Level level;
    Player player;

    Texture backgroundTex;
    Sprite backgroundSprite;

    static const int NUM_CHARACTERS = 4;
    int currentPlayer;
    Texture playerTextures[NUM_CHARACTERS];
    Sprite playerSprite;

    const int screenW = 1600;
    const int screenH = 900;
    const float scaleX = 0.2f;
    const float scaleY = 0.2f;
    const float camMinX = 300.0f;


    void loadAssets() {
         (backgroundTex.loadFromFile("Data/Backgrounds/Bg1.jpg")) ;
        backgroundSprite.setTexture(backgroundTex);
        unsigned int bgWidth = backgroundTex.getSize().x;
        unsigned int bgHeight = backgroundTex.getSize().y;
        if (bgWidth > 0 && bgHeight > 0)
        {
            backgroundSprite.setScale(
                static_cast<float>(screenW) / bgWidth,
                static_cast<float>(screenH) / bgHeight);
        }

        playerTextures[0].loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");
        playerTextures[1].loadFromFile("Data/Characters/Tarma Roving/Tarma.png");
        playerTextures[2].loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");
        playerTextures[3].loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");

        player.setTexture(playerTextures[currentPlayer]);
        player.setScale(scaleX, scaleY);

         (level.loadTextures("Data/blocks/grass_block_side.png",
            "Data/blocks/stone.png")) ;
        
    }
    void handleEvents() {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) window.close();

            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Z) {
                currentPlayer = (currentPlayer + 1) % NUM_CHARACTERS;
                player.setTexture(playerTextures[currentPlayer]);  // Set on player
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
    }

    void update() {
        player.update(0.016f);
        player.handleInput();

        player.applyHorizontalMovement();
        float corrX = level.resolveHorizontalCollision(player.getPosition().x, player.getPosition().y, player.getSpriteWidth(), player.getSpriteHeight());
        player.resolveHorizontal(corrX);

        camera.update(player.getPosition().x, camMinX, screenW - camMinX - player.getSpriteWidth());

        player.applyGravity();
        float groundY = level.resolveVerticalCollision(player.getPosition().x, player.getPosition().y, player.getVelocityX(), player.getSpriteWidth(), player.getSpriteHeight());
        player.resolveVertical(groundY);
    }

    void render() {
        window.clear();
        window.draw(backgroundSprite);
        level.drawLevel(window, camera.getCameraOffset());

        player.draw(window);

        window.display();
    }

public:
    Game()
        : window(VideoMode(1600, 900), "Metal Slug", Style::Close),
        camera(1600, 900),
        level(14, 350, 64, screenH, screenW),
        currentPlayer(0)
    {
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
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