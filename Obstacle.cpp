#include "Obstacle.h"
#include <random>

Obstacle::Obstacle(float x, float y, float baseSpeed)
    : position(x, y)
    , speed(baseSpeed) {
    
    // Random size between 15 and 35
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> sizeDis(15.0f, 35.0f);
    size = sizeDis(gen);
    
    // Speed variation based on current game speed - faster game = faster obstacles
    // At low speeds: ±10% variation, at high speeds: ±5% variation
    float speedVariation = std::max(0.05f, 0.10f - (baseSpeed - 300.0f) / 900.0f * 0.05f);
    static std::uniform_real_distribution<float> speedVarDis(1.0f - speedVariation, 1.0f + speedVariation);
    float speedMultiplier = speedVarDis(gen);
    speed *= speedMultiplier;
    
    // Set velocity based on speed
    velocity = sf::Vector2f(0, speed);
    
    // Create circle shape
    shape.setRadius(size);
    shape.setPosition(position);
    
    // Set color based on speed relative to current game speed
    float speedRatio = speed / baseSpeed;
    if (speedRatio < 0.9f) {
        shape.setFillColor(sf::Color::Green);  // Slower than average - Green
    } else if (speedRatio < 1.1f) {
        shape.setFillColor(sf::Color::Yellow);  // Average speed - Yellow
    } else if (speedRatio < 1.3f) {
        shape.setFillColor(sf::Color::Red);     // Faster than average - Red
    } else {
        shape.setFillColor(sf::Color::Magenta); // Much faster - Magenta
    }
    
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.5f);
}

void Obstacle::update(float deltaTime) {
    position += velocity * deltaTime;
    shape.setPosition(position);
}

void Obstacle::setVelocity(const sf::Vector2f& newVelocity) {
    velocity = newVelocity;
    speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

void Obstacle::addVelocity(const sf::Vector2f& deltaVelocity) {
    velocity += deltaVelocity;
    speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

void Obstacle::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    shape.setPosition(position);
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Obstacle::setColor(const sf::Color& color) {
    shape.setFillColor(color);
}

bool Obstacle::isOffscreen() const {
    return position.y > 880.0f; // Below the 853p screen
} 