//
// Created by salamander on 3/1/19.
//
#include "Network.h"
#include "poker.h"
#include <iostream>
#include <thread>

int main()
{
    Network Node("Client");
    std::cout<<"Waiting For Server To connect"<<std::endl;
    Node.listenBroadcast();
    Node.peerUpdate();
    std::string msg;
    std::thread MainGame(Network::receiveGamedata);
    MainGame.join();


    return 0;

}
