
class Camera {
private:
    float cameraOffset;//how much the player has moved in world
    int screenWidth;
    int screenHeight;
    
public:
    Camera(int w, int h) : cameraOffset(0), screenWidth(w), screenHeight(h) {}
    
    void update(float playerWorldX, float minScreenX, float maxScreenX) {
        float screenX = playerWorldX - cameraOffset;// converting the worldCoordinates to screen Coordinates

 
        if (screenX > maxScreenX) cameraOffset = playerWorldX - maxScreenX;
        else if (screenX < minScreenX) cameraOffset = playerWorldX - minScreenX;
    }
    
    float getCameraOffset() const { return cameraOffset; }
    
    int worldToScreenX(float worldX) { return (int)(worldX - cameraOffset); }
    int getScreenWidth() const {
        return screenWidth;
    }
    int getScreenHeight() const {
        return screenHeight;
    }
    
    int worldToScreenY(float worldY) { return (int)worldY; }
    
    bool isVisible(float worldX, float width) {
        float screenX = worldX - cameraOffset;
        return ( (screenX + width > 0) && (screenX < screenWidth) );
    }
};

