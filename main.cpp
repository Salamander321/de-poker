//
// Created by salamander on 2/21/19.
//

#include "game.h"
#include "gui_engine_defs.h"
#include "Network.h"
#include <thread>
#include "PlayerContainer.h"

using namespace std::literals::chrono_literals;

void NetWorkThread()
{

    Network Node("Client");
    std::cout<<"Waiting For Server To connect"<<std::endl;
    while(!Game::listenStart){
        std::this_thread::sleep_for(400ms);
    }
    Node.listenBroadcast();
    Node.peerUpdate();
    PlayerContainer myContainer;

    std::string msg;
    std::thread MainGame(Network::receiveGamedata);
    MainGame.join();
}

int main()
{
    std::thread net_thread(NetWorkThread);
    net_thread.detach();
    Game(SCREEN_WIDTH, SCREEN_HEIGHT, "Poker Texas Hold'em");
}


