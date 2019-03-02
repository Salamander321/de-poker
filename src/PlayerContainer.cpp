//
// Created by salamander on 3/2/19.
//

#include "PlayerContainer.h"
#include "Network.h"

PlayerContainer::PlayerContainer()
{
    for (int i = 0; i < Network::peers.size();i++){
        gamePlayer[i].m_index = i;
        gamePlayer[i].m_name = Network::peersName[Network::peers[i]];
    }
    holecardIndex = 0;
    communityCardIndex = 0;
    playerNo = Network::peers.size();
}

void PlayerContainer::setHoleCards(int suit, int value)
{
        m_holeCards[holecardIndex].suit = suit;
        m_holeCards[holecardIndex].value = value;
        holecardIndex++;
}

void PlayerContainer::setCommunityCards(int suit, int value)
{
    m_communityCards[communityCardIndex].suit = suit;
    m_communityCards[communityCardIndex].value = value;
    communityCardIndex++;
}

void PlayerContainer::getHoleCards(cards holecards[])
{
    for (int i = 0; i < 2;i++){
        holecards[i] = m_holeCards[i];
    }
}

PlayerContainer::~PlayerContainer()
{

}