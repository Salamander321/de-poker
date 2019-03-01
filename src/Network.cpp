//
// Created by salamander on 3/1/19.
//

#include "Network.h"
#include "game_defs.h"
#include "player.h"
#include <SFML/Network.hpp>
#include <iostream>

//Member Variable Definitions

bool Network::broadcasting;
sf::IpAddress Network::hostAddress;
sf::TcpListener Network::listener;
std::vector <sf::TcpSocket*> Network::clients;
sf::SocketSelector Network::selector;
bool Network::NetworkEngine = true;
sf::TcpSocket Network::ServerConnect;
std::map <sf::IpAddress,std::string> Network::clientsName;
std::vector <std::string> Network::peers;
bool Network::receivedActionState[] = {false,false,false,false,false,false};
int Network::action;
int Network::value1;
int Network::value2;
int Network::value3;
//MemberFunction Definition

Network::Network(const std::string & name) {
    MyName = name;
}

void Network::MessageBroadcast()
{
    sf::UdpSocket UdpBroadcast;
    UdpBroadcast.bind(sf::Socket::AnyPort);
    broadcasting = true;
    sf::Packet BroadcastPacket;
    BroadcastPacket<<"POKER";
    UdpBroadcast.setBlocking(false);
    while(broadcasting){
        UdpBroadcast.send(BroadcastPacket,sf::IpAddress::Broadcast,BROADCASTLISTEN);
    }
}

void Network::listenBroadcast() {
    sf::UdpSocket UdpListen;
    UdpListen.bind(BROADCASTLISTEN);
    bool listening = true;
    sf::Packet rcvPkg;
    std::string rcvMsg, rcvMSG;
    unsigned short hostPort;
    while (listening) {
        if (UdpListen.receive(rcvPkg, hostAddress, hostPort) == sf::Socket::Done) {
            rcvPkg >> rcvMsg;
            if (rcvMsg == "POKER") {
                if (ServerConnect.connect(hostAddress, HOSTCONNECT) == sf::Socket::Done) {
                    listening = false;
                    std::cout << "Successfully Connected to Server" << std::endl;
                }
            }
        }
    }
    rcvPkg.clear();
    rcvPkg << "Name" << MyName;
    if (ServerConnect.send(rcvPkg) == sf::Socket::Done) {
        std::cout << "Successfully send Name to server" << std::endl;
    }
}

void Network::serverLoop() {
    listener.listen(HOSTCONNECT);
    listener.setBlocking(false);
    sf::Packet serverPacket;
    selector.add(listener);
    std::string type;
    int us_action,us_value1,us_value2,us_value3;
    bool running = true;
    while (running) {

        if (selector.wait())
        {
            if (selector.isReady(listener)) {

                sf::TcpSocket *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    std::cout << client->getRemoteAddress().toString() << "has been successfully connected."
                              << std::endl;
                    client->setBlocking(false);
                    clients.push_back(client);
                    selector.add(*client);
                    sendPeerDetail();
                } else {
                    delete client;
                }
            } else {

                for (int i = 0; i < clients.size(); i++)
                {
                    serverPacket.clear();
                    if (clients[i]->receive(serverPacket) == sf::Socket::Done && selector.isReady(*clients[i])) {
                        std::cout << clients[i]->getRemoteAddress().toString() << " has send Packet" << std::endl;
                        serverPacket>>type>>us_action>>us_value1>>us_value2>>us_value3;
                        serverPacket.clear();
                        if(type == "GAT")
                        {
                            std::cout<<"value1:"<<us_value1<<" value2:"<<us_value2<<" value3:"<<us_value3;
                            Network::action = us_action;
                            Network::value1 = us_value1;
                            receivedActionState[i] = true;

                        }
                       // if()
                        /*
                        for (int j = 0; j < clients.size(); j++) {
                            if (i != j) {
                                std::cout<<"send pack to other"<<std::endl;
                                clients[j]->send(serverPacket);
                            }
                        }
                        */
                    }

                }
            }
        }
    }
}

void Network::receiveData()
{
    std::string rcvMsg, rcvMSG;
    sf::Packet rcvPkg;
    bool listening = true;
    while(listening)
    {
        rcvPkg>>rcvMSG>>rcvMsg;
        if(rcvMSG == "G" || rcvMsg == "G"){
            listening = false;
        }
    }
}


void Network::sendData(std::string sendMsg)
{
    sf::Packet sendPkg;
    sendPkg<<sendMsg;
    if(ServerConnect.send(sendPkg) == sf::Socket::Done)
    {
        std::cout<<"Packet Send Successfully"<<std::endl;
    }
}

