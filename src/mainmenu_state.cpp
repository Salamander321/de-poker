#include <sstream>
#include "mainmenu_state.h"
#include "gui_engine_defs.h"
#include "game_state.h"
#include "Network.h"
#include "Lobby.h"

#include <iostream>

MainMenuState::MainMenuState(GameDataRef data)
    : m_data(data)
{

}

void MainMenuState::Init()
{
    m_data->assets.loadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH);
    m_data->assets.loadTexture("Play Button", MAIN_MENU_PLAY_BUTTON);
    m_data->assets.loadTexture("Play Button Outer", MAIN_MENU_PLAY_BUTTON_OUTER);

    m_background.setTexture(m_data->assets.getTexture("Background"));
    m_playButton.setTexture(m_data->assets.getTexture("Play Button"));
    m_playButtonOuter.setTexture(m_data->assets.getTexture("Play Button Outer"));

    m_playButton.setPosition((SCREEN_WIDTH / 2) - (m_playButton.getGlobalBounds().width / 2) - 300, 
        ((SCREEN_HEIGHT /2) - (m_playButton.getGlobalBounds().height / 2)));
    //m_playButton.setColor(sf::Color::Yellow);
    //m_playButtonOuter.setPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));

}

void MainMenuState::HandleInput()
{
    sf::Event event;

    while(m_data->window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_data->window.close();
        }
        if(m_data->input.isSpriteClicked(m_playButton, 
            sf::Mouse::Left, m_data->window))
        {
            m_playButton.setColor(COLOR_GREY);
            Game::listenStart = true;
            // Switch to Lobby State
            m_data->machine.addState(StateRef(new Lobby(m_data)), true);
        }
    }
}

void MainMenuState::Update(float dt)
{

}

void MainMenuState::Draw(float dt)
{
    m_data->window.clear();

    m_data->window.draw(m_background);
    m_data->window.draw(m_playButton);
    //m_data->window.draw(m_playButtonOuter);

    m_data->window.display();
}