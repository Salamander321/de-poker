//
// Created by salamander on 2/22/19.
//
#include "windows.h"
#ifndef DEPOKER_MENU_H
#define DEPOKER_MENU_H


class Menu {
private:
    Window menu_window;
    int selectedItem;
    sf::Font font;
    sf::Text menu[3];
public:
    Menu(const std::string& title, const sf::Vector2u& menuSize);
    ~Menu();
    void Draw();
    void moveUp();
    void moveDown();
    int getPressedItem();
    bool menuOpen();
    void render();
};


#endif //DEPOKER_MENU_H
