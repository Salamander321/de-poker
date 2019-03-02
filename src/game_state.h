#pragma once

#include <SFML/Graphics.hpp>
#include "state.h"
#include "game.h"
#include "card.h"
#include "poker.h"
#include "gui_engine_defs.h"
#include "PlayerContainer.h"
#include "Network.h"

class GameState : public State
{
public:
    GameState(GameDataRef data);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

private:
    GameDataRef m_data;

    // Player Container for Network
    PlayerContainer m_playerContainer;
    bool receivedHC;
    bool myTurn;

    // Background and Table
    sf::Sprite m_background;
    sf::Sprite m_table;

    // Deck of Cards
    sf::RectangleShape m_cardFront[SUITSIZE][CARDVALUESIZE];

    // Action Buttons
    sf::Sprite m_buttonAction[GAME_ACTION_BUTTON_COUNT];
    sf::Text m_buttonActionText[GAME_ACTION_BUTTON_COUNT];

    // Pot
    MONEY potMoney;
    sf::RectangleShape m_potRect;
    sf::Text m_potText;
    // Pot Stack
    sf::RectangleShape totalchipStack[30]; //total chips at the middle
    void pot(MONEY, int, int);

    // Card Back View Small
    sf::Sprite m_cardBack[GAME_PLAYER_SIZE * 2];

    // Card Distribute
    float posx[12];
    float posy[12];
    float angle[12];
    int ab;

    // Bet Slider
    struct 
    {
        sf::Sprite buttonPlus;
        sf::Sprite buttonMinus;
        sf::RectangleShape gap;
        MONEY money;
        sf::Text moneyText;
    } m_BetSlider;

    // Player Avatar
    struct Players
    {
        sf::Sprite Avatar;
        MONEY money;
        sf::Text moneyText;
        int serverIndex;
        sf::Text lastActionText;
        Player::ACTION lastAction;
        struct
        {
            sf::CircleShape circle1;
            sf::CircleShape circle2;
            sf::RectangleShape rectangle;
            sf::CircleShape avatarOutline;
        } InfoBox; 
    }; 
    Players m_Players[GAME_PLAYER_SIZE];

    // Display Card Front
    void setCardFront(Card::SUIT, Card::VALUE, int, int, bool);

    void buttonPressedColor(sf::Sprite &sprite)
    {
        sprite.setColor(COLOR_GREY);
    }
    void buttonPressedColor(sf::Sprite &sprite, sf::Text &text)
    {
        sprite.setColor(COLOR_GREY);
        text.setFillColor(COLOR_GREY);
    }
    void buttonReleasedColor(sf::Sprite &sprite)
    {
        sprite.setColor(sf::Color::White);
    }
    void buttonReleasedColor(sf::Sprite &sprite, sf::Text &text)
    {
        sprite.setColor(sf::Color::White);
        text.setFillColor(sf::Color::White);
    }

    // Money Text Update: Value and Position
    void updateMoneyText(MONEY &inMoney, sf::Text &inMoneyText, sf::RectangleShape &inRect)
    {
        inMoneyText.setString(std::to_string(inMoney));
        inMoneyText.setPosition(inRect.getPosition().x + ((inRect.getSize().x - inMoneyText.getGlobalBounds().width) / 2),
                inRect.getPosition().y + ((inRect.getSize().y - inMoneyText.getGlobalBounds().height) / 2) - 10);
    }

    // For Player Money Text Update: Value and Position
    void updateAvatarMoney(MONEY &inMoney, sf::Text &inMoneyText, sf::RectangleShape &inRect)
    {
        inMoneyText.setString("$" + std::to_string(inMoney));
        inMoneyText.setPosition(inRect.getPosition().x + ((inRect.getSize().x - inMoneyText.getGlobalBounds().width) / 2),
                                inRect.getPosition().y + ((inRect.getSize().y - inMoneyText.getGlobalBounds().height) / 2 - 5));
    }

    // Send Action to Server
    void sendAction(Player::ACTION action, MONEY money) {
        int Raction, Rvalue;
        Raction = (int)action;
        Rvalue = (int)money;
        Network::sendActionToServer(Raction, Rvalue);
        myTurn = false;
    }

    // To String
    std::string intoString(Player::ACTION action);

    // Card Distribute
    void cardDistribute();
};