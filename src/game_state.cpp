#include <sstream>
#include "game_state.h"
#include "gui_engine_defs.h"
#include "player.h"

#include <iostream>
#include <string>

GameState::GameState(GameDataRef data)
    : m_data(data)
{
    receivedHC = false;
    myTurn = false;
    commCard = false;
    turnCard = false;
    riverCard = false;
    displayWinner = false;
}

void GameState::Init()
{
    // Load Board

    // Load Fonts
    m_data->assets.loadFont("Anke Sans", GAME_FONT);

    // Load Textures
    m_data->assets.loadTexture("Background", GAME_BACKGROUND_FILEPATH);
    m_data->assets.loadTexture("Table", GAME_TABLE_FILEPATH);
    m_data->assets.loadTexture("Card Back", GAME_CARD_BACK_FILEPATH);
    m_data->assets.loadTexture("Button Action", GAME_BUTTON_FILEPATH);
    m_data->assets.loadTexture("Button Plus", GAME_BUTTON_PLUS);
    m_data->assets.loadTexture("Button Minus", GAME_BUTTON_MINUS);
    m_data->assets.loadTexture("Chips", GAME_CHIPS_FILEPATH);

    // Load Cards 
    for(int i = 0; i < DECKSIZE; i++)
    {
        m_data->assets.loadTexture(("Card " + std::to_string(i)),
            std::string(GAME_CARD_PATH) + "Card" + std::to_string(i+1) + ".png"); 
    }
    for(int i = 0; i < SUITSIZE; i++)
    {
        for(int j = 0; j < CARDVALUESIZE; j++)
        {
            m_cardFront[i][j].setTexture(&m_data->assets.getTexture("Card " + std::to_string(i * CARDVALUESIZE + j)));
        }
    }
    // Single Texture Test  
    /*m_data->assets.loadTexture(("Card " + std::to_string(0)),
            std::string(GAME_CARD_PATH) + "Card" + std::to_string(1) + ".png"); 

    m_cardFront[0][0].setTexture(&m_data->assets.getTexture("Card " + std::to_string(0)));
    m_cardFront[0][0].setSize(sf::Vector2f(53.0f,85.0f));
    m_cardFront[0][0].setPosition(0 ,200);
    */

    // Players
    // Set Server Index
    for (int i = 0; i < Network::peers.size(); i++)
    {
        if(sf::IpAddress::getLocalAddress().toString() == Network::peers[i])
        {
            m_Players[0].serverIndex = i;
            break;
        }
    }

    int index_player = (m_Players[0].serverIndex + 1) % Network::peers.size();
    int posIndex = 1;
    while(index_player != m_Players[0].serverIndex)
    {
        m_Players[posIndex].serverIndex = index_player++;
        if(index_player == Network::peers.size())
        {
            index_player = 0;
        }
    }

    // Set Position
    m_Players[0].Avatar.setPosition(375,475);
    m_Players[1].Avatar.setPosition(40,425);
    m_Players[2].Avatar.setPosition(50,70);
    m_Players[3].Avatar.setPosition(410,8);
    m_Players[4].Avatar.setPosition(780,110);
    m_Players[5].Avatar.setPosition(770,460);
    for (int i = 0; i < GAME_PLAYER_SIZE; i++)
    {
        m_data->assets.loadTexture(("Avatar" + std::to_string(i)), 
            std::string(GAME_PLAYER_AVATAR_FILEPATH) + "player" + std::to_string(i) + ".png");
        m_Players[i].Avatar.setTexture(m_data->assets.getTexture("Avatar" + std::to_string(i)));
    
        m_Players[i].InfoBox.rectangle.setSize(sf::Vector2f(60, 25));
        m_Players[i].InfoBox.rectangle.setFillColor(sf::Color::White);
        m_Players[i].InfoBox.rectangle.setPosition(m_Players[i].Avatar.getPosition().x + 30,
                m_Players[i].Avatar.getPosition().y + 100);
        m_Players[i].InfoBox.circle1.setRadius(12.5f);
        m_Players[i].InfoBox.circle1.setFillColor(sf::Color::White);
        m_Players[i].InfoBox.circle1.setPosition(m_Players[i].InfoBox.rectangle.getPosition().x - 12.5,
                m_Players[i].InfoBox.rectangle.getPosition().y);
        m_Players[i].InfoBox.circle2.setRadius(12.5f);
        m_Players[i].InfoBox.circle2.setFillColor(sf::Color::White);
        m_Players[i].InfoBox.circle2.setPosition(m_Players[i].InfoBox.rectangle.getPosition().x + 
                m_Players[i].InfoBox.rectangle.getSize().x - 12.5f, m_Players[i].InfoBox.rectangle.getPosition().y);
        m_Players[i].InfoBox.avatarOutline.setPointCount(120);        
        m_Players[i].InfoBox.avatarOutline.setRadius(m_Players[i].Avatar.getGlobalBounds().width / 2);
        m_Players[i].InfoBox.avatarOutline.setFillColor(COLOR_TRANSPARENT);
        m_Players[i].InfoBox.avatarOutline.setOutlineColor(COLOR_GREY_TRANSPARENT);
        m_Players[i].InfoBox.avatarOutline.setOutlineThickness(7.0f);
        m_Players[i].InfoBox.avatarOutline.setPosition(m_Players[i].Avatar.getGlobalBounds().left,
                m_Players[i].Avatar.getGlobalBounds().top);

        // Money
        m_Players[i].money = 10000;
        m_Players[i].moneyText.setString("$10000");
        m_Players[i].moneyText.setFont(m_data->assets.getFont("Anke Sans"));
        m_Players[i].moneyText.setFillColor(sf::Color::Black);
        m_Players[i].moneyText.setCharacterSize(15);
        updateMoneyText(m_Players[i].money, m_Players[i].moneyText, m_Players[i].InfoBox.rectangle);

        // Last Action
        m_Players[i].lastActionText.setFont(m_data->assets.getFont("Anke Sans"));
        m_Players[i].lastActionText.setString("  ");
        m_Players[i].lastActionText.setFillColor(COLOR_GREY_TRANSPARENT);
        m_Players[i].lastActionText.setStyle(sf::Text::Bold);
        m_Players[i].lastActionText.setCharacterSize(15);
        m_Players[i].lastActionText.setPosition(m_Players[i].InfoBox.rectangle.getPosition().x + ((m_Players[i].InfoBox.rectangle.getSize().x - m_Players[i].lastActionText.getGlobalBounds().width) / 2),
                                                m_Players[i].InfoBox.rectangle.getPosition().y - m_Players[i].InfoBox.rectangle.getSize().y);
        //set pos
    }

    // Card Back View
    for(int i = 0; i < (PLAYERSIZE * 2); i++) 
    {    
        m_cardBack[i].setTexture(m_data->assets.getTexture("Card Back"));
    }

    // Background Image
    m_background.setTexture(m_data->assets.getTexture("Background"));

    // Table
    m_table.setTexture(m_data->assets.getTexture("Table"));

    // Pot
    potMoney = 0;
    m_potRect.setPosition(325,230);
    m_potRect.setSize(sf::Vector2f(300,50));
    m_potRect.setFillColor(sf::Color(0,0,0,75));
    m_potText.setFont(m_data->assets.getFont("Anke Sans"));
    m_potText.setString("  POT $" + std::to_string(potMoney));
    m_potText.setCharacterSize(25);
    m_potText.setStyle(sf::Text::Bold);
    m_potText.setPosition(460,235);
    m_potText.setFillColor(sf::Color(152,160,152));
    // Pot Stack
    for (int j = 0; j < 30; j++) 
    {
        totalchipStack[j].setTexture(&m_data->assets.getTexture("Chips"));
        totalchipStack[j].setSize(sf::Vector2f(40.0f, 35.0f));
    }//for total stack of chips
    
    // Action Button
    float buttonPositionX = 275.0f;
    float buttonPositionY = 615.0f;
    float buttonOffset = 140.0f;
    for(int i = 0; i < GAME_ACTION_BUTTON_COUNT; i++)
    {
        m_buttonAction[i].setTexture(m_data->assets.getTexture("Button Action"));
        m_buttonAction[i].setPosition(buttonPositionX, buttonPositionY);
        buttonPositionX += buttonOffset;
        //m_buttonAction.setScale(sf::Vector2f(85.0f, 40.0f));
    }
    // Action Button Text
    m_buttonActionText[Player::CALL].setString("CALL");
    m_buttonActionText[Player::RAISE].setString("RAISE");
    m_buttonActionText[Player::FOLD].setString("FOLD");
    for(int i = 0; i < GAME_ACTION_BUTTON_COUNT; i++)
    {
        m_buttonActionText[i].setFont(m_data->assets.getFont("Anke Sans"));
        m_buttonActionText[i].setFillColor(sf::Color::White);
        m_buttonActionText[i].setCharacterSize(25);
        m_buttonActionText[i].setPosition(m_buttonAction[i].getPosition().x + 30,
            m_buttonAction[i].getPosition().y + 5);
    }

    // Bet Slider
    m_BetSlider.buttonPlus.setTexture(m_data->assets.getTexture("Button Plus"));
    m_BetSlider.buttonPlus.setPosition(715, 612);
    m_BetSlider.buttonMinus.setTexture(m_data->assets.getTexture("Button Minus"));
    m_BetSlider.buttonMinus.setPosition(865, 612);
    m_BetSlider.gap.setSize(sf::Vector2f(150, 45));
    m_BetSlider.gap.setFillColor(sf::Color::Black);
    m_BetSlider.gap.setPosition(740, 615);

    m_BetSlider.money = static_cast<MONEY>(GAME_RAISE);
    m_BetSlider.moneyText.setFont(m_data->assets.getFont("Anke Sans"));
    m_BetSlider.moneyText.setFillColor(sf::Color::White);
    m_BetSlider.moneyText.setCharacterSize(30);
    updateMoneyText(m_BetSlider.money, m_BetSlider.moneyText, m_BetSlider.gap);

    // Card Distribute
    // setup for card distribution
    ab = 0;
    for(int i = 0; i < 12; i++)
    {
        posx[i] = 200;
        posy[i] = 150;
        angle[i] = 0;
    }
    m_cardBack[6].setPosition(150, 170);
    m_cardBack[6].setRotation(-20);
    m_cardBack[7].setPosition(180, 170);
    m_cardBack[7].setRotation(-20);

    // For community cards
    for(int i=0;i<4;i++)
        middlecardposx[i]=50;
}

