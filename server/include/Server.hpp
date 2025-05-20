#pragma once 
#include <winsock2.h>
#include <string>
#include "json.hpp"

class listenServer{
    private:
        WSADATA wsa;
        SOCKET clientSocket,listenSocket;
        nlohmann::json db;
        struct sockaddr_in clientAddr,listenAddr;
        std::string ip;
        int port;
        void handleClient(const SOCKET client);

    public:
        listenServer(const std::string newIp,const int newport);
        void initialize();
        void acceptClient();
        void shutdown();

};

