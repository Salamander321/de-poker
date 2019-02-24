//
// Created by tiwari-shaswat7
//

#include "card.hpp"

const std::string Card::suitString[SUITSIZE] = {"Spades", "Clubs", "Hearts", "Diamonds"};

const std::string Card::valueString[CARDVALUESIZE] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};

void Card::display()
{
	std::cout << m_suit << ", " << m_value << std::endl;
}