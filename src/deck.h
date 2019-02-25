#pragma once

#include "card.h"
#include "game_defs.h"

#include <iostream>
#include <random>
#include <ctime>

// class representation of a deck of 52 cards
class Deck
{
private:
    Card *m_cards[DECKSIZE];
    bool m_initialized;
    int m_topCardIndex;

    
public:
    Deck();

	~Deck();
    
    void init();

    void quickShuffle();
    //void riffleShuffle();		// might not be needed
	void fisherYatesShuffle();	// Fisher Yates Modern Shuffling ALgorithm

    Card* popCard();
	void burnCard();
    void getFlop(Card* &, Card* &, Card* &);
    Card* getTurn();
    Card* getRiver();
    
    // Utility Member Functions
    void swapCards(int, int);
    
    // Test Functions (display deck in a 13 x 4 grid
    void display();		
};