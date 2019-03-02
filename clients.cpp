//
// Created by salamander on 3/1/19.
//
#include "Network.h"
#include "poker.h"
#include "game.h"
#include "gui_engine_defs.h"
#include "asset_manager.h"
#include <iostream>
#include <thread>

void NetWorkThread()
{
    Network Node("Client");
    std::cout<<"Waiting For Server To connect"<<std::endl;
    Node.listenBroadcast();
    Node.peerUpdate();
    std::string msg;
    std::thread MainGame(Network::receiveGamedata);
    MainGame.join();
}

int main()
{   std::thread Network(NetWorkThread);
    Network.detach();
    Game(SCREEN_WIDTH,SCREEN_HEIGHT,"Lobby");


    return 0;
}
