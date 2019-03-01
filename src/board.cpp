#include "board.h"
#include "Network.h"

Board::Board()
{
	initPot();
}

Board::~Board()
{

}

void Board::initPlayers(int count)
{
	for (int i = 0; i < count; i++)
	{
		m_players.push_back(new Player());
	}
	m_playerCount = m_players.size();
	setDealer(0);
}

void Board::initPlayers(int player_count, std::vector<std::pair<std::string, Player::AVATAR>> player_data)
{
	while (player_data.size() < size_t(player_count))
	{
		int i = 0;
		player_data.push_back(std::pair<std::string, Player::AVATAR>("Player " + std::to_string(i), static_cast<Player::AVATAR>(i)));
		i++;
	}
	for (int i = 0; i < player_count; i++)
	{
		m_players.push_back(new Player(player_data[i].first, player_data[i].second));
	}
	m_playerCount = m_players.size();
	setDealer(0);
}

void Board::setDealer(int index)
{
	if (m_playerCount == 2)
	{
		m_dealerIndex = index % m_playerCount;
		m_smallBlindIndex = index % m_playerCount;
		m_bigBlindIndex = (index + 1) % m_playerCount;
		return;
	}
	m_dealerIndex = index % m_playerCount;
	m_smallBlindIndex = (index + 1) % m_playerCount;
	m_bigBlindIndex = (index + 2) % m_playerCount;
}

int Board::getDealer()
{
	return m_dealerIndex;
}

int Board::getSmallBlind()
{
	return m_smallBlindIndex;
}

int Board::getBigBlind()
{
	return m_bigBlindIndex;
}

void Board::nextGameRound()
{
	setDealer(m_dealerIndex + 1);
}

void Board::initGame()
{
	m_foldedCount = 0;
	m_allFolded = false;
	
	// reset last round bet
	resetPlayerRoundBet();

	m_currentBet = GAME_START_BBLIND;
	m_pot += m_players[m_smallBlindIndex]->getSmallBlind(GAME_START_SBLIND);
	m_pot += m_players[m_bigBlindIndex]->getBigBlind(GAME_START_BBLIND);

}

void Board::shuffleDeck()
{
	m_deck.fisherYatesShuffle();
}

void Board::dealCards()
{
	for (int i = 0; i < HOLECARDSSIZE; i++)
	{
		for (int j = 0; j < m_playerCount; j++)
		{
			m_players[(m_smallBlindIndex + j) % m_playerCount]->setHoleCards(m_deck.popCard());
		}
	}
	int suit[2],value[2];
	for(int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->getHoleCards(suit,value);
		for(int j = 0;j < 2;j++)
		{
			Network::sendHolecards(i,suit[j],value[j]);
		}
	}
}

void Board::preFlopRound()
{
	if (m_playerCount == 2)
		bettingRound(m_smallBlindIndex, GAME_START_BBLIND);
	else
		bettingRound(m_bigBlindIndex + 1, GAME_START_BBLIND);
	resetPlayerRoundBet();
}

void Board::flopRound()
{
	if (m_allFolded)
	{
		return;
	}
	m_deck.getFlop(m_communityCards[0], m_communityCards[1], m_communityCards[2]);
	displayFlop();
	int value,suit;
	for (int i = 0;i < 3; i++)
	{
		Network::sendCommunityCard(m_communityCards[i]->getSuit(),m_communityCards[i]->getValue());
	}
	bettingRound((m_dealerIndex + 1) % m_playerCount, GAME_START_BBLIND);
	resetPlayerRoundBet();
}

void Board::turnRound()
{
	if (m_allFolded)
	{
		return;
	}

	m_communityCards[3] = m_deck.getTurn();
	Network::sendCommunityCard(m_communityCards[3]->getSuit(),m_communityCards[3]->getValue());
	displayTurn();
	bettingRound((m_dealerIndex + 1) % m_playerCount, GAME_START_BBLIND);
	resetPlayerRoundBet();
}

void Board::riverRound()
{
	if (m_allFolded)
	{
		return;
	}

	m_communityCards[4] = m_deck.getRiver();
	Network::sendCommunityCard(m_communityCards[4]->getSuit(),m_communityCards[4]->getValue());
	displayRiver();
	resetPlayerRoundBet();
}

