#pragma once

#include <SFML/Graphics.hpp>

class InputManager
{
public:
    InputManager() {}
    ~InputManager() {}

    bool isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, 
        sf::RenderWindow &window);
    sf::Vector2i getMousePosition(sf::RenderWindow &window);
};