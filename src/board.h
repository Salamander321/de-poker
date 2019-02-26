#pragma once

#include "game_defs.h"

#include "player.h"
#include "deck.h"

class Board
{
private:
	std::vector <Player *> m_players;
	Deck m_deck;
	MONEY m_pot;	// pot: for money collection

	bool m_initialized;
	std::vector<Player *> m_winners;

	int m_dealerIndex;
	int m_smallBlindIndex;
	int m_bigBlindIndex;

	int m_playerCount;
	bool m_allFolded;
	int m_foldedCount;
	MONEY m_currentBet;
	Card* m_communityCards[COMMUNITYCARDSSIZE];

public:
	Board();
	~Board();

	// players 
	void initPlayers(int count);
	void initPlayers(int, std::vector<std::pair<std::string, Player::AVATAR>>);
	void setDealer(int index);	// also sets small blind and BB
	int getDealer();
	int getSmallBlind();
	int getBigBlind();
	void nextGameRound();

	// gameplay
	void initGame();
	void shuffleDeck();
	void dealCards();
	void preFlopRound();
	void flopRound();
	void turnRound();
	void riverRound();
	void setPlayersCardRank();
	void bettingRound(int, MONEY);
	void resetPlayerRoundBet();

	// Board Pot manipulation
	//void setPot(MONEY);
	void initPot();
	MONEY getPot();
	MONEY add_to_pot(MONEY);
	MONEY deduct_from_pot(MONEY);
	MONEY winPot();
	void displayPot();

	// Declare Winner
	void setWinner();
	std::vector<Player *> getWinner();
	void clearWinner();
	int getFoldedCount();
	void setAllFolded();
	bool haveAllFolded();
	Player* unfoldedPlayer();

	// Display in CLI
	void displayFlop();
	void displayTurn();
	void displayRiver();
	void display();
};