#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font& font;
    std::string label;
    std::function<void()> onClick;
    
    // Visual states
    bool isHovered;
    bool isPressed;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    
public:
    Button(sf::Font& font, const std::string& label, float x, float y, float width, float height);
    
    void setOnClick(std::function<void()> callback);
    void update(const sf::Vector2f& mousePos, bool mousePressed);
    void draw(sf::RenderWindow& window);
    
    // Getters
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    
    // Setters
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed);
    void setTextSize(unsigned int size);
}; 