bool Network::findPeers(std::string &peerName) {
    auto result = std::find(std::begin(peers),std::end(peers),peerName);
    if (result != std::end(Network::peers)){
        return true;
    } else {
        return false;
    }
}

void Network::peerUpdate() {
    ServerConnect.setBlocking(false);
    bool running = true;
    sf::Packet receivePkg;
    std::string type1,type2;
    while(running){
        ServerConnect.receive(receivePkg);
        receivePkg>>type1>>type2;
        if (type1 == "P" && !findPeers(type2)){
            std::cout<<type2<<std::endl;
            peers.push_back(type2);
        } else if (type2 == "G"){
            running = false;
        }
        receivePkg.clear();
    }
}

void Network::sendPeerDetail() {
    sf::Packet peerPkg;
    for(int j = 0; j< clients.size(); j++) {
        for (int i = 0; i < clients.size(); i++) {
                peerPkg << "P" << clients[i]->getRemoteAddress().toString();
                clients[j]->send(peerPkg);
                peerPkg.clear();
        }
    }
}

void Network::sendGameStart() {
    sf::Packet startPkg;
    startPkg<<"G"<<"G";
    for(int i = 0; i < clients.size(); i++){
        clients[i]->send(startPkg);
    }
}

int Network::getClientNo() {
    std::cout<<clients.size()<<std::endl;
    return clients.size();
}

void Network::sendPot(MONEY potMoney) {
    sf::Packet pkg;
    pkg<<"POT"<<113<<potMoney<<113<<113;
    for (int i = 0; i < getClientNo(); i++){
        clients[i]->send(pkg);
    }
    pkg.clear();
}

void Network::sendHolecards(int clientNo, int suit, int value)
{
    sf::Packet pkg;
    pkg<<"HC"<<113<<suit<<value<<113;
    if(clients[clientNo]->send(pkg) == sf::Socket::Done)
    {
        std::cout<<"Successfully Send HoleCards"<<std::endl;
    }
}

Player::ACTION Network::getAction(int playerIndex)
{
    sf::Packet pkg;
    pkg<<"GAT"<<113<<113<<113<<113;
    if(clients[playerIndex]->send(pkg) == sf::Socket::Done)
    {
        std::cout<<"Action Request Send to "<<clients[playerIndex]->getRemoteAddress().toString()<<" successfully"<<std::endl;
        receivedActionState[playerIndex] = false;
    }
    while(!receivedActionState[playerIndex]);
    receivedActionState[playerIndex] = false;

    return static_cast<Player::ACTION> (Network::action);
}

void Network::sendCommunityCard(int suit, int value)
{
    sf::Packet communityPkg;
    communityPkg<<"CMABC"<<113<<suit<<value<<113;
    for (int i = 0; i < clients.size(); i++)
    {
        if(clients[i]->send(communityPkg) == sf::Socket::Done)
        {
         std::cout<<"CMSEND to "<<clients[i]->getRemoteAddress().toString()<<std::endl;
        }
    }
}

void Network::sendRank(int playerIndex, int Rank)
{
    sf::Packet pkg;
    pkg<<"RANK"<<113<<Rank<<113<<113;
    if(clients[playerIndex]->send(pkg) == sf::Socket::Done)
    {
        std::cout<<"Successfully Sent Rank to "<<clients[playerIndex]->getRemoteAddress().toString()<<std::endl;
    }
}

void Network::receiveGamedata()
{
    sf::Packet pkg;
    std::string type;
    bool running = true;
    while(running && Network::NetworkEngine)
    {   pkg.clear();
        if(ServerConnect.receive(pkg) == sf::Socket::Done)
        {
            pkg>>type>>action>>value1>>value2>>value3;
            if(type == "POT")
            {
                std::cout<<"POT Money is"<<value1<<std::endl;
            }
            else if(type == "HC")
            {
                std::cout<<"Hole Card is "<<value1<<" : "<<value2<<std::endl;
            } else if(type =="GAT")
            {
                int Raction, Rvalue;
                std::cout<<"Action Request has been Sent by server"<<std::endl;
                std::cout<<"0.CAll 1.RAISE 2.FOLD 3.CHECK 4.BET"<<std::endl;
                std::cin>>Raction;
                std::cout<<"Enter a value"<<std::endl;
                std::cin>>Rvalue;
                pkg.clear();
                pkg<<"GAT"<<Raction<<Rvalue;
                if(ServerConnect.send(pkg) == sf::Socket::Done){
                    std::cout<<"Action send successfully"<<std::endl;
                }
            } else if(type == "CMABC")
            {
                std::cout<<"Community Card got"<<std::endl;
                std::cout<<value1<<" : "<<value2<<std::endl;
            }
        }
    }
}
