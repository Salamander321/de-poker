#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "state_machine.h"
#include "asset_manager.h"
#include "input_manager.h"

struct GameData
{
    StateMachine machine;
    sf::RenderWindow window;
    AssetManager assets;
    InputManager input;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game
{
public:
    Game(int width, int height, std::string title);

private:
    const float dt = 1.0f / 60.0f;
    sf::Clock m_clock;

    GameDataRef m_data = std::make_shared<GameData>();

    void Run();

};