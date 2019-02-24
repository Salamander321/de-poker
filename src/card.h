//
// Created by tiwari-shaswat7
//

#pragma once

#include <iostream>
#include <string>

#include "game_defs.hpp"

// class to represent a card
class Card
{
public:
    // enum for card suits
    enum SUIT { SPADES, CLUBS, HEARTS, DIAMONDS };
    // enum for card values (ascending order of value)
    enum VALUE { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

private:
    SUIT m_suit;
    VALUE m_value;

public:
    // string representation for suit and cardvalue
	static const std::string suitString[SUITSIZE];
    static const std::string valueString[CARDVALUESIZE]; 

    // constructor
    Card() {}
    Card(SUIT new_suit, VALUE new_value) : m_suit(new_suit), m_value(new_value) {}

    SUIT getSuit() { return m_suit; }
    // void setSuit()

    VALUE getValue() { return m_value; }
    // void setValue()

    // convert suit and value to string
    std::string toString() { return suitString[m_suit] + ' ' + valueString[m_value]; }

	// display card (value, suit)
	void display();
};