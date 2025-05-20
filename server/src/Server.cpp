#include <winsock2.h>
#include "log.hpp"
#include "Server.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include "Endpoints.hpp"

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

    std::string recvStr(recvBuffer, recvSize);
    log("Received: " + recvStr, 0);

    json recvjson;
    json sendjson;
    try {
        recvjson = json::parse(recvStr);
        std::string method = recvjson.value("method", "");
        if (method == "register") {
            sendjson = registerUser(this->db, recvjson);
        } else if (method == "login") {
            sendjson = loginUser(this->db, recvjson);
        } else if (method == "info") {
            sendjson = getUserData(this->db, recvjson);
        } else if (method == "ping") {
            sendjson = {{"response", "pong"}};
        } else {
            sendjson = {{"status", "fail"}, {"error", "Unknown method"}};
        }
    } catch (const std::exception& e) {
        sendjson = {{"status", "fail"}, {"error", e.what()}};
    }

    std::string sendBuffer = sendjson.dump();
    send(client, sendBuffer.c_str(), sendBuffer.length(), 0);
}

void listenServer::shutdown(){
    std::ofstream dbFile("data.json");
    dbFile << this->db.dump(4);
    log("Database saved to data.json", 0);
    WSACleanup();
    log("Shutting down Winsock",0);
};