#include "User.hpp"
#include "log.hpp"

bool User::canAfford(const double &amount) const {
    return this->balance >= amount;
};

double User::deposit(const double &amount){
    this->balance += amount;
    return this->balance;
}

double User::withdraw(const double &amount){
    this->balance -= amount;
    return this->balance;
}