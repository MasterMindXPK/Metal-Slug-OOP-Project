#include "Camera.h"

class Player {
private:
    float worldX; 
    float worldY;

public:
    Player() : worldX(0), worldY(300) {}

    void moveX(float disX) {
        worldX += disX;
    }

    void moveY(float disY) {
        worldY += disY;
    }

    float getWorldX() const { return worldX; }
    float getWorldY() const { return worldY; }
    float getScreenX(const Camera& cam) const { return worldX - cam.getXOffset(); }
};
