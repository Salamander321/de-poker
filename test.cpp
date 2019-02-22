//
// Created by salamander on 2/21/19.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "windows.h"

sf::Vector2u tempSize(800,600);

int main(){

    Menu mymenu("DePoker",tempSize);
    while(mymenu.menuOpen()){
        mymenu.render();

    }
    return 0;
}
