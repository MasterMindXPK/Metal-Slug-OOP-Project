#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Animation {
private:
    Sprite& sprite;
    int frameWidth, frameHeight;
    int currentFrame, totalFrames;
    float frameTime, elapsed;
    int row; // which animation row on the sheet

public:
    Animation(sf::Sprite& spr, int fW, int fH, int frames, int row, float speed)
        : sprite(spr), frameWidth(fW), frameHeight(fH),
        currentFrame(0), totalFrames(frames), frameTime(speed), elapsed(0), row(row) {
    }

    void update(float deltaTime) {
        elapsed += deltaTime;
        if (elapsed >= frameTime) {
            elapsed = 0;
            currentFrame = (currentFrame + 1) % totalFrames;

            // Move along X for next frame, Y for different animations
            sprite.setTextureRect(sf::IntRect(
                currentFrame * frameWidth,   // X: scroll through frames
                row * frameHeight,           // Y: pick animation row
                frameWidth,
                frameHeight
            ));
        }
    }
};

#endif