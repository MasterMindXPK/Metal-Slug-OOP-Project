#pragma once
#include <SFML/Graphics.hpp>

#define MAX_FRAMES 32  // max frames per animation strip; increase if needed

class Animation {
public:
    struct Frame {
        sf::IntRect rect;
        float duration;  // seconds this frame is shown
    };

private:
    Frame  frames[MAX_FRAMES];
    int    frameCount   = 0;
    int    currentFrame = 0;
    float  elapsedTime  = 0.f;
    bool   looping      = true;
    bool   finished     = false;

public:
    Animation() : frameCount(0), currentFrame(0), elapsedTime(0.f),
                  looping(true), finished(false) {}

    // Add a single frame manually
    void addFrame(sf::IntRect rect, float duration) {
        if (frameCount >= MAX_FRAMES) return;
        frames[frameCount].rect     = rect;
        frames[frameCount].duration = duration;
        ++frameCount;
    }

    // Add a horizontal strip from a sprite sheet
    // frameW/H      : size of one frame in pixels
    // row           : which row on the sheet (0 = top)
    // count         : number of frames in this strip
    // frameDuration : seconds per frame  (1.f / fps)
    // offsetX/Y     : pixel offset into the sheet if there's padding
    void addStrip(int frameW, int frameH, int row, int count,
                  float frameDuration, int offsetX = 0, int offsetY = 0)
    {
        for (int i = 0; i < count && frameCount < MAX_FRAMES; ++i) {
            sf::IntRect rect(offsetX + i * frameW,
                             offsetY + row * frameH,
                             frameW, frameH);
            addFrame(rect, frameDuration);
        }
    }

    void setLooping(bool loop) { looping  = loop;  }
    bool isFinished()    const { return finished;   }
    bool isEmpty()       const { return frameCount == 0; }
    int  getCurrentFrameIndex() const { return currentFrame; }

    void reset() {
        elapsedTime  = 0.f;
        currentFrame = 0;
        finished     = false;
    }

    void update(float dt) {
        if (frameCount == 0 || finished) return;

        elapsedTime += dt;
        while (elapsedTime >= frames[currentFrame].duration) {
            elapsedTime -= frames[currentFrame].duration;

            if (currentFrame + 1 < frameCount) {
                ++currentFrame;
            } else {
                if (looping) {
                    currentFrame = 0;
                } else {
                    finished = true;
                    return;
                }
            }
        }
    }

    void applyToSprite(sf::Sprite& sprite) const {
        if (frameCount > 0)
            sprite.setTextureRect(frames[currentFrame].rect);
    }
};
