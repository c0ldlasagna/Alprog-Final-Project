#include "User.hpp"
#include "log.hpp"
#include "Transaction.hpp"

User::User(const std::string& username, const std::string& password)
    : username(username), password(password), balance(0.0), transactions() {}

User::User(const std::string& username, const std::string& password,const double& balance, const std::vector<Transaction>& transactions)
    : username(username), password(password), balance(balance), transactions(transactions) {}


bool User::canAfford(const double &amount) const {
    return this->balance >= amount;
}

void User::deposit(const double &amount){
    this->balance+=amount;
    this->transactions.push_back(Transaction(TDEPOSIT,amount));
    return;
}

void User::withdraw(const double &amount){
    if(this->canAfford(amount)){
        this->balance -= amount;
        this->transactions.push_back(Transaction(TWITHDRAW,amount));
        return;
    }
    else{
        throw std::runtime_error("Insufficient funds");
    }
}

void User::transfer(const double &amount,const std::string& target){
    if(this->canAfford(amount)){
        this->balance -= amount;
        this->transactions.push_back(Transaction(TTRANSFER,amount,target));
        return;
    }
    else{
        throw std::runtime_error("Insufficient funds");
    }
}

void User::receive(const double& amount, const std::string& target){
    this->balance+= amount;
    this->transactions.push_back(Transaction(TRECEIVE,amount,target));
    return;
}

