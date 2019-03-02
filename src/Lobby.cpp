//
// Created by salamander on 3/2/19.
//

#include "Lobby.h"
#include "gui_engine_defs.h"
#include "Network.h"

Lobby::Lobby(GameDataRef data)
    :m_data(data)
{

}

void Lobby::Init()
{
    m_data->assets.loadFont("Text Font",GAME_LOBBY_FONT_PATH);
    m_data->assets.loadTexture("Background",GAME_LOBBY_BACKGROUND_FILEPATH);

    m_background.setTexture(m_data->assets.getTexture("Background"));
    m_title.setFont(m_data->assets.getFont("Text Font"));
    m_title.setPosition((SCREEN_WIDTH*0.25),(SCREEN_HEIGHT*0.10));
    m_title.setFillColor((sf::Color::White));
    m_title.setString("Player List");
    m_title.setCharacterSize(30);

}
void Lobby::Update(float dt)
{
    sf::Text tempText;
    if (Network::peers.size() > 0 && m_textIP.size() != Network::peers.size())
    {
        for (int i = m_textIP.size(); i < Network::peers.size(); i++) {
            tempText.setFont(m_data->assets.getFont("Text Font"));
            tempText.setPosition((SCREEN_WIDTH * 0.15), (SCREEN_HEIGHT * (0.10 * (i + 2))));
            tempText.setFillColor((sf::Color::White));
            tempText.setString(Network::peers[i]);
            tempText.setCharacterSize(24);
            m_textIP.push_back(tempText);
            tempText.setFont(m_data->assets.getFont("Text Font"));
            tempText.setPosition((SCREEN_WIDTH * 0.35), (SCREEN_HEIGHT * (0.10 * (i + 2))));
            tempText.setFillColor((sf::Color::White));
            tempText.setString(Network::peersName[Network::peers[i]]);
            tempText.setCharacterSize(24);
            m_textName.push_back(tempText);
        }
    }

}

void Lobby::HandleInput()
{
    sf::Event event;

    while (m_data->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_data->window.close();
        }

    }
}

void Lobby::Draw(float dt)
{
    m_data->window.clear();
    m_data->window.draw(m_background);
    m_data->window.draw(m_title);
    for (int i = 0; i < m_textIP.size();i++)
    {
        m_data->window.draw(m_textIP[i]);
        m_data->window.draw(m_textName[i]);
    }
    m_data->window.display();
}