void GameState::HandleInput()
{
    sf::Event event;

    while(m_data->window.pollEvent(event))
    {
        // Close Window
        if(event.type == sf::Event::Closed)
        {
            m_data->window.close();
        }
        // Mouse Postition
        if(event.type == sf::Event::MouseMoved)
        {
            std::cout << event.mouseMove.x << ", " << event.mouseMove.y << std::endl;
        }
        // Action Button
        // CALL
        if(m_data->input.isSpriteClicked(m_buttonAction[Player::CALL], 
            sf::Mouse::Left, m_data->window))
        {
            buttonPressedColor(m_buttonAction[Player::CALL], m_buttonActionText[Player::CALL]);
            m_BetSlider.money = GAME_RAISE;
            updateMoneyText(m_BetSlider.money, m_BetSlider.moneyText, m_BetSlider.gap);
            std::cout << "Call" << std::endl;
            sendAction(Player::CALL, 0);
        }
        else
        {
            buttonReleasedColor(m_buttonAction[Player::CALL], m_buttonActionText[Player::CALL]);
        }
        // RAISE
        if(m_data->input.isSpriteClicked(m_buttonAction[Player::RAISE], 
            sf::Mouse::Left, m_data->window))
        {
            buttonPressedColor(m_buttonAction[Player::RAISE], m_buttonActionText[Player::RAISE]);
            m_BetSlider.money = GAME_RAISE;
            updateMoneyText(m_BetSlider.money, m_BetSlider.moneyText, m_BetSlider.gap);
            std::cout << "Raise" << std::endl;
            std::cout << m_BetSlider.money << std::endl;
            sendAction(Player::RAISE, m_BetSlider.money);
        }
        else
        {
            buttonReleasedColor(m_buttonAction[Player::RAISE], m_buttonActionText[Player::RAISE]);
        }
        // FOLD
        if(m_data->input.isSpriteClicked(m_buttonAction[Player::FOLD], 
            sf::Mouse::Left, m_data->window))
        {
            buttonPressedColor(m_buttonAction[Player::FOLD], m_buttonActionText[Player::FOLD]);
            m_BetSlider.money = GAME_RAISE;
            updateMoneyText(m_BetSlider.money, m_BetSlider.moneyText, m_BetSlider.gap);
            std::cout << "Fold" << std::endl;
            sendAction(Player::FOLD, 0);
        }
        else
        {
            buttonReleasedColor(m_buttonAction[Player::FOLD], m_buttonActionText[Player::FOLD]);
        }
        // Bet Slider
        // Plus Button
        if(m_data->input.isSpriteClicked(m_BetSlider.buttonPlus, 
            sf::Mouse::Left, m_data->window))
        {
            buttonPressedColor(m_BetSlider.buttonPlus);
            m_BetSlider.money += GAME_RAISE_UNIT;
            // Bet Slider Money Text Position
            updateMoneyText(m_BetSlider.money, m_BetSlider.moneyText, m_BetSlider.gap);
        }
        else
        {
            buttonReleasedColor(m_BetSlider.buttonPlus);
        }
        // Minus Button
        if(m_data->input.isSpriteClicked(m_BetSlider.buttonMinus, 
            sf::Mouse::Left, m_data->window))
        {
            buttonPressedColor(m_BetSlider.buttonMinus);
            if((static_cast<int>(m_BetSlider.money) - GAME_RAISE_UNIT) < GAME_RAISE)
            {
                m_BetSlider.money == static_cast<MONEY>(GAME_RAISE);
            }
            else
            {
                m_BetSlider.money -= GAME_RAISE_UNIT;
            }
            // Bet Slider Money Text Position
            updateMoneyText(m_BetSlider.money, m_BetSlider.moneyText, m_BetSlider.gap);
        }
        else
        {
            buttonReleasedColor(m_BetSlider.buttonMinus);
        }
    }
}

