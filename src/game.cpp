#include "game.h"
#include "splash_state.h"
#include "asset_manager.h"

bool Game::listenStart = false;

Game::Game(int width, int height, std::string title)
{
    m_data->window.create(sf::VideoMode(width, height), title,
    sf::Style::Close | sf::Style::Titlebar);

    m_data->machine.addState(StateRef(new SplashState(m_data)));

    Run();
}

void Game::Run()
{
    float newTime, frameTime, interpolation;

    float currentTime = m_clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (m_data->window.isOpen())
    {
        m_data->machine.processStateChanges();

        newTime = m_clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;

        if(frameTime > 0.25f)
        {
            frameTime = 0.25f;
        }

        currentTime = newTime;
        accumulator += frameTime;

        while(accumulator >= dt)
        {
            m_data->machine.getActiveState()->HandleInput();
            m_data->machine.getActiveState()->Update(dt);

            accumulator -= dt;
        }

        interpolation = accumulator / dt;
        m_data->machine.getActiveState()->Draw(interpolation);
    }
}
