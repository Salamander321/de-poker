//
// Created by salamander on 3/1/19.
//
#include "Network.h"
#include "poker.h"
#include "game.h"
#include "gui_engine_defs.h"
#include "asset_manager.h"
#include "PlayerContainer.h"
#include <iostream>
#include <thread>

void NetWorkThread()
{
    Network Node("Client");
    std::cout<<"Waiting For Server To connect"<<std::endl;
    Node.listenBroadcast();
    Node.peerUpdate();
    PlayerContainer myContainer;

    std::string msg;
    std::thread MainGame(Network::receiveGamedata);
    MainGame.join();
}

void GameLoop(){
    bool gameRunning = true;
    std::cout<<"Game State Entered"<<std::endl;
    // if conditional should be run continuously so it is put under while loop//
    while(gameRunning && Network::NetworkEngine){
        if(Network::receiveData)
        {
            if(Network::rcvType == "POT")
            {
                /*
                 * Network::value1 is value of Pot
                 * Also Don't forgot to Network::receivedData = false once finished instantly
                 */
                std::cout<<"Pot Money is "<<Network::value1<<std::endl;
                Network::receiveData = false;
            } else if (Network::rcvType == "HC")
            {
                /*
                 *This is for Hole card. Two cards will be send on two different packets
                 * Its for suit and value respectively
                 */
                std::cout<<Network::value1<<" : "<<Network::value2<<std::endl;
                Network::receiveData = false;
            } else if( Network::rcvType == "GAT")
            {
                /*
                 * This is for send type action calling user for raise , call bet and fold
                 */
                Network::receiveData = false;
                int Raction,Rvalue;
                std::cout<<"Action Request has been Sent by server"<<std::endl;
                std::cout<<"0.CAll 1.RAISE 2.FOLD 3.CHECK 4.BET"<<std::endl;
                std::cin>>Raction;
                std::cout<<"Enter a value"<<std::endl;
                std::cin>>Rvalue;
                Network::sendActionToServer(Raction, Rvalue);

            } else if(Network::rcvType == "CMABC")
            {
                /*
                 * This is for community card so will be send in continous three way. Packet loss is possible
                 * Suit and Value Format
                 */
                std::cout<<Network::value1<<" : "<<Network::value2<<std::endl;
                Network::receiveData = false;
            } else if(Network::action == 114)
            {
                /*
                 * This is for getting winners Name;
                 */
                std::cout<<"Winner is : "<<Network::rcvType<<std::endl;
                Network::receiveData = false;
            } else if(Network::rcvType == "RANK")
            {
                /*
                 * This is for Player getting their own Rank
                 */
                std::cout<<"Rank is : "<<Network::value1<<std::endl;
                Network::receiveData  = false;
            } else if(Network::rcvType == "BSTPACK")
            {
                std::cout<<"Best Card is : "<<Network::value1<<" : "<<Network::value2<<std::endl;
                Network::receiveData = false;
            } else if(Network::rcvType == "PGAT")
            {
                std::cout<<"Player "<<Network::peersName[Network::peers[Network::value1]]<<" has "<<Network::value2<<std::endl;
                Network::receiveData = false;

            }
        }
    }
}
int main()
{   std::thread Network(NetWorkThread);
    Network.detach();
    GameLoop();

    //Game(SCREEN_WIDTH,SCREEN_HEIGHT,"Lobby");


    return 0;
}