void GameState::Update(float dt)
{
    if(Network::receiveData)
    {
        if(Network::rcvType == "POT")
        {
            /*
             * Network::value1 is value of Pot
             * Also Don't forgot to Network::receivedData = false once finished instantly
             */
            potMoney = static_cast<MONEY>(Network::value1);
            //std::cout<<"Pot Money is "<<Network::value1<<std::endl;
            Network::receiveData = false;
        } else if (Network::rcvType == "HC")
        {
            static int call = 0;
            /*
             *This is for Hole card. Two cards will be send on two different packets
             * Its for suit and value respectively
             */
            m_playerContainer.setHoleCards(Network::value1, Network::value2);
            //std::cout<<Network::value1<<" : "<<Network::value2<<std::endl;
            // Set Hole Cards
            if(call == 0) {
                setCardFront(static_cast<Card::SUIT>(m_playerContainer.m_holeCards[0].suit),
                             static_cast<Card::VALUE>(m_playerContainer.m_holeCards[0].value), 490, 475, true);
                call++;
            }
            else if(call == 1) {
                setCardFront(static_cast<Card::SUIT>(m_playerContainer.m_holeCards[1].suit),
                             static_cast<Card::VALUE>(m_playerContainer.m_holeCards[1].value), 585, 475, true);
                call = 0;
                receivedHC = true;
            }
            /*setCardFront(Card::SPADES, Card::TWO , 515, 475, true);
            setCardFront(Card::DIAMONDS, Card::ACE , 515, 475, true);*/
            Network::receiveData = false;
        } else if( Network::rcvType == "GAT")
        {
            /*
             * This is for send type action calling user for raise , call bet and fold
             */
            myTurn = true;
            Network::receiveData = false;
            /*int Raction,Rvalue;
            std::cout<<"Action Request has been Sent by server"<<std::endl;

            std::cout<<"0.CAll 1.RAISE 2.FOLD 3.CHECK 4.BET"<<std::endl;
            std::cin>>Raction;
            std::cout<<"Enter a value"<<std::endl;
            std::cin>>Rvalue;
            Network::sendActionToServer(Raction, Rvalue);*/


        } else if(Network::rcvType == "CMABC")
        {
            /*
             * This is for community card so will be send in continous three way. Packet loss is possible
             * Suit and Value Format
             */
            static int comCardIndex = 0;
            m_communityCards[comCardIndex].suit = Network::value1;
            m_communityCards[comCardIndex].value = Network::value2;
            if(comCardIndex == 2)
            {
                commCard = true;
            }
            if(comCardIndex == 3)
            {
                turnCard = true;
            }
            if(comCardIndex == 4)
            {
                riverCard = true;
            }
            comCardIndex++;



            //std::cout<<Network::value1<<" : "<<Network::value2<<std::endl;

            Network::receiveData = false;
        } else if(Network::action == 114)
        {
            /*
             * This is for getting winners Name;
             */
            std::cout<<"Winner is : "<<Network::rcvType<<std::endl;
            Network::receiveData = false;
        } else if(Network::rcvType == "RANK")
        {
            /*
             * This is for Player getting their own Rank
             */
            std::cout<<"Rank is : "<<Network::value1<<std::endl;
            Network::receiveData  = false;
        } else if(Network::rcvType == "BSTPACK")
        {
            std::cout<<"Best Card is : "<<Network::value1<<" : "<<Network::value2<<std::endl;
            Network::receiveData = false;
        } else if(Network::rcvType == "PGAT")
        {
            for(int i = 0; i < Network::peers.size(); i++)
            {
                if(m_Players[i].serverIndex == Network::value1)
                {
                    m_Players[(Network::peers.size() + i - 1) % Network::peers.size()].InfoBox.avatarOutline.setOutlineColor(COLOR_YELLOW_ACTIVE);
                    m_Players[i].InfoBox.avatarOutline.setOutlineColor(COLOR_GREY_TRANSPARENT);

                    m_Players[i].lastAction = (Player::ACTION) Network::value2;
                    m_Players[i].lastActionText.setString(intoString(m_Players[i].lastAction));

                    m_Players[i].moneyText.setString(std::to_string(Network::value3));
                }
            }
            //std::cout<<"Player "<<Network::peersName[Network::peers[Network::value1]]<<" has "<<Network::value2<<std::endl;
            Network::receiveData = false;

        }
    }

    if(commCard)
    {
        setCommunityCard();
    }
    if(turnCard)
    {
        setTurn();
    }
    if(riverCard)
    {
        setRiver();
    }
    cardDistribute();
    m_potText.setString("POT  $" + std::to_string(potMoney));
    pot(potMoney, 335, 248);

}

