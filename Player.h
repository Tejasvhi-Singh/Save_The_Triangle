#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    // Power colors
    enum class PowerState {
        Normal,
        SpeedBoost,
        Invulnerable,
        Charging,
        Overcharged
    };

private:
    sf::ConvexShape shape;
    sf::Vector2f position;
    float speed;
    float size;
    float currentRotation;
    float targetRotation;
    float rotationSpeed;
    bool isFlashing;
    float flashTime;
    
    PowerState currentPowerState;
    float powerTime;
    float powerDuration;
    
public:
    Player();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void reset();
    
    // Getters
    sf::Vector2f getPosition() const { return position; }
    float getSize() const { return size; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    
    // Movement (rocket-like)
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void updateRotation(float deltaTime);
    void setTargetRotation(float rotation);
    void setFlashing(bool flashing);
    void updateFlashing(float deltaTime);
    void setPowerState(PowerState state, float duration = 0.0f);
    void updatePowerState(float deltaTime);
    void updateColors();
}; 