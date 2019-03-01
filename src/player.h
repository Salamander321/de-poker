#pragma once

#include "game_defs.h"
#include "card.h"
#include "hand.h"

#include <string>

class Player
{
public:
	// AVATAR for display pic
	enum AVATAR {PLAYER0, PLAYER1, PLAYER2, PLAYER3, PLAYER4, PLAYER5};
	// enum for actions
	enum ACTION {CALL, RAISE, FOLD, CHECK, BET};

private:
	std::string m_name;
	MONEY m_cash;
	AVATAR m_avatar;

	Card *m_holeCards[HOLECARDSSIZE] = { nullptr };
	Hand m_hand;
	bool m_hasFolded;
	bool m_isAllIn;

public:
	Player();
	Player(std::string);
	Player(std::string, AVATAR);

	MONEY m_totalRoundBet;

	void setHoleCards(Card *);
	void getHoleCards(Card* &, Card* &);
	void getHoleCards(int[], int[]);

	MONEY getCash();
	void addCash(MONEY);

	MONEY call(MONEY);
	MONEY raise(MONEY);
	void fold();
	MONEY bet(MONEY);
	void check();
	//void allIn();

	MONEY getSmallBlind(MONEY);
	MONEY getBigBlind(MONEY);

	// Player Actions
	ACTION getAction();
	MONEY getRaise(MONEY);
	MONEY getBet(MONEY);

	bool hasFolded();

	void setRank(Card* []);
	Hand::RANK getRank();
	Card* getFiveCardHand(int index);
	void resetTotalRoundBet();

	std::string getName();
	void display();
	void displayFiveCardHand();
};