void GameState::Draw(float dt)
{
    m_data->window.clear();
    // Background
    m_data->window.draw(m_background);
    m_data->window.draw(m_table);
    m_data->window.draw(m_potRect);
    m_data->window.draw(m_potText);
    for(int i = 0; i < 30; i++)
    {
        m_data->window.draw(totalchipStack[i]);
    }

    // Actions
    if(myTurn) {
        for (int i = 0; i < GAME_ACTION_BUTTON_COUNT; i++) {
            m_data->window.draw(m_buttonAction[i]);
            m_data->window.draw(m_buttonActionText[i]);
        }
        m_data->window.draw(m_BetSlider.gap);
        m_data->window.draw(m_BetSlider.buttonPlus);
        m_data->window.draw(m_BetSlider.buttonMinus);
        m_data->window.draw(m_BetSlider.moneyText);
    }
    // Player Avatar
    for(int i = 0; i < GAME_PLAYER_SIZE; i++)
    {
        m_data->window.draw(m_Players[i].Avatar);
        m_data->window.draw(m_Players[i].InfoBox.avatarOutline);
        m_data->window.draw(m_Players[i].InfoBox.circle1);
        m_data->window.draw(m_Players[i].InfoBox.circle2);
        m_data->window.draw(m_Players[i].InfoBox.rectangle);
        m_data->window.draw(m_Players[i].moneyText);
        m_data->window.draw(m_Players[i].lastActionText);
    }

    // Draw Card
    for (int i = 0; i < (GAME_PLAYER_SIZE * 2); i++)
    {
        m_data->window.draw(m_cardBack[i]);
    }

    if(receivedHC) {
        m_data->window.draw(m_cardFront[(Card::SUIT)m_playerContainer.m_holeCards[0].suit][(Card::VALUE)m_playerContainer.m_holeCards[0].value]);
        m_data->window.draw(m_cardFront[(Card::SUIT)m_playerContainer.m_holeCards[1].suit][(Card::VALUE)m_playerContainer.m_holeCards[1].value]);

    }

    if(commCard)
    {
        for (int i = 0; i < 3; i++)
            m_data->window.draw(m_cardFront[m_communityCards[i].suit][m_communityCards[i].value]);
    }

    if(turnCard)
    {
        m_data->window.draw(m_cardFront[m_communityCards[3].suit][m_communityCards[3].value]);
    }

    if(riverCard)
    {
        m_data->window.draw(m_cardFront[m_communityCards[4].suit][m_communityCards[4].value]);
    }


    m_data->window.display();
}

