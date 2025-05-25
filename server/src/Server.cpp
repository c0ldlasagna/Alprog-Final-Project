#include <winsock2.h>
#include "log.hpp"
#include "Server.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include "Endpoints.hpp"
#include "Request.hpp"
#include "Response.hpp"

using json = nlohmann::json;

listenServer::listenServer(const std::string newIp, const int newPort): ip(newIp),port(newPort) {}


void listenServer::initialize(){
    //Load Database from file if it exists
    std::ifstream dbFile("data.json");
    if (dbFile.good()) {
        try {
            dbFile >> this->db;
            log("Loaded database from data.json", 0);
        } catch (const std::exception& e) {
            log(std::string("Failed to parse data.json: ") + e.what(), 1);
            this->db = json::object();
        }
    } else {
        std::ofstream newDbFile("data.json");
        this->db = json::object();
        newDbFile << this->db.dump(4);
        log("Created new data.json", 0);
    }
    //Winsock startup
    if(WSAStartup(MAKEWORD(2,2),&this->wsa) != 0){
        log("Failed to initialize winsock.",1);
    }
    log("Winsock initialized Successfully",0);

    listenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (listenSocket==INVALID_SOCKET){
        log("Error when creating listen socket",1);
    }

    listenAddr.sin_family = AF_INET;
    listenAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    listenAddr.sin_port = htons(port);

    if(bind(listenSocket,(struct sockaddr*)&listenAddr,sizeof(listenAddr)) == SOCKET_ERROR){
        log("Binding failed",1);
    }

    if(listen(listenSocket,SOMAXCONN)==SOCKET_ERROR){
        log("Listen failed",1);
    }
};

void listenServer::acceptClient(){
    while(true){
        this->clientSocket = accept(this->listenSocket,nullptr,nullptr);
        if(clientSocket == INVALID_SOCKET){
            log("Invalid Socket",1);
            continue;
        }
        handleClient(clientSocket);
    }
};

void listenServer::handleClient(SOCKET client){
    char recvBuffer[1024];
    int recvSize = recv(client, recvBuffer, sizeof(recvBuffer), 0);
    if (recvSize <= 0) return;

    std::string recv(recvBuffer, recvSize);
    log("Received: " + recv, 0);

    Request request;
    Response response;
    try {
        request = json::parse(recv);
        switch(request.method){
            case LOGIN:
                response = login(this->db,request);
                break;
            case SIGNUP:
                response = signup(this->db,request);
                break;
            default:
                response = Response{.success=false,.message="Unknown Method",.u=std::nullopt};
                break;
        }
    } catch (const std::exception& e) {
        response = Response{.success=false,.message="Server Error",.u=std::nullopt};
    }
    std::string sendBuffer = json(response).dump();
    send(client, sendBuffer.c_str(), sendBuffer.length(), 0);
}

void listenServer::shutdown(){
    std::ofstream dbFile("data.json");
    dbFile << this->db.dump(4);
    log("Database saved to data.json", 0);
    WSACleanup();
    log("Shutting down Winsock",0);
};