void Board::setPlayersCardRank()
{
	if (m_allFolded)
	{
		return;
	}

	for (int i = 0; i < m_playerCount; i++)
	{
		m_players[i]->setRank(m_communityCards);
	}
}

void Board::bettingRound(int actingPlayerIndex, MONEY currentBet)
{
	
	MONEY initialBet = 0;
	int raiseCount = 0;
	int temp_offset = 0;

	for (int i = 0; i < m_playerCount; i++)
	{
		if (m_foldedCount == m_playerCount - 1)
		{
			return;
		}
		int playerIndex = (actingPlayerIndex + i + temp_offset) % m_playerCount;
		if (!(m_players[playerIndex]->hasFolded()))
		{
			//Player::ACTION action = m_players[playerIndex]->getAction();
			Player::ACTION action = Network::getAction(playerIndex);
            std::cout<<"Action has been selected"<<playerIndex<<":" <<action<<std::endl;
			MONEY raise;
			MONEY bet;
			
			switch (action)
			{
			case Player::RAISE:
				if (raiseCount == 3)
				{
					m_pot += m_players[playerIndex]->call(currentBet);
					break;
				}
				std::cout<<playerIndex<<" : "<<"RAISE"<<std::endl;
				raise = m_players[playerIndex]->raise(m_players[playerIndex]->getRaise(currentBet));
				m_pot += raise;
				currentBet = raise;
				raiseCount++;
				temp_offset = i;
				i = 0;
				break;
			case Player::BET:
                std::cout<<playerIndex<<" : "<<"BET"<<std::endl;
				bet = m_players[playerIndex]->bet(m_players[playerIndex]->getBet(currentBet));
				m_pot += bet;
				currentBet = bet;
				temp_offset = i;
				i = 0;
				break;
			case Player::CALL:
                std::cout<<playerIndex<<" : "<<"CALL"<<std::endl;
				m_pot += m_players[playerIndex]->call(currentBet);
				break;
			case Player::CHECK:
                std::cout<<playerIndex<<" : "<<"CHECK"<<std::endl;
				m_players[playerIndex]->check();
				break;
			case Player::FOLD:
                std::cout<<playerIndex<<" : "<<"FOLD"<<std::endl;
				m_players[playerIndex]->fold();
				m_foldedCount++;
				break;
			}
		}
	}
}

void Board::resetPlayerRoundBet()
{
	for (int i = 0; i < m_playerCount; i++)
	{
		m_players[i]->resetTotalRoundBet();
		//m_players[i]->isAllIn
	}
}

void Board::initPot()
{
	m_pot = 0;
}

MONEY Board::getPot()
{
	return m_pot;
}

MONEY Board::add_to_pot(MONEY inCash)
{
	m_pot += inCash;
	return m_pot;
}

MONEY Board::deduct_from_pot(MONEY outCash)
{
	if (m_pot < outCash) return m_pot;
	m_pot -= outCash;
	return m_pot;
}

MONEY Board::winPot()
{
	m_pot /= m_winners.size();
	for (size_t i = 0; i < m_winners.size(); i++)
	{
		m_winners[i]->addCash(m_pot);
	}

	m_pot = 0;
	return m_pot;
}

void Board::displayPot()
{
	std::cout << "POT: $" << m_pot << std::endl;
	Network::sendPot(m_pot);

}