// Set Card Front
void GameState::setCardFront(Card::SUIT suit, Card::VALUE value, 
        int posX, int posY, bool isLarge = false)
{
    if(isLarge)
    {
        m_cardFront[suit][value].setSize(sf::Vector2f(90.0f,130.0f));
    }
    else
    {
        m_cardFront[suit][value].setSize(sf::Vector2f(53.0f,85.0f));
    }
        m_cardFront[suit][value].setPosition(posX, posY);
}

// Player::ACTION to string
std::string GameState::intoString(Player::ACTION action)
{
    switch(action)
    {
        case Player::CALL:
            return "CALL";

        case Player::RAISE:
            return "RAISE";

        case Player::FOLD:
            return "FOLD";

        case Player::CHECK:
            return "CHECK";

        case Player::BET:
            return "BET";

        default:
            return " ";
    }
}

// Community Card
void GameState::setCommunityCard() {
    static int bc = 0;
    if (middlecardposx[0] <= 322) {
        setCardFront((Card::SUIT)m_communityCards[0].suit, (Card::VALUE)m_communityCards[0].value, middlecardposx[0], 287.0f);
        middlecardposx[0] += 3;
        bc++;

        if (middlecardposx[0] >= 322) {
            setCardFront((Card::SUIT)m_communityCards[0].suit, (Card::VALUE)m_communityCards[0].value, 323, 287.0f);
            //m_cardFront[m_communityCards[0].suit][m_communityCards[0].value].setPosition(323, 287.0f);
            // m_cardFront[suit1][value1].setFillColor(sf::Color(255,255,255,255));

        }
    }
    if (middlecardposx[1] <= 385 && bc > 30) {
        setCardFront((Card::SUIT)m_communityCards[1].suit, (Card::VALUE)m_communityCards[1].value, middlecardposx[1], 287.0f);

        middlecardposx[1] += 3;
        bc++;

        if (middlecardposx[1] >= 385) {
            setCardFront((Card::SUIT)m_communityCards[1].suit, (Card::VALUE)m_communityCards[1].value, 386, 287.0f);

            //m_cardFront[m_communityCards[1].suit][m_communityCards[1].value].setPosition(386, 287.0f);
        }
    }

    if (middlecardposx[2] <= 448 && bc > 100) {
        setCardFront((Card::SUIT)m_communityCards[2].suit, (Card::VALUE)m_communityCards[2].value, middlecardposx[2], 287.0f);

        middlecardposx[2] += 3;
        bc++;


        if (middlecardposx[2] >= 448) {
            setCardFront((Card::SUIT)m_communityCards[2].suit, (Card::VALUE)m_communityCards[2].value, 448, 287.0f);

            //m_cardFront[m_communityCards[1].suit][m_communityCards[1].value].setPosition(448, 287.0f);
        }
    }
}

