
class Camera {
private:
    float xOffset;
    float scrollSpeed;
    int screenWidth;
    int screenHeight;
    
public:
    Camera(int w, int h) : xOffset(0), scrollSpeed(2.0f), 
                           screenWidth(w), screenHeight(h) {}
    
    void update() {
        xOffset += scrollSpeed;
    }
    
    float getXOffset() const { return xOffset; }
    
    int worldToScreenX(float worldX) {
        return (int)(worldX - xOffset);
    }
    
    int worldToScreenY(float worldY) {
        return (int)worldY; 
    }
    
    bool isVisible(float worldX, float width) {
        float screenX = worldX - xOffset;
        return screenX + width > 0 && screenX < screenWidth;
    }
};

