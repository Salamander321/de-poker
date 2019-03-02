//
// Created by salamander on 3/2/19.
//

#ifndef DEPOKER_LOBBY_H
#define DEPOKER_LOBBY_H

#include <SFML/Graphics.hpp>
#include "state.h"
#include "game.h"
#include "asset_manager.h"

class Lobby : public State
{
public:
    Lobby(GameDataRef data);

    void Init();

    void Update(float dt);
    void Draw(float dt);
    void HandleInput();

private:
    GameDataRef m_data;

    std::vector <sf::Text> m_textIP;
    std::vector <sf::Text> m_textName;
    sf::Text m_title;
    sf::Sprite m_background;

};


#endif //DEPOKER_LOBBY_H