void GameState::setTurn()
{

    if(middlecardposx[3]<=511)// && bc >180){
        setCardFront((Card::SUIT)m_communityCards[3].suit, (Card::VALUE)m_communityCards[3].value, middlecardposx[3], 287.0f);
        //m_cardFront[suit4][value4].setPosition(middlecardposx[3],287.0f);
        middlecardposx[3]+=3;
        //bc++;


        if(middlecardposx[3]>=511) {
            setCardFront((Card::SUIT)m_communityCards[3].suit, (Card::VALUE)m_communityCards[3].value, 511, 287.0f);
            //arrangedcards[suit5][value5].setPosition(511,287.0f);
        }
}


void GameState::setRiver()
{

    if(middlecardposx[4]<=574)// && bc >180){
        setCardFront((Card::SUIT)m_communityCards[4].suit, (Card::VALUE)m_communityCards[4].value, middlecardposx[4], 287.0f);
    //m_cardFront[suit4][value4].setPosition(middlecardposx[3],287.0f);
    middlecardposx[4]+=3;
    //bc++;


    if(middlecardposx[4]>=574) {
        setCardFront((Card::SUIT)m_communityCards[4].suit, (Card::VALUE)m_communityCards[4].value, 574, 287.0f);
        //arrangedcards[suit5][value5].setPosition(511,287.0f);
    }
}


