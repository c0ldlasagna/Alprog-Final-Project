#include "log.hpp"
#include "Server.hpp"

int main(){
    listenServer srv("127.0.0.1",6969);

    srv.initialize();

    srv.acceptClient();

    srv.shutdown();
    
    return 0;
    
}