//
// Created by salamander on 2/22/19.
//

#include "Menu.h"

Menu::Menu(const std::string &title, const sf::Vector2u &menuSize) {
    menu_window.setup(title,menuSize);
    if(!font.loadFromFile("arial.ttf")){
        //error handling
    }
    sf::Vector2u size = menu_window.getSize();
    menu[0].setFont(font);
    menu[0].setColor(sf::Color::Red);
    menu[0].setString("SinglePlayer");
    menu[0].setPosition(sf::Vector2f(size.x/2-80,size.y /4 * 1));

    menu[1].setFont(font);
    menu[1].setColor(sf::Color::White);
    menu[1].setString("Multiplayer");
    menu[1].setPosition(sf::Vector2f(size.x/2-80,size.y/4*2));
    Draw();
}

Menu::~Menu() {
    menu_window.Destroy();
}

void Menu::Draw() {
    for (int i = 0;  i < 2; i++){
        menu_window.draw(menu[i]);
    }
}

bool Menu::menuOpen() {
    return menu_window.IsOpen();
}

void Menu::render() {
    menu_window.update();
    menu_window.beginDraw();
    Draw();
    menu_window.endDraw();
}