// Card Distribute Function
void GameState::cardDistribute() 
{
    if(posx[1]<=510) {
        m_cardBack[1].setPosition(posx[1], posy[1]);
        m_cardBack[1].setRotation(angle[1]);
        angle[1] +=5;
        posx[1]+=7;
        posy[1]+=1.0f;
        ab++;
        if(posx[1]>=510)
        {
            m_cardBack[1].setPosition(510, 140);
            m_cardBack[1].setRotation(0);
        }}

    if(posx[0]<490 && ab>20) {
        m_cardBack[0].setPosition(posx[0], posy[0]);
        m_cardBack[0].setRotation(angle[0]);
        angle[0] +=7;
        posx[0]+=8;
        posy[0]+=1.0f;
        ab++;
        if(posx[0]>=490)
        {
            m_cardBack[0].setPosition(490, 140);
            m_cardBack[0].setRotation(0);
        }}

    if(posx[2]<=700 && ab>50){
        m_cardBack[2].setPosition(posx[2],posy[2]);
        m_cardBack[2].setRotation(angle[2]);
        angle[2]+=17.5 ;
        posx[2]+=8;
        ab++;
        // posy+=5.0f;
        if(posx[2]>=700)
        {
            m_cardBack[2].setPosition(760, 170);
            m_cardBack[2].setRotation(20);
        }}


    if(posx[3]<=1000 && ab>90){
        m_cardBack[3].setPosition(posx[3],posy[3]);
        m_cardBack[3].setRotation(angle[3]);
        angle[3]+=17 ;
        posx[3]+=8;
        ab++;
        // posy+=5.0f;
        if(posx[2]>=700)
        {
            m_cardBack[3].setPosition(730, 170);
            m_cardBack[3].setRotation(20);
        }}

    if(posx[4]<=690 && ab>140){
        m_cardBack[4].setPosition(posx[4],posy[4]);
        m_cardBack[4].setRotation(angle[4]);
        angle[4]+=16;
        posx[4]+=7.5;
        posy[4]+=5.0f;
        ab++;

    if(posx[4]>=690) {
        m_cardBack[4].setPosition(700, 475);
        m_cardBack[4].setRotation(-20);
    }}



    if(posx[5]<=690 &&ab>=170){
       // std::this_thread::sleep_for(1ms);
        m_cardBack[5].setPosition(posx[5],posy[5]);
        m_cardBack[5].setRotation(angle[5]);
        angle[5]+=16;
        posx[5]+=7.5;
        posy[5]+=5.0f;
        ab++;
    if(posx[5]>=690) {
        m_cardBack[5].setPosition(730, 475);
        m_cardBack[5].setRotation(-20);
    }}

    if(posx[10]<=490 &&ab>=200){
       // std::this_thread::sleep_for(1ms);
        m_cardBack[10].setPosition(posx[10],posy[10]);
        m_cardBack[10].setRotation(angle[10]);
        angle[10]+=16;
        posx[10]+=4.5f;
        posy[10]+=5.5f;
        ab++;
    if(posx[10]>=490) {
        m_cardBack[10].setPosition(490, 525);
        m_cardBack[10].setRotation(0);
    }}
    if(posx[11]<=510 &&ab>=260){
       // std::this_thread::sleep_for(1ms);
        m_cardBack[11].setPosition(posx[11],posy[11]);
        m_cardBack[11].setRotation(angle[11]);
        angle[11]+=12;
        posx[11]+=5.0f;
        posy[11]+=5.0f;
        ab++;
    if(posx[11]>=510) {
        m_cardBack[11].setPosition(510, 525);
        m_cardBack[11].setRotation(0);
    }}

    if(posy[9]<=450 && ab>340) {
        m_cardBack[9].setPosition(posx[9], posy[9]);
        m_cardBack[9].setRotation(angle[9]);
        angle[9] +=10;
        ab++;
        posy[9]+=8.0f;
        if(posy[9]>=450)
        {
            m_cardBack[9].setPosition(190, 440);
            m_cardBack[9].setRotation(20);
        }}

    if(posy[8]<=450 && ab>370) {
        m_cardBack[8].setPosition(posx[8], posy[8]);
        m_cardBack[8].setRotation(angle[8]);
        angle[8] +=10;
       // posx[8]+=8;
        posy[8]+=8.0f;
        if(posy[8]>=450)
        {
            m_cardBack[8].setPosition(160, 450);
            m_cardBack[8].setRotation(20);
        }}


}

