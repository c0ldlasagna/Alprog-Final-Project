#pragma once
#include <winsock2.h>
#include "json.hpp"
#include "User.hpp"

struct SessionState{
    bool loggedIn;
    User user;
};

class Client{
    private:
        WSADATA wsa;
        SOCKET clientSocket;
        struct sockaddr_in serverAddr;
    public:
        SessionState session;
        Client(const std::string serverAddr,const int serverPort);
        void initialize();
        nlohmann::json request(const nlohmann::json request);
        void shutdown();
};