#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float size;
    float speed;
    
public:
    Obstacle(float x, float y, float baseSpeed);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setColor(const sf::Color& color);
    
    // Getters
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    float getSpeed() const { return speed; }
    sf::Vector2f getVelocity() const { return velocity; }
    bool isOffscreen() const;
    
    // Physics
    void setVelocity(const sf::Vector2f& newVelocity);
    void addVelocity(const sf::Vector2f& deltaVelocity);
    void setPosition(const sf::Vector2f& newPosition);
    float getSize() const { return size; }
}; 