#include "poker.h"

int main()
{
	Board board;
	board.initPlayers(PLAYERSIZE);
	board.initGame();
	board.displayPot();

	board.shuffleDeck();

	board.dealCards();
	board.preFlopRound();
	board.displayPot();
	
	board.flopRound();
	board.displayPot();

	board.turnRound();
	board.displayPot();
	board.riverRound();
	board.displayPot();

	board.setPlayersCardRank();
	board.display();

	board.setWinner();
	std::vector <Player *> winners = board.getWinner();
	Network::sendWinners(winners[0]->getName());
	for (size_t i = 0; i < winners.size(); i++)
	{
		std::cout << winners[i]->getName() << "   CASH: $" << winners[i]->getCash() << std::endl;
		winners[i]->displayFiveCardHand();
	}
	board.displayPot();
	std::cin.get();
	std::cin.get();
    return 0;
}
	/*Deck deck;
	Player player[PLAYER_COUNT];
	Card * community_cards[COMMUNITYCARDSSIZE] = { nullptr };*/
	

	//deck.fisherYatesShuffle();
	//deck.display();

	/*for (int i = 0; i < 2; i++)
	{
		player[0].setHoleCards(deck.popCard());
		player[1].setHoleCards(deck.popCard());
	}*/
	/*deck.getFlop(community_cards[0], community_cards[1], community_cards[2]);*/
	/*community_cards[3] = deck.getTurn();
	community_cards[4] = deck.getRiver();*/
	//player[0].setRank(community_cards);

	//player[0].getHoleCards(hole_cards[0], hole_cards[1]);
	//
	//Hand hand;
	//hand.setSevenCardHand(hole_cards, community_cards);
	//hand.setSevenSuitsAndValues();
	//hand.setSuitCount();
	//hand.setValueCountAndSequenceTracker();
	//Hand::RANK rank = hand.getRankAndSetFiveCardHand();
	//hand.setRank();

	/*std::cout << std::endl;
	player[0].display();
	std::cout << community_cards[0]->getValue() << ", " << community_cards[0]->getSuit() <<std::endl;
	std::cout << community_cards[1]->getValue() << ", " << community_cards[1]->getSuit() <<std::endl;
	std::cout << community_cards[2]->getValue() << ", " << community_cards[2]->getSuit() <<std::endl;
	std::cout << community_cards[3]->getValue() << ", " << community_cards[3]->getSuit() <<std::endl;
	std::cout << community_cards[4]->getValue() << ", " << community_cards[4]->getSuit() <<std::endl;
	*/			  

	//std::cout << rank << std::endl;

	//std::cout << player[0].getRank() << std::endl;