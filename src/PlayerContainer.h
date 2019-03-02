//
// Created by salamander on 3/2/19.
//

#ifndef DEPOKER_PLAYERCONTAINER_H
#define DEPOKER_PLAYERCONTAINER_H

#include "poker.h"

struct cards
{
    int suit,value;
    cards* operator = (cards c1)
    {
        this->suit = c1.suit;
        this->value = c1.value;
        return this;
    }
};

class PlayerContainer {
public:
    PlayerContainer();
    ~PlayerContainer();
    void setHoleCards(int suit, int value);
    void setCommunityCards(int suit, int value);
    void getHoleCards(cards []);

private:
    int holecardIndex;
    int communityCardIndex;
    cards m_holeCards[2];
    cards m_communityCards[5];
    int playerNo;
    cards m_winnerCards[5];
    struct players
    {
        int m_index;
        MONEY cash;
        std::string m_name;
        bool m_folded;
    };
    players gamePlayer[6];



};


#endif //DEPOKER_PLAYERCONTAINER_H
