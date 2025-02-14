#pragma once

#include <SFML/Graphics.hpp>
#include "state.h"
#include "game.h"
#include "gui_engine_defs.h"

class SplashState : public State
{
public:
    SplashState(GameDataRef data);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);
    
private:
    GameDataRef m_data;

    sf::Clock m_clock;

    sf::Sprite m_background;
}; 