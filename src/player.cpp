#include "player.h"
#include "Network.h"
Player::Player()
{
	m_hasFolded = false;
	m_isAllIn = false;
	static int p_index = 0;
	m_name = "player" + std::to_string(p_index);
	m_cash = GAME_START_CASH;
	m_avatar = static_cast<Player::AVATAR>(p_index);
	p_index++;
}

Player::Player(std::string name)
{
	m_hasFolded = false;
	m_name = name;
	m_cash = GAME_START_CASH;
	m_avatar = PLAYER0;
}

Player::Player(std::string name, Player::AVATAR avatar)
{
	m_hasFolded = false;
	m_name = name;
	m_cash = GAME_START_CASH;
	m_avatar = avatar;
}

void Player::setHoleCards(Card* holeCard)
{
	if (m_holeCards[0] == nullptr)
	{
		m_holeCards[0] = holeCard;
		return;
	}
	if (m_holeCards[1] == nullptr)
	{
		m_holeCards[1] = holeCard;
		return;
	}
	return;
}

// TODO remove
void Player::getHoleCards(Card*& card1, Card*& card2)
{
	card1 = m_holeCards[0];
	card2 = m_holeCards[1];
}

void Player::getHoleCards(int suit[], int value[])
{
	for (int i = 0; i <HOLECARDSSIZE; i++){
		suit[i] = m_holeCards[i]->getSuit();
		value[i] = m_holeCards[i]->getValue();
	}
}

MONEY Player::getCash()
{
	return m_cash;
}

void Player::addCash(MONEY inCash)
{
	m_cash += inCash;
}

MONEY Player::bet(MONEY betAmount)
{
	/*if (m_cash <= betAmount)
	{
		return allIn(m_cash);
	}*/
	if (m_cash <= betAmount)
	{

	}
	m_cash -= betAmount;
	m_totalRoundBet = betAmount;
	return betAmount;
}

void Player::check()
{
	return;
}

MONEY Player::call(MONEY callMoney)
{
	callMoney -= m_totalRoundBet;
	m_cash -= callMoney;
	m_totalRoundBet += callMoney;
	return callMoney;
}

MONEY Player::raise(MONEY raiseAmount)// = currentbet)
{
	m_cash -= raiseAmount;
	m_totalRoundBet += raiseAmount;
	return raiseAmount;
}

/*void Player::allIn()
{

}*/

MONEY Player::getSmallBlind(MONEY gameSmallBlind)
{
	bet(gameSmallBlind);
	return gameSmallBlind;
}

MONEY Player::getBigBlind(MONEY gameBigBlind)
{
	bet(gameBigBlind);
	return gameBigBlind;
}

void Player::fold()
{	
	m_hasFolded = true;
}

Player::ACTION Player::getAction()
{
	int action;
	std::cout << "--------" << m_name <<"---------" << std::endl;
	std::cout << "0.Call 1.Raise 2.Fold 3.Check 4.Bet :";
	std::cin >> action;
	return static_cast<ACTION>(action);
}

MONEY Player::getRaise(MONEY current_bet)
{
	MONEY raiseAmount;
	do
	{
		std::cout << "Raise Amount: $"<<Network::value1;
		//std::cin >> raiseAmount;
		raiseAmount=(MONEY) Network::value1;
	} while (raiseAmount < 2 * current_bet);

	return raiseAmount;
}

MONEY Player::getBet(MONEY current_bet = 0)
{
	MONEY betAmount;
	do
	{
		std::cout << "Bet Amount: $"<<Network::value1;
		//std::cin >> betAmount;
		betAmount = (MONEY) Network::value1;
	} while (betAmount < GAME_START_BBLIND);

	return betAmount;
}

bool Player::hasFolded()
{
	return m_hasFolded;
}

void Player::setRank(Card* community_cards[])
{
	m_hand.setSevenCardHand(m_holeCards, community_cards);
	m_hand.setSevenSuitsAndValues();
	m_hand.setSuitCount();
	m_hand.setValueCountAndSequenceTracker();
	m_hand.setRank();
}

Hand::RANK Player::getRank()
{
	return m_hand.getRank();
}

Card * Player::getFiveCardHand(int index)
{
	return m_hand.getFiveCardHand(index);
}

void Player::resetTotalRoundBet()
{
	m_totalRoundBet = 0;
}


std::string Player::getName()
{
	return m_name;
}

void Player::display()
{
	for (int i = 0; i < 2; i++)
	{
		std::cout << m_holeCards[i]->getValue() << ", " << m_holeCards[i]->getSuit() << std::endl;
	}
}

void Player::displayFiveCardHand()
{
	m_hand.displayFiveCardHand();
}
