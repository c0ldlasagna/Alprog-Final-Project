#pragma once
#include <winsock2.h>
#include "json.hpp"

class Client{
    private:
        WSADATA wsa;
        SOCKET clientSocket;
        struct sockaddr_in serverAddr;
    public:
        Client(const std::string serverAddr,const int serverPort);
        void initialize();
        nlohmann::json request(const nlohmann::json request);
        void shutdown();
};