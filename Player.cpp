#include "Player.h"

Player::Player() 
    : position(240.0f, 650.0f)  // Moved up from 750 to 650
    , speed(400.0f)  // Increased speed for rocket movement
    , size(16.0f)  // Smaller triangle size
    , currentRotation(0.0f)
    , targetRotation(0.0f)
    , rotationSpeed(360.0f)  // Degrees per second
    , isFlashing(false)
    , flashTime(0.0f)
    , currentPowerState(PowerState::Normal)
    , powerTime(0.0f)
    , powerDuration(0.0f) {
    
    // Create triangle shape pointing upward
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0, -size));      // Top point
    shape.setPoint(1, sf::Vector2f(-size, size));   // Bottom left
    shape.setPoint(2, sf::Vector2f(size, size));    // Bottom right
    
    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Cyan);
    shape.setOutlineThickness(1.5f);  // Thinner outline for smaller triangle
    shape.setPosition(position);
    shape.setRotation(currentRotation);
}

void Player::update(float deltaTime) {
    // Update rotation smoothly
    updateRotation(deltaTime);
    updateFlashing(deltaTime);
    updatePowerState(deltaTime);
    updateColors();
    
    // Triangle stays stationary - no automatic movement
    // Only keyboard movement will be handled in Game::update()
    
    // Keep player within screen bounds (for keyboard movement)
    if (position.y < 60.0f) {  // Adjusted for 853p height
        position.y = 60.0f;
    }
    if (position.y > 793.0f) {  // Adjusted for 853p height
        position.y = 793.0f;
    }
    
    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::reset() {
    position = sf::Vector2f(240.0f, 650.0f);  // Moved up from 750 to 650
    currentRotation = 0.0f;
    targetRotation = 0.0f;
    shape.setPosition(position);
    shape.setRotation(currentRotation);
    currentPowerState = PowerState::Normal;
    powerTime = 0.0f;
    powerDuration = 0.0f;
}

void Player::moveLeft(float deltaTime) {
    position.x -= speed * deltaTime;
    if (position.x < size) {  // Adjusted for smaller triangle
        position.x = size;
    }
    shape.setPosition(position);
    
    // Tilt left when moving left
    setTargetRotation(-30.0f);
}

void Player::moveRight(float deltaTime) {
    position.x += speed * deltaTime;
    if (position.x > 480.0f - size) {  // Adjusted for 480 width
        position.x = 480.0f - size;
    }
    shape.setPosition(position);
    
    // Tilt right when moving right
    setTargetRotation(30.0f);
}

void Player::moveForward(float deltaTime) {
    // Move upward (forward in rocket terms)
    position.y -= speed * deltaTime;
    if (position.y < 60.0f) {  // Top boundary
        position.y = 60.0f;
    }
    shape.setPosition(position);
    
    // Slight upward tilt when moving forward
    setTargetRotation(-15.0f);
}

void Player::moveBackward(float deltaTime) {
    // Move downward (backward in rocket terms)
    position.y += speed * deltaTime;
    if (position.y > 793.0f) {  // Bottom boundary
        position.y = 793.0f;
    }
    shape.setPosition(position);
    
    // Slight downward tilt when moving backward
    setTargetRotation(15.0f);
}

void Player::updateRotation(float deltaTime) {
    // Smoothly interpolate towards target rotation
    float rotationDiff = targetRotation - currentRotation;
    
    // Handle angle wrapping (e.g., going from 350° to 10°)
    if (rotationDiff > 180.0f) {
        rotationDiff -= 360.0f;
    } else if (rotationDiff < -180.0f) {
        rotationDiff += 360.0f;
    }
    
    // Move towards target rotation
    if (std::abs(rotationDiff) > 1.0f) {
        float rotationStep = rotationSpeed * deltaTime;
        if (std::abs(rotationDiff) < rotationStep) {
            currentRotation = targetRotation;
        } else {
            currentRotation += (rotationDiff > 0 ? rotationStep : -rotationStep);
        }
    } else {
        currentRotation = targetRotation;
    }
    
    // Keep rotation in 0-360 range
    while (currentRotation >= 360.0f) currentRotation -= 360.0f;
    while (currentRotation < 0.0f) currentRotation += 360.0f;
    
    shape.setRotation(currentRotation);
}

void Player::setTargetRotation(float rotation) {
    targetRotation = rotation;
}

void Player::setFlashing(bool flashing) {
    isFlashing = flashing;
    flashTime = 0.0f;
}

void Player::updateFlashing(float deltaTime) {
    if (isFlashing) {
        flashTime += deltaTime;
        float flashRate = 8.0f; // Flashes per second
        bool visible = static_cast<int>(flashTime * flashRate) % 2 == 0;
        
        if (visible) {
            updateColors(); // Use power colors when visible
        } else {
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineColor(sf::Color::Transparent);
        }
    } else {
        updateColors(); // Use power colors when not flashing
    }
}

void Player::setPowerState(PowerState state, float duration) {
    currentPowerState = state;
    powerTime = 0.0f;
    powerDuration = duration;
}

void Player::updatePowerState(float deltaTime) {
    if (powerDuration > 0) {
        powerTime += deltaTime;
        if (powerTime >= powerDuration) {
            currentPowerState = PowerState::Normal;
            powerDuration = 0.0f;
        }
    }
}

void Player::updateColors() {
    if (isFlashing) return; // Colors handled in updateFlashing
    
    switch (currentPowerState) {
        case PowerState::Normal:
            shape.setFillColor(sf::Color::White);
            shape.setOutlineColor(sf::Color::Cyan);
            break;
            
        case PowerState::SpeedBoost:
            // Blue for speed boost
            shape.setFillColor(sf::Color(100, 150, 255));
            shape.setOutlineColor(sf::Color::White);
            break;
            
        case PowerState::Invulnerable:
            // Golden for invulnerability
            shape.setFillColor(sf::Color::Yellow);
            shape.setOutlineColor(sf::Color::White);
            break;
            
        case PowerState::Charging:
            // Green for charging
            shape.setFillColor(sf::Color::Green);
            shape.setOutlineColor(sf::Color::White);
            break;
            
        case PowerState::Overcharged:
            // Red for overcharged
            shape.setFillColor(sf::Color::Red);
            shape.setOutlineColor(sf::Color::Yellow);
            break;
    }
} 