//for chips at the middle
void GameState::pot(MONEY money, int bottomPositionX, int bottomPositionY)
{
    int tempmoney=money;
    int thousand,fivehundred,hundred,fifty,ten;
    thousand=tempmoney/1000;
    tempmoney=tempmoney%1000;
    fivehundred=tempmoney/500;
    tempmoney=tempmoney%500;
    hundred=tempmoney/100;
    tempmoney=tempmoney%100;
    fifty=tempmoney/50;
    tempmoney=tempmoney%50;
    ten=tempmoney/10;

    sf::Texture texture = m_data->assets.getTexture("Chips");
    sf::Vector2u textureSize2 = texture.getSize();
    textureSize2.x /= 7;
    textureSize2.y /= 2;

    int kk = 0;
    int vv = bottomPositionY;
    int mm = bottomPositionY - 12;
    for (int i = 0; i < fivehundred; i++) {
        //  int v=490;
        totalchipStack[kk].setTextureRect(sf::IntRect(textureSize2.x * 3, textureSize2.y * 0, textureSize2.x, textureSize2.y));
        totalchipStack[kk].setPosition(bottomPositionX + 25, vv);
        vv = vv - 10;
        kk++;
    }        
    mm = bottomPositionY - 12;
    vv = bottomPositionY;

    for (int i = 0; i < fifty; i++) {
        //  int m=490;
        totalchipStack[kk].setTextureRect(sf::IntRect(textureSize2.x * 5, textureSize2.y * 0, textureSize2.x, textureSize2.y));
        totalchipStack[kk].setPosition(bottomPositionX + 75, mm);
        mm = mm - 10;
        kk++;
    }
    mm = bottomPositionY - 12;
    vv = bottomPositionY;
    
    for (int i = 0; i < thousand; i++) {
        //int u=490;
        totalchipStack[kk].setTextureRect(sf::IntRect(textureSize2.x * 2, textureSize2.y * 0, textureSize2.x, textureSize2.y));
        totalchipStack[kk].setPosition(bottomPositionX, vv);
        vv = vv- 10;
        kk++;
    }
    mm = bottomPositionY - 12;
    vv = bottomPositionY;
    for (int i = 0; i < hundred; i++) {
        //  int x=490;
        totalchipStack[kk].setTextureRect(sf::IntRect(textureSize2.x * 4, textureSize2.y * 0, textureSize2.x, textureSize2.y));
        totalchipStack[kk].setPosition(bottomPositionX + 50, mm);
        mm = mm - 10;
        kk++;
    }

    for (int i = 0; i < ten; i++) {
        //int z=490;
        totalchipStack[kk].setTextureRect(sf::IntRect(textureSize2.x * 6, textureSize2.y * 0, textureSize2.x, textureSize2.y));
        totalchipStack[kk].setPosition(bottomPositionX + 100, vv);
        vv = vv - 10;
        kk++;
    }
}
