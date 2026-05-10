#pragma once
#include "Animation.h"
#include <SFML/Graphics.hpp>

// ─────────────────────────────────────────────────────────────────────────────
//  AnimState  –  all animation states a soldier can be in
// ─────────────────────────────────────────────────────────────────────────────
enum class AnimState {
    Idle,
    Run,
    Jump,
    Fall,
    Shoot,
    Walk,       // used by enemies
    COUNT       // always keep last – used for array sizing
};

#define ANIM_COUNT static_cast<int>(AnimState::COUNT)

// ─────────────────────────────────────────────────────────────────────────────
//  AnimationManager
// ─────────────────────────────────────────────────────────────────────────────
class AnimationManager {
public:
    sf::Texture spriteSheet;

private:
    Animation animations[ANIM_COUNT];
    AnimState currentState  = AnimState::Idle;
    AnimState previousState = AnimState::Idle;

    int idx(AnimState s) const { return static_cast<int>(s); }

public:
    AnimationManager() : currentState(AnimState::Idle),
                         previousState(AnimState::Idle) {}

    // Load the sprite sheet texture from a file path (const char* not string)
    bool loadSheet(const char* path) {
        return spriteSheet.loadFromFile(path);
    }

    // Register one animation for a state
    // fps is converted to frameDuration internally
    void addAnimation(AnimState state,
                      int frameW, int frameH,
                      int row,    int count,
                      float fps,
                      int offsetX = 0, int offsetY = 0,
                      bool loop   = true)
    {
        int i = idx(state);
        animations[i] = Animation();                // reset to clean slate
        animations[i].addStrip(frameW, frameH, row, count,
                               1.f / fps, offsetX, offsetY);
        animations[i].setLooping(loop);
    }

    // ── State control ─────────────────────────────────────────────────────────

    void setState(AnimState newState) {
        if (newState == currentState) return;
        previousState = currentState;
        currentState  = newState;
        animations[idx(currentState)].reset();
    }

    AnimState getState()     const { return currentState;  }
    AnimState getPrevState() const { return previousState; }

    // ── Per-frame update ──────────────────────────────────────────────────────

    // Call once per frame; applies current frame rect to the sprite
    void update(float dt, sf::Sprite& sprite) {
        Animation& anim = animations[idx(currentState)];

        if (anim.isFinished()) {
            setState(AnimState::Idle);   // non-looping anim done → back to idle
            return;
        }

        anim.update(dt);
        anim.applyToSprite(sprite);
    }

    // Automatically pick the right state from physics values.
    // Call this every frame before update().
    // isShooting should be true for exactly one frame when the player fires.
    void updateStateFromPhysics(float velocityX, bool onGround, bool isShooting) {
        if (!onGround) {
            setState(AnimState::Jump);
        } else if (isShooting) {
            setState(AnimState::Shoot);
        } else if (velocityX != 0.f) {
            setState(AnimState::Run);
        } else {
            setState(AnimState::Idle);
        }
    }

    // Enemy variant – only needs Walk / Idle
    void updateStateEnemy(float velocityX) {
        if (velocityX != 0.f)
            setState(AnimState::Walk);
        else
            setState(AnimState::Idle);
    }
};


// ─────────────────────────────────────────────────────────────────────────────
//  SpriteSheetConfig  –  plain data struct; no string, no vector
//
//  Fill one per character/enemy type and pass to loadSpriteSheet() in Soldier.
//  MAX_ANIM_ROWS controls how many animation rows one sheet can describe.
// ─────────────────────────────────────────────────────────────────────────────
#define MAX_ANIM_ROWS 10

struct AnimRow {
    AnimState state;
    int   row;
    int   count;
    float fps;
    bool  loop = true;
};

struct SpriteSheetConfig {
    const char* sheetPath;  // e.g. "Data/Characters/Marco_sheet.png"
    int frameW;
    int frameH;
    int offsetX = 0;
    int offsetY = 0;
    AnimRow rows[MAX_ANIM_ROWS];
    int     rowCount = 0;   // how many entries in rows[] are valid

    // Convenience: push an AnimRow without vector::push_back
    void addRow(AnimState state, int row, int count, float fps, bool loop = true) {
        if (rowCount >= MAX_ANIM_ROWS) return;
        rows[rowCount++] = { state, row, count, fps, loop };
    }
};
