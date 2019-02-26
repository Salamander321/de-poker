#pragma once

#include "game_defs.h"
#include "card.h"

#include <array>
#include <deque>
#include <vector>
#include <list>
#include <algorithm>

class Hand
{
public:
	enum RANK{ROYALFLUSH, STRAIGHTFLUSH, FOUROFAKIND, FULLHOUSE,
		FLUSH, STRAIGHT, THREEOFAKIND, TWOPAIRS, PAIR, HIGHCARD};

private:
	std::array<Card *, SEVENCARDHANDSIZE> m_sevenCardHand;
	std::array<Card *, FIVECARDHANDSIZE> m_fiveCardHand;
	RANK m_rank;
	std::array<Card::SUIT, SEVENCARDHANDSIZE> m_sevenSuits;
	std::array<Card::VALUE, SEVENCARDHANDSIZE> m_sevenValues;
	struct ValueCount
	{
		int count;
		Card::VALUE cardValue;
		int indexInSeven;

		ValueCount(int c, Card::VALUE value, int index) :
			count(c), cardValue(value), indexInSeven(index)
		{
		}
	};
	std::vector<ValueCount> m_valueCount;
	std::array<int, SUITSIZE> m_suitCount;
	std::deque<Card *> m_sequenceTracker;	// keeps track if cards are in sequential order
	std::deque<Card *> m_flushTracker;

public:
	Hand() {};
	~Hand() {};

	void setSevenCardHand(Card* [HOLECARDSSIZE], Card* [COMMUNITYCARDSSIZE]);
	void setSevenSuitsAndValues();
	void setSuitCount();	// counts no of cards with same suits-> flush
	void setValueCountAndSequenceTracker();

	RANK getRankAndSetFiveCardHand();
	void setRank();
	RANK getRank();

	Card* getFiveCardHand(int index);
	
	void displayFiveCardHand();
};