#include "hand.h"
#include "Network.h"

void Hand::setSevenCardHand(Card *hCard[HOLECARDSSIZE], Card *comCard[COMMUNITYCARDSSIZE])
{
	for (int i = 0; i < HOLECARDSSIZE; i++)
		m_sevenCardHand[i] = hCard[i];
	for (int i = 0; i < COMMUNITYCARDSSIZE; i++)
		m_sevenCardHand[HOLECARDSSIZE + i] = comCard[i];

	// sort cards in ascending order of value
	struct 
	{
		bool operator() (Card *c1, Card *c2)
		{
			return static_cast<int>(c1->getValue()) < static_cast<int>(c2->getValue());
		}
	}customLess;
	std::sort(m_sevenCardHand.begin(), m_sevenCardHand.end(), customLess);
}

void Hand::setSevenSuitsAndValues()
{
	for (int i = 0; i < SEVENCARDHANDSIZE; i++)
	{
		m_sevenSuits[i] = m_sevenCardHand[i]->getSuit();
		m_sevenValues[i] = m_sevenCardHand[i]->getValue();
	}
}

void Hand::setSuitCount()
{
	for (int i = 0; i < SUITSIZE; i++)
	{
		m_suitCount[i] = std::count(m_sevenSuits.begin(), m_sevenSuits.end(), static_cast<Card::SUIT>(i));
	}

	// Set Flush Tracker
	for (int i = 0; i < SUITSIZE; i++)
	{
		if (m_suitCount[i] >= 5)
		{
			for (int j = 0; j < SEVENCARDHANDSIZE; j++)
			{
				if (m_sevenCardHand[j]->getSuit() == static_cast<Card::SUIT>(i))
				{
					if (m_flushTracker.size() == 5)
					{
						m_flushTracker.pop_front();
					}
					m_flushTracker.push_back(m_sevenCardHand[j]);
				}
			}
		}
	}
}

void Hand::setValueCountAndSequenceTracker()
{
	for (int i = 0; i < SEVENCARDHANDSIZE; i++)
	{
		// for sequence tracking
		int sequenceTrackerSize = m_sequenceTracker.size();
		if (sequenceTrackerSize == 0)
		{
			m_sequenceTracker.push_back(m_sevenCardHand[i]);
		}
		else
		{
			if (m_sevenValues[i] == m_sequenceTracker[sequenceTrackerSize - 1]->getValue() + 1)
			{
				if (sequenceTrackerSize == 5)
				{
					m_sequenceTracker.pop_front();
				}	
				m_sequenceTracker.push_back(m_sevenCardHand[i]);
			}
			else if (sequenceTrackerSize == 1 && m_sequenceTracker[0]->getValue() == Card::ACE 
				&& m_sevenValues[i] == Card::TWO)
			{
				if (sequenceTrackerSize == 5)
				{
					m_sequenceTracker.pop_front();
				}
				m_sequenceTracker.push_back(m_sevenCardHand[i]);
			}
			else
			{
				if (sequenceTrackerSize != 5)
				{
					m_sequenceTracker.clear();
					m_sequenceTracker.push_back(m_sevenCardHand[i]);
				}
			}
		}
		// for checking card repetition
		int count = std::count(m_sevenValues.begin() + i, m_sevenValues.end(), m_sevenValues[i]);
		if (count > 1)
		{
			m_valueCount.push_back(ValueCount(count, m_sevenValues[i], i));
			i += count - 1;
		}
	}

	// sort m_valueCount[] descending order of count
	struct
	{
		bool operator() (ValueCount struct1, ValueCount struct2)
		{
			// if count equal, descending order of Card Value
			if (struct1.count == struct2.count)
			{
				return struct1.cardValue > struct2.cardValue;
			}
			return struct1.count > struct2.count;
		}
	}customMore;
	std::sort(m_valueCount.begin(), m_valueCount.end(), customMore);

	// if deque.size() != 5, ie sequence < 5, clear
	if (m_sequenceTracker.size() != 5)
	{
		m_sequenceTracker.clear();
	}
}

