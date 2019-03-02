//
// Created by salamander on 3/1/19.
//

#include "Network.h"
#include "poker.h"
#include <thread>
#include <iostream>
using namespace std::literals::chrono_literals;
int main()
{
    Network Server("Host");
    std::thread broadcastMessage(Network::MessageBroadcast);

    std::thread serverloop(Network::serverLoop);
    serverloop.detach();
    while(true){
        std::cout<<"Enter any Key to quit"<<std::endl;
        std::cin.get();
        Network::broadcasting = false;
        break;
    }
    broadcastMessage.join();
    Server.sendGameStart();
    std::this_thread::sleep_for(1000ms);
    std::cout<<"Game Started"<<std::endl;
    Board board;
    board.initPlayers(Network::getClientNo());
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
    }
    board.displayPot();
    std::cout<<"Game Ended"<<std::endl;
    return 0;

}