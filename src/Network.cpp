//
// Created by salamander on 3/1/19.
//

#include "Network.h"
#include "game_defs.h"
#include "player.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
using namespace std::literals::chrono_literals;
//Member Variable Definitions

bool Network::broadcasting;
sf::IpAddress Network::hostAddress;
sf::TcpListener Network::listener;
std::vector <sf::TcpSocket*> Network::clients;
sf::SocketSelector Network::selector;
bool Network::NetworkEngine = true;
sf::TcpSocket Network::ServerConnect;
std::map <sf::IpAddress,std::string> Network::clientsName;
std::map <std::string,std::string> Network::peersName;
std::vector <std::string> Network::peers;
bool Network::receivedActionState[] = {false,false,false,false,false,false};
int Network::action;
int Network::value1;
int Network::value2;
int Network::value3;
std::string Network::rcvType;
bool Network::receiveData;
bool Network::GameStart = false;

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
        std::this_thread::sleep_for(1s);
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
                    rcvPkg.clear();
                    rcvPkg << "Name" << MyName;
                    if (ServerConnect.send(rcvPkg) == sf::Socket::Done) {
                        std::cout << "Successfully send Name to server" << std::endl;
                    }
                    std::cout << "Successfully Connected to Server" << std::endl;
                }
            }
        }
    }

}

void Network::serverLoop() {
    listener.listen(HOSTCONNECT);
    listener.setBlocking(false);
    sf::Packet serverPacket;
    selector.add(listener);
    std::string type;
    std::string Name;
    int us_action,us_value1,us_value2,us_value3;
    bool running = true;
    while (running) {
        std::this_thread::sleep_for(100ms);

        if (selector.wait())
        {
            if (selector.isReady(listener)) {

                sf::TcpSocket *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    std::cout << client->getRemoteAddress().toString() << "has been successfully connected."
                              << std::endl;
                    if(client->receive(serverPacket) == sf::Socket::Done)
                    {
                        std::cout<<"successfully received Name"<<std::endl;
                    }
                    serverPacket>>type>>Name;
                    if(type == "Name")
                    {
                        clientsName.insert(std::pair<sf::IpAddress,std::string>(client->getRemoteAddress(),Name));
                    }
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

/*
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
*/

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
    std::string type1,type2,type3;
    while(running){
        std::this_thread::sleep_for(100ms);
        ServerConnect.receive(receivePkg);
        receivePkg>>type1>>type2>>type3;
        if (type1 == "P" && !findPeers(type2)){
            std::cout<<type2<<type3<<std::endl;
            peers.push_back(type2);
            peersName.insert(std::pair<std::string,std::string>(type2,type3));
        } else if (type2 == "G"){
            running = false;
            GameStart = true;
        }
        receivePkg.clear();
    }
}

void Network::sendPeerDetail() {
    sf::Packet peerPkg;
    for(int j = 0; j< clients.size(); j++) {
        for (int i = 0; i < clients.size(); i++) {
                peerPkg << "P" << clients[i]->getRemoteAddress().toString()<<clientsName[clients[i]->getRemoteAddress()];
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
void Network::sendBestCards(int suit, int value)
{
    sf::Packet bestPkg;
    bestPkg<<"BSTPACK"<<113<<suit<<value<<113;
    for (int i = 0; i < clients.size(); i++)
    {
        if(clients[i]->send(bestPkg) == sf::Socket::Done)
        {
            std::cout<<"BSTCARD to "<<clients[i]->getRemoteAddress().toString()<<std::endl;
        }
    }
}

void Network::sendWinners(std::string winnerName)
{
    sf::Packet winName;
    winName<<winnerName<<114<<113<<113<<113;
    for (int i = 0; i < clients.size(); i++)
    {
        if(clients[i]->send(winName) == sf::Socket::Done)
        {
            std::cout<<"Winners are send to "<<clients[i]->getRemoteAddress().toString()<<std::endl;
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
            {   rcvType = type;
                Network::receiveData = true;
                std::cout<<"POT Money is"<<value1<<std::endl;
                while (receiveData);
            }
            else if(type == "HC")
            {   rcvType = type;
                Network::receiveData = true;
                std::cout<<"Hole Card is "<<value1<<" : "<<value2<<std::endl;
                while (receiveData);
            } else if(type =="GAT")
            {
                //int Raction, Rvalue;
                rcvType = type;
                Network::receiveData = true;
                while (receiveData);
                /*
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
                 */
            } else if(type == "CMABC")
            {   rcvType = type;
                Network::receiveData = true;
                while (receiveData);//value1 and value2
                std::cout<<"Community Card got"<<std::endl;
                std::cout<<value1<<" : "<<value2<<std::endl;
            }
            else if(action == 114)
            {
                rcvType = type; //WInner name is received in this
                Network::receiveData = true;
                while(receiveData);
            }
            else if(type == "RANK")
            {   rcvType = type;
                //Rank can be got by value1
                Network::receiveData = true;
                while (receiveData);
            } else if(type == "BSTPACK")
            {   rcvType = type;
                Network::receiveData = true;
                while (receiveData);
                //Suit = Value1 and Value = Value2
            } else if (type == "PGAT")
            {   rcvType = type;
                Network::receiveData = true;
                while (receiveData);
            }
        }
    }
}

void Network::sendActionToServer(int Action, int Value)
{
    sf::Packet pkg;
    pkg<<"GAT"<<Action<<Value;
    ServerConnect.send(pkg);
}
void Network::sendPlayerAction(int PlayerIndex, int Action)
{
    sf::Packet actionPkg;
    actionPkg<<"PGAT"<<113<<PlayerIndex<<Action;
    for (int i = 0; i < clients.size(); i++)
    {
        if (i != PlayerIndex)
        {
            clients[i]->send(actionPkg);
        }
    }
}

