#include "log.hpp"
#include "Server.hpp"

#include <csignal>
#include <atomic>

std::atomic<bool> running(true);
listenServer* globalServer = nullptr;

void signalHandler(int signum) {
    if (globalServer) {
        globalServer->shutdown();
    }
    running = false;
}

int main(){
    listenServer srv("127.0.0.1",6969);

    srv.initialize();

    srv.acceptClient();

    srv.shutdown();
    
    return 0;
    
}