void Board::setWinner()
{
	if (m_allFolded)
	{
		m_winners.push_back(unfoldedPlayer());
		return;
	}
	int startIndex = 0;
	// see if first player hasFolded
	if (m_players[startIndex]->hasFolded())
	{
		startIndex++;
	}

	std::vector <Player *> ranking;
	ranking.push_back(m_players[startIndex]);

	// find the best rank
	for (int i = startIndex + 1; i < m_playerCount; i++)
	{
		if (!(m_players[i]->hasFolded()))
		{
			if (m_players[i]->getRank() == ranking[0]->getRank())
			{
				ranking.push_back(m_players[i]);
			}
			else if (m_players[i]->getRank() < ranking[0]->getRank())
			{
				ranking.clear();
				ranking.push_back(m_players[i]);
			}
		}
	}

	// decide winner
	if (ranking.size() == 1)
	{
		m_winners.push_back(ranking[0]);
	}
	if (ranking.size() > 1)
	{
		switch (ranking[0]->getRank())
		{
		case Hand::ROYALFLUSH:
			m_winners = ranking;
			break;
		case Hand::STRAIGHTFLUSH:
		case Hand::STRAIGHT:
			m_winners.push_back(ranking[0]);
			for (size_t i = 1; i < ranking.size(); i++)
			{
				if (ranking[i]->getFiveCardHand(FIVECARDHANDSIZE - 1) > 
					m_winners[0]->getFiveCardHand(FIVECARDHANDSIZE - 1))
				{
					m_winners.clear();
					m_winners.push_back(ranking[i]);
				}
			}
			break;
		case Hand::FOUROFAKIND:
		case Hand::FULLHOUSE:
		case Hand::THREEOFAKIND:
			m_winners.push_back(ranking[0]);
			for (size_t i = 1; i < ranking.size(); i++)
			{
				if (ranking[i]->getFiveCardHand(0) > 
					m_winners[0]->getFiveCardHand(0))
				{
					m_winners.clear();
					m_winners.push_back(ranking[i]);
				}
			}
			break;
		case Hand::FLUSH:
		case Hand::TWOPAIRS:
		case Hand::PAIR:
		case Hand::HIGHCARD:
			m_winners.push_back(ranking[0]);
			for (size_t i = 1; i < ranking.size(); i++)
			{
				for (int j = 0; j < FIVECARDHANDSIZE; j++)
				{
					if (ranking[i]->getFiveCardHand(j) >
						m_winners[0]->getFiveCardHand(j))
					{
						m_winners.clear();
						m_winners.push_back(ranking[i]);
						break;
					}
					else if (ranking[i]->getFiveCardHand(j) <
						m_winners[0]->getFiveCardHand(j))
					{
						break;
					}
					else if (j == FIVECARDHANDSIZE - 1)
					{
						if (ranking[i]->getFiveCardHand(j) ==
							m_winners[0]->getFiveCardHand(j))
						{
							m_winners.push_back(ranking[i]);
						}
					}
				}
			}
			break;
		}
	}

	// distribute money to winners
	winPot();
}

std::vector<Player *> Board::getWinner()
{
	return m_winners;
}

void Board::clearWinner()
{
	m_winners.clear();
}

int Board::getFoldedCount()
{
	return m_foldedCount;
}

void Board::setAllFolded()
{
	if (m_foldedCount == m_playerCount - 1)
	{
		m_allFolded = true;
	}
}

bool Board::haveAllFolded()
{
	return m_allFolded;
}

Player * Board::unfoldedPlayer()
{
	for (int i = 0; i < m_playerCount; i++)
	{
		if (!(m_players[i]->hasFolded()))
		{
			return m_players[i];
		}
	}
	return nullptr;
}

void Board::displayFlop()
{
	for (int i = 0; i < 3; i++)
	{
		m_communityCards[i]->display();
	}
}

void Board::displayTurn()
{
	m_communityCards[3]->display();
}

void Board::displayRiver()
{
	m_communityCards[4]->display();
}

void Board::display()
{
	for (int i = 0; i < m_playerCount; i++)
	{
		std::cout << m_players[i]->getName() << std::endl;
		m_players[i]->display();
	}
	std::cout << "--------------------------" << std::endl;
	for (int i = 0; i < COMMUNITYCARDSSIZE; i++)
		std::cout << m_communityCards[i]->getValue() << ", " << m_communityCards[i]->getSuit() << std::endl;
	std::cout << "Rankings" << std::endl;
	for (int i = 0; i < m_playerCount; i++)
	{
		if(!(m_players[i]->hasFolded()))
			std::cout << m_players[i]->getRank() << std::endl;

	}
	for (int i = 0; i < m_playerCount; i++)
	{	

		if(!(m_players[i]->hasFolded()))
			m_players[i]->displayFiveCardHand();
	}
}
