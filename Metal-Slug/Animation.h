#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// A single frame: spritesheet crop region + how long to display it
struct Frame {
    IntRect rect;
    float   duration;   // seconds
};

// -----------------------------------------------------------------------
// Animation
// Holds a fixed array of frames and advances through them over time.
// -----------------------------------------------------------------------
class Animation {
private:
    static const int MAX_FRAMES = 64;

    Frame       frames[MAX_FRAMES];
    int         frameCount;
    int         currentFrame;
    float       elapsed;        // time since last frame change
    bool        looping;
    bool        finished;       // true once a non-looping anim completes
    const char* name;           // must be a string literal or static char[]

public:
    Animation(const char* animName = "", bool loop = true)
        : frameCount(0), currentFrame(0), elapsed(0.f),
        looping(loop), finished(false), name(animName) {
    }

    // Add one frame with an explicit IntRect + duration
    // Use this for irregular spritesheets (non-uniform frame positions)
    void addFrame(const IntRect& rect, float duration = 0.1f) {
        if (frameCount >= MAX_FRAMES) return;
        frames[frameCount].rect = rect;
        frames[frameCount].duration = duration;
        frameCount++;
    }

    // Add multiple frames from a uniform horizontal strip on a spritesheet
    // startX, startY = pixel origin of the first frame
    // frameW, frameH = size of each frame
    // count          = number of frames to add
    // duration       = seconds per frame (same for all)
    void addFrameStrip(int startX, int startY,
        int frameW, int frameH,
        int count, float duration = 0.1f)
    {
        for (int i = 0; i < count && frameCount < MAX_FRAMES; i++) {
            frames[frameCount].rect = IntRect(startX + i * frameW, startY, frameW, frameH);
            frames[frameCount].duration = duration;
            frameCount++;
        }
    }

    // Advance animation by dt seconds
    // Returns the IntRect of the current frame — apply to sprite via setTextureRect()
    const IntRect& update(float dt) {
        static IntRect empty(0, 0, 0, 0);
        if (frameCount == 0 || finished) {
            return (frameCount == 0) ? empty : frames[currentFrame].rect;
        }

        elapsed += dt;

        while (elapsed >= frames[currentFrame].duration) {
            elapsed -= frames[currentFrame].duration;
            currentFrame++;

            if (currentFrame >= frameCount) {
                if (looping) {
                    currentFrame = 0;
                }
                else {
                    currentFrame = frameCount - 1;
                    finished = true;
                    break;
                }
            }
        }

        return frames[currentFrame].rect;
    }

    // Restart from frame 0 — call this when switching TO this animation
    void reset() {
        currentFrame = 0;
        elapsed = 0.f;
        finished = false;
    }

    const IntRect& getCurrentRect() const {
        static IntRect empty(0, 0, 0, 0);
        return (frameCount == 0) ? empty : frames[currentFrame].rect;
    }

    bool        isFinished()   const { return finished; }
    bool        isLooping()    const { return looping; }
    bool        isEmpty()      const { return frameCount == 0; }
    int         getFrameCount()const { return frameCount; }
    const char* getName()      const { return name; }
    void        setLooping(bool loop) { looping = loop; }
};


// -----------------------------------------------------------------------
// AnimationController
// Owns a fixed set of Animation objects and manages switching between them.
// Sits as a member inside PlayerSoldier (or any character class).
// -----------------------------------------------------------------------
class AnimationController {
private:
    static const int MAX_ANIMATIONS = 16;

    struct AnimEntry {
        const char* name;
        Animation   anim;
    };

    AnimEntry   anims[MAX_ANIMATIONS];
    int         animCount;
    int         currentIdx;     // index into anims[], -1 if none active
    int         previousIdx;

    int findIdx(const char* name) const {
        for (int i = 0; i < animCount; i++) {
            if (anims[i].name && strcmp(anims[i].name, name) == 0)
                return i;
        }
        return -1;
    }

public:
    AnimationController()
        : animCount(0), currentIdx(-1), previousIdx(-1) {
    }

    // Register an Animation — its name() is used as the lookup key
    void addAnimation(const Animation& anim) {
        if (animCount >= MAX_ANIMATIONS) return;
        anims[animCount].name = anim.getName();
        anims[animCount].anim = anim;
        animCount++;
    }

    // Switch to a named animation
    // Does nothing if already playing that animation (prevents restart flicker)
    // Pass force=true to restart even if it's already active
    void play(const char* name, bool force = false) {
        int idx = findIdx(name);
        if (idx == -1) return;                          // animation not registered

        if (idx == currentIdx && !force) return;        // already playing, skip

        previousIdx = currentIdx;
        currentIdx = idx;
        anims[currentIdx].anim.reset();
    }

    // Call every frame with delta time
    // Returns the IntRect to apply to the sprite via sprite.setTextureRect()
    IntRect update(float dt) {
        if (currentIdx == -1) return IntRect(0, 0, 0, 0);
        return anims[currentIdx].anim.update(dt);
    }

    // Return to whichever animation was playing before the current one
    void playPrevious() {
        if (previousIdx != -1)
            play(anims[previousIdx].name);
    }

    bool isFinished() const {
        if (currentIdx == -1) return true;
        return anims[currentIdx].anim.isFinished();
    }

    bool has(const char* name) const {
        return findIdx(name) != -1;
    }

    const char* getCurrent() const {
        if (currentIdx == -1) return "";
        return anims[currentIdx].name;
    }
};