#include "log.hpp"
#include <string>
#include <iostream>

void log(const std::string &message,const int &mode){
    std::string prompt;
    mode == 1 ? prompt = "\033[0;31m[ERROR] " : mode == 2 ? prompt = "\033[0;35m[INPUT] ": prompt = "\033[0;34m[INFO]  ";
    std::cout << prompt << "\033[0m" << message << std::endl;
    return;
}