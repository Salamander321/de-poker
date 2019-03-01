//
// Created by salamander on 3/1/19.
//

#ifndef DEPOKER_NETWORK_H
#define DEPOKER_NETWORK_H

#include <string>
#include <SFML/Network.hpp>
#include "game_defs.h"
#include "player.h"


class Network {
private:
    std::string MyName;
    static sf::IpAddress hostAddress;
    static sf::TcpSocket ServerConnect;
    static sf::TcpListener listener;
    static std::vector <sf::TcpSocket*> clients;
    static sf::SocketSelector selector;
    static std::map <sf::IpAddress,std::string> clientsName;
public:
    Network(const std::string&);
    static bool receivedActionState[];
    static std::vector <std::string> peers;
    static int action, value1, value2, value3;
    static bool NetworkEngine;
    static bool broadcasting;
    static void MessageBroadcast();
    void listenBroadcast();
    static void serverLoop();
    void peerUpdate();
    static void sendPeerDetail();
    static bool findPeers(std::string &);
    void sendGameStart();
    static void sendHolecards(int,int,int);
    static int getClientNo();
    static Player::ACTION getAction(int);
    static void sendCommunityCard(int,int);
    static void sendRank(int,int);
    static void receiveGamedata();

    //May or May not Required
    static void receiveData();
    void sendData(std::string);
    static void sendPot(MONEY );



};


#endif //DEPOKER_NETWORK_H
