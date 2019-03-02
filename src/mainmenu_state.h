#include <SFML/Graphics.hpp>
#include "state.h"
#include "game.h"

class MainMenuState : public State
{
public:
    MainMenuState(GameDataRef data);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

private:
    GameDataRef m_data;

    sf::Sprite m_background;

    sf::Sprite m_playButton;

    sf::Sprite m_playButtonOuter;
};