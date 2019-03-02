//
// Created by salamander on 2/21/19.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "windows.h"
#include "PlayerContainer.h"

sf::Vector2u tempSize(800,600);

int main(){
    PlayerContainer myContainer;
    myContainer.setHoleCards(2,4);
    myContainer.setHoleCards(5,6);
    cards Holecards[2];
    myContainer.getHoleCards(Holecards);
    for(int i = 0;i<2;i++)
    {
        std::cout<<Holecards[i].suit<<Holecards[i].value<<std::endl;

    }


    return 0;
}
