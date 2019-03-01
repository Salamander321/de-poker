//
// Created by salamander on 2/25/19.
//

#ifndef DEPOKER_NETWORK_H
#define DEPOKER_NETWORK_H

#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <vector>
#include <iterator>
#include <thread>
#include <chrono>
using namespace std::literals::chrono_literals;

sf::UdpSocket UdpNode, UdpHost;
sf::TcpSocket Tcp+;
sf::TcpListener listener;
std::string networkName;
sf::Packet BroadCastMessgae;
std::vector <sf::IpAddress> peer;
std::map <std::string,std::string> peersName;
static bool broadcastingMessage = true;
static bool listenbroadcasting = true;
bool startGame = false;
unsigned short BroadcastListen = 8888;
unsigned short BroadcastLobby = 8877;
unsigned short PeerRequest = 8899;
unsigned short PeerListen = 8866;


class Network{
private:
    //sf::UdpSocket UdpNode, UdpHost;
    //sf::Packet BroadCastMessgae;
    std::vector <sf::IpAddress> peers;
    //Peers game_friend;
    std::string MyName;
    bool blockinStatusNode, blockinStatusHost;
public:
    Network(const std::string& Name){
        MyName = Name;
        networkName = MyName;
        blockinStatusHost = true;
        blockinStatusHost = false;
        //broadcastingMessage =  true;
        //listenbroadcasting = true;
        UdpNode.setBlocking(true);
        UdpHost.setBlocking(true);
    }

    void toggleUDPnodeblock(){
        if (blockinStatusNode){
            blockinStatusNode = false;
            UdpNode.setBlocking(blockinStatusNode);
        }
        else{
            blockinStatusNode = true;
            UdpNode.setBlocking(blockinStatusNode);
        }
    }

    void toggleUDPhostblock(){
        if(blockinStatusHost){
            blockinStatusHost = false;
            UdpHost.setBlocking(blockinStatusHost);
        } else {
            blockinStatusHost = true;
            UdpHost.setBlocking(blockinStatusHost);
        }
    }

    static bool findPeers(sf::IpAddress& PeerIP){
        auto result = std::find(std::begin(peer),std::end(peer),PeerIP);
        if (result != std::end(peer)){
            return true;
        } else {
            return false;
        }
    }


    //For Hoster who host the game
    void setupHostLobby(){
        UdpHost.bind(sf::Socket::AnyPort);
        //listener.listen(PeerListen);
        //std::Cout<<UdpHost.getLocalPort()<<std::endl;
    }
    //BroadCastLobby

    static void BroadcastlobbyTCP(){
        bool LoopLobby = true;
        std::thread broadcastTh(BroadcastMessageHost);
        broadcastTh.detach();
        if(listener.listen(PeerListen) != sf::Socket::Done){
            //error handleing
        }
        TcpNode.setBlocking(false);
        sf::Packet peerPacket;
        while(LoopLobby){
            if (listener.accept(TcpNode) == sf::Socket::Done){


            }
        }

    }

    static void BroadcastLobbyHost(){
        bool LobbyLoop = true;
        sf::IpAddress tempRecvIP;
        unsigned short tempRecvP,peerSize;
        sf::Packet tempPacket;
        std::string peerMsg,peerName;
        std::thread broadcastTh(BroadcastMessageHost);
        broadcastTh.detach();
        UdpNode.setBlocking(true);
        UdpNode.bind(PeerRequest);
        std::cout<<"Entering LobbyLoop"<<std::endl;
        while(LobbyLoop){
            //UdpHost.receive(tempPacket,tempRecvIP,tempRecvP);
            UdpNode.receive(tempPacket,tempRecvIP,tempRecvP);
            tempPacket>>peerMsg>>peerName;
            if (peerMsg == "Peer"){
                peer.push_back(tempRecvIP);
                peersName.insert(std::pair<std::string,std::string>(tempRecvIP.toString(),peerName));
            }
            if (peer.size() > 0){
                peerSize = peer.size();
                for (int i = 0; i < peerSize;i++){
                    for(int j = 0; j < peerSize;j++){
                        tempPacket<<"Peer"<<peer[j].toString()<<peersName[peer[j].toString()];
                        //UdpHost.send(tempPacket,peer[i],PeerListen);
                        UdpNode.send(tempPacket,peer[i],PeerListen);
                        tempPacket.clear();
                    }
                }
                std::this_thread::sleep_for(400ms);
                //for ()
            }
            //Temp Global variable used for testing
            if (broadcastingMessage == false){
                LobbyLoop = false;
            }
        }
        tempPacket.clear();
        int count = 100;
        while(count >= 0){
            if (peer.size() > 0){
                peerSize = peer.size();
                for (int i = 0; i < peerSize;i++){
                    for(int j = 0; j < peerSize;j++){
                        tempPacket<<"GAME"<<"GAME"<<"GAME";
                        //UdpHost.send(tempPacket,peer[i],PeerListen);
                        UdpNode.send(tempPacket,peer[i],PeerListen);
                        tempPacket.clear();
                    }
                }
            }
            count--;
        }

        //if(broadcastTh.joinable()){
        //  broadcastTh.join();
        //}

    }

