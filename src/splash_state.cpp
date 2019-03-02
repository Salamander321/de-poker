#include "splash_state.h"
#include "mainmenu_state.h"
#include <sstream>
#include <iostream>

SplashState::SplashState(GameDataRef data) : m_data(data)
{

}

void SplashState::Init()
{
    m_data->assets.loadTexture("Splash State Background", SPLASH_SCENE_BACKGROUND_FILEPATH);

    m_background.setTexture(m_data->assets.getTexture("Splash State Background"));
}

void SplashState::HandleInput()
{
    sf::Event event;
    while(m_data->window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_data->window.close();
        }
    }
}

void SplashState::Update(float dt)
{
    if(m_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME)
    {
        // Switch to the Main Menu
        m_data->machine.addState(StateRef (new MainMenuState(m_data)), true);
    }
}

void SplashState::Draw(float dt)
{
    m_data->window.clear();

    m_data->window.draw(m_background);

    m_data->window.display();
}