#include "Button.h"

Button::Button(sf::Font& font, const std::string& label, float x, float y, float width, float height)
    : font(font)
    , label(label)
    , isHovered(false)
    , isPressed(false)
    , normalColor(sf::Color(70, 70, 70, 200))
    , hoverColor(sf::Color(100, 100, 100, 200))
    , pressedColor(sf::Color(50, 50, 50, 200)) {
    
    // Setup shape
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(normalColor);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.0f);
    
    // Setup text
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    
    // Center text on button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (width - textBounds.width) / 2.0f,
        y + (height - textBounds.height) / 2.0f - 5.0f  // Slight offset for visual centering
    );
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::update(const sf::Vector2f& mousePos, bool mousePressed) {
    bool wasHovered = isHovered;
    isHovered = isMouseOver(mousePos);
    
    // Handle press state
    if (isHovered && mousePressed) {
        isPressed = true;
    } else if (!mousePressed) {
        if (isPressed && isHovered && onClick) {
            onClick();  // Trigger callback when button is released while hovering
        }
        isPressed = false;
    }
    
    // Update colors based on state
    if (isPressed) {
        shape.setFillColor(pressedColor);
    } else if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(normalColor);
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    
    // Update text position
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (shape.getSize().x - textBounds.width) / 2.0f,
        y + (shape.getSize().y - textBounds.height) / 2.0f - 5.0f
    );
}

void Button::setSize(float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    
    // Update text position
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        shape.getPosition().x + (width - textBounds.width) / 2.0f,
        shape.getPosition().y + (height - textBounds.height) / 2.0f - 5.0f
    );
}

void Button::setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed) {
    normalColor = normal;
    hoverColor = hover;
    pressedColor = pressed;
    
    // Update current color
    if (isPressed) {
        shape.setFillColor(pressedColor);
    } else if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(normalColor);
    }
}

void Button::setTextSize(unsigned int size) {
    text.setCharacterSize(size);
    
    // Re-center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        shape.getPosition().x + (shape.getSize().x - textBounds.width) / 2.0f,
        shape.getPosition().y + (shape.getSize().y - textBounds.height) / 2.0f - 5.0f
    );
} 