    //BroadCast Message for GameHost
    static void BroadcastMessageHost(){
        UdpHost.setBlocking(false);
        BroadCastMessgae<<"DEPOKER"<<networkName;

        while(broadcastingMessage){
            UdpHost.send(BroadCastMessgae,sf::IpAddress::Broadcast,BroadcastListen);
            std::this_thread::sleep_for(100ms);


            //TCP process
            //UdpHost.send(BroadCastMessgae,sf::IpAddress::Broadcast,BroadcastListen);
            //std::this_thread::sleep_for(100ms);


        }

        //std::cout<<"Broadcasting has been ended"<<std::endl;
    }




    //Listen function for Node want to join game;
    static void listenMessageLobby(){
        sf::IpAddress tempRecvIP,broadcastListenIP, tempPeerIP;
        unsigned short tempRecvP, broadcastListenPort;
        sf::Packet tempPacket, tempPeerPacket;
        std::string checkMessage,Ip,Name;
        //std::vector <std::string>
        UdpNode.setBlocking(true);

        while(listenbroadcasting){
            std::cout<<"Enter Listening Phase";
            UdpNode.receive(tempPacket,tempRecvIP,tempRecvP);
            tempPacket>>checkMessage>>Name;
            std::cout<<tempRecvIP.toString()<<std::endl;
            std::cout<<tempRecvP<<std::endl;
            if (checkMessage == "DEPOKER"){
                peer.push_back(tempRecvIP);
                peersName.insert(std::pair<std::string,std::string>(tempRecvIP.toString(),Name));
                listenbroadcasting = false;
            }
            tempPacket.clear();
            std::this_thread::sleep_for(400ms);
        }





        //listen using UDP Port

        std::cout<<"Listen Phase finished"<<std::endl;


        tempPacket.clear();
        checkMessage = "Peer";
        tempPacket<<checkMessage<<networkName;
        UdpNode.send(tempPacket,tempRecvIP,PeerRequest);
        UdpNode.unbind();
        UdpNode.bind(PeerListen);
        tempPacket.clear();
        listenbroadcasting = true;
        UdpNode.setBlocking(true);
        while(listenbroadcasting){
            UdpNode.receive(tempPacket, tempRecvIP, tempRecvP);
            tempPacket>>checkMessage>>Ip>>Name;
            if(checkMessage == "GAME"){
                listenbroadcasting = false;

                /*
                code here for changing this to different path
                */


            } else if (checkMessage == "Peer"){
                tempPeerIP = Ip;
                if (!findPeers(tempPeerIP) && Ip != "0.0.0.0"){
                    peer.push_back(tempPeerIP);
                    peersName.insert(std::pair<std::string,std::string>(tempPeerIP.toString(),Name));
                }
            }
            std::this_thread::sleep_for(400ms);
        }



    }
    /*   if (!game_friend.findPeers(tempRecvIP)){
           game_friend.updatePeer(tempRecvIP);
       }
   }
   */
    //Node who want to join hosted game

    void setupNodeLobby(){
        UdpNode.bind(BroadcastListen);

    }

    /*
    void getConnectedpeers(){
        for (int i = 0; i < game_friend.getSize();i++){
            auto a = game_friend.returnPeer(i);
            std::cout<<a.toString()<<std::endl;
        }
    }
*/
    static void listenBroadcastMessage(){
        while(listenbroadcasting){
            //UdpHost.receive();
            // std::literals::chrono_literals::this_thread::sleep_for(10ms);

        }
    }

    static void Peerdisplay(){
        if (peer.size()>0){
            for(int ii = 0; ii< peer.size();ii++){
                std::cout<<peer[ii]<<":"<<peersName[peer[ii].toString()]<<std::endl;
            }
        }
    }


};

#endif //DEPOKER_NETWORK_H
