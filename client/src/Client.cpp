#include <winsock2.h>
#include "log.hpp"
#include "Client.hpp"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;


Client::Client(
                const std::string serverAddr,
                const int serverPort
                ){
                  this->serverAddr.sin_family = AF_INET;
                  this->serverAddr.sin_addr.s_addr = inet_addr(serverAddr.c_str());
                  this->serverAddr.sin_port = htons(serverPort);
};

void Client::initialize(){
    if(WSAStartup(MAKEWORD(2,2),&this->wsa)!=0){
        log("Failed to initialize winsock.",1);
    }
    log("Client initialization Successful",0);    
}

json Client::request(const json request){
    char recvBuffer[1024];
    int recvSize;

    this->clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(this->clientSocket == INVALID_SOCKET){
        log("Client socket creation failed",1);
    }

    if(connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR){
        log("Failed to connect to server",1);    
    }

    std::string payload = request.dump().c_str();

    std::cout << payload << std::endl;

    if(send(clientSocket,payload.c_str(),(int)payload.size(),0)==SOCKET_ERROR){
        log("Error when sending request",1);
    }

    recvSize = recv(clientSocket,recvBuffer,sizeof(recvBuffer),0);
    
    std::string recvStr(recvBuffer,recvSize);

    json response = json::parse(recvStr);

    closesocket(clientSocket);

    return response;

}

void Client::shutdown(){
    WSACleanup();
    log("Client is shutting down...",0);
}

