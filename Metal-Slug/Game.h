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

    const int screenW = 1600;
    const int screenH = 900;
    const float scaleX = 0.2f;
    const float scaleY = 0.2f;
    const float camMinX = 300.0f;

    bool loadAssets() {
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
        /*playerTextures[0].loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");
        playerTextures[1].loadFromFile("Data/Characters/Tarma Roving/Tarma.png");
        playerTextures[2].loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");
        playerTextures[3].loadFromFile("Data/Characters/Marco Rossi/MarcoRossi_Player1.png");
        player.setSpriteTexture(playerTextures[0]);
        player.setSpriteScale(scaleX, scaleY);

        player.setTexture(playerTextures[currentPlayer]);
        player.setScale(scaleX, scaleY);

        player.setSpriteTexture(playerTextures[currentPlayer]);
        player.setSpriteScale(scaleX, scaleY);*/

        if (!level.loadTextures("Data/blocks/grass_block_side.png",
            "Data/blocks/stone.png")) return false;
        return true;
    }
    void handleEvents() {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) window.close();

            if (Keyboard::isKeyPressed(Keyboard::Z)) {
                currentPlayer = (currentPlayer + 1) % NUM_CHARACTERS;
                player.setSpriteTexture(playerTextures[currentPlayer]);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
    }

    void update() {
        player.applyGravity();
        player.handleInput();
        
        

        //Collsiions
        float corrX = level.horizontalCollision(player.getPosition().x, player.getPosition().y, player.getVelocityX(),player.getWidth(), player.getHeight());
        player.applyHorizontalCollision(corrX);

        float groundY = level.verticalCollision(player.getPosition().x, player.getPosition().y,
            player.getWidth(), player.getHeight());
        player.applyVerticalCollision(groundY);


        camera.update(player.getPosition().x, camMinX,
            screenW - camMinX - player.getWidth());

        Position newPosition = { player.getPosition().x + player.getVelocityX(),player.getPosition().y + player.getVelocityY() };
        player.setPosition(newPosition);
        player.setSprPos(player.getPosition().x - camera.getCameraOffset(), player.getPosition().y);
        
    }
   

    void render() {
        window.clear();
        window.draw(backgroundSprite);
        level.drawLevel(window, camera.getCameraOffset());
        window.draw(player.getSprite());
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