// rank determination + best five cards
Hand::RANK Hand::getRankAndSetFiveCardHand()
{
	// For Straight
	if (m_sequenceTracker.size() == 5)
	{
		// For Straight Flush
		for (int i = 1; i < 5; i++)
		{
			if (m_sequenceTracker[0]->getSuit() != m_sequenceTracker[i]->getSuit())
				break;
			// Assigning Five Card Hand
			if (i == 4)
			{
				for (int j = 0; j < FIVECARDHANDSIZE; j++)
				{
					m_fiveCardHand[j] = m_sequenceTracker[j];
				}
				// For Royal Flush
				if (m_fiveCardHand[FIVECARDHANDSIZE - 1]->getValue() == Card::ACE)
					return ROYALFLUSH;
				return STRAIGHTFLUSH;
			}
		}
		return STRAIGHT;
	}

	// For Flush
	if(m_flushTracker.size() == 5)
	{
		// Assigning Five Card Hand
		for (int i = 0; i < FIVECARDHANDSIZE; i++)
		{
			m_fiveCardHand[i] = m_flushTracker[FIVECARDHANDSIZE - 1 - i];
		}
		return FLUSH;
	}

	if (m_valueCount.size() > 0)
	{
		// For Four of a Kind
		if (m_valueCount[0].count == 4)
		{
			for (int i = 0; i < 4; i++)
			{
				m_fiveCardHand[i] = m_sevenCardHand[m_valueCount[0].indexInSeven + i];
			}
			// fill remaining one card slot
			if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 4)
			{
				m_fiveCardHand[4] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
			}
			else
			{
				m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[0].indexInSeven - 1];
			}
				return FOUROFAKIND;
		}
		// 3 cards repeated
		if (m_valueCount[0].count == 3)
		{
			// For Full House
			if (m_valueCount.size() > 1 && m_valueCount[1].count == 2)
			{
				for (int i = 0; i < 3; i++)
				{
					m_fiveCardHand[i] = m_sevenCardHand[m_valueCount[0].indexInSeven + i];
				}
				for (int i = 0; i < 2; i++)
				{
					m_fiveCardHand[3 + i] = m_sevenCardHand[m_valueCount[1].indexInSeven + i];
				}
				return FULLHOUSE;
			}
			// For Three of a Kind
			for (int i = 0; i < 3; i++)
			{
				m_fiveCardHand[i] = m_sevenCardHand[m_valueCount[0].indexInSeven + i];
			}
			// fill remaining two card slot
			if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 4)
			{
				m_fiveCardHand[3] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
				m_fiveCardHand[4] = m_sevenCardHand[SEVENCARDHANDSIZE - 2];
			}
			else if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 3)
			{
				m_fiveCardHand[3] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
				m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[0].indexInSeven - 1];
			}
			else
			{
				m_fiveCardHand[3] = m_sevenCardHand[m_valueCount[0].indexInSeven - 1];
				m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[0].indexInSeven - 2];
			}
			return THREEOFAKIND;
		}
		// 2 cards repeated
		if (m_valueCount[0].count == 2)
		{
			// For Two Pairs
			if (m_valueCount.size() > 1 && m_valueCount[1].count == 2)
			{
				// Assiging to Five Card Hand
				for (int i = 0; i < 2; i++)
				{
					m_fiveCardHand[i] = m_sevenCardHand[m_valueCount[0].indexInSeven + i];
				}
				for (int i = 2; i < 4; i++)
				{
					m_fiveCardHand[i] = m_sevenCardHand[m_valueCount[1].indexInSeven + i];
				}
				// fill remaining one card slot
				if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 2)
				{
					m_fiveCardHand[4] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
				}
				else
				{
					if (m_valueCount[1].indexInSeven < SEVENCARDHANDSIZE - 4)
					{
						m_fiveCardHand[4] = m_sevenCardHand[SEVENCARDHANDSIZE - 3];
					}
					else
					{
						m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[1].indexInSeven - 1];
					}
				}
				return TWOPAIRS;
			}
			// Assiging to Five Card Hand
			for (int i = 0; i < 2; i++)
			{
				m_fiveCardHand[i] = m_sevenCardHand[m_valueCount[0].indexInSeven + i];
			}
			// fill remaining three card slot
			if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 4)
			{
				m_fiveCardHand[2] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
				m_fiveCardHand[3] = m_sevenCardHand[SEVENCARDHANDSIZE - 2];
				m_fiveCardHand[4] = m_sevenCardHand[SEVENCARDHANDSIZE - 3];
			}
			else if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 3)
			{
				m_fiveCardHand[2] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
				m_fiveCardHand[3] = m_sevenCardHand[SEVENCARDHANDSIZE - 2];
				m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[0].indexInSeven - 1];
			}
			else if (m_valueCount[0].indexInSeven < SEVENCARDHANDSIZE - 2)
			{
				m_fiveCardHand[2] = m_sevenCardHand[SEVENCARDHANDSIZE - 1];
				m_fiveCardHand[3] = m_sevenCardHand[m_valueCount[0].indexInSeven - 1];
				m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[0].indexInSeven - 2];
			}
			else
			{
				m_fiveCardHand[2] = m_sevenCardHand[m_valueCount[0].indexInSeven - 1];
				m_fiveCardHand[3] = m_sevenCardHand[m_valueCount[0].indexInSeven - 2];
				m_fiveCardHand[4] = m_sevenCardHand[m_valueCount[0].indexInSeven - 3];
			}
			return PAIR;
		}
	}
	// Assigning Five Card Hand (ascending order)
	for (int i = 0; i < FIVECARDHANDSIZE; i++)
	{
		m_fiveCardHand[i] = m_sevenCardHand[SEVENCARDHANDSIZE - 1 - i];
	}

	return HIGHCARD;
}

void Hand::setRank()
{
	m_rank = getRankAndSetFiveCardHand();
}

Hand::RANK Hand::getRank()
{
	return m_rank;
}

Card* Hand::getFiveCardHand(int index)
{
	return m_fiveCardHand[index];
}

void Hand::displayFiveCardHand()
{
	int suit, value;
	std::cout << "----------------" << std::endl;
	for (int i = 0; i < FIVECARDHANDSIZE; i++)
	{
		m_fiveCardHand[i]->display();
		suit = m_fiveCardHand[i]->getSuit();
		value = m_fiveCardHand[i]->getSuit();
	}
	std::cout << std::endl;
}
