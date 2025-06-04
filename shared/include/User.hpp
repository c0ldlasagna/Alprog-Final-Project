#pragma once
#include <string>
#include "json.hpp"
#include <vector>
#include <optional>
#include "Transaction.hpp"


class User{

    public:

        std::string username;
        std::string password;
        double balance;
        std::vector<Transaction> transactions;

        bool canAfford(const double &amount) const;

        User(const std::string& username,const std::string& password);
        User(const std::string& username, const std::string& password,const double& balance, const std::vector<Transaction>& transactions);
        User() = default;  

        void deposit(const double &amount);
        void withdraw(const double &amount);
        void transfer(const double &amount, const std::string& target);
        void receive(const double &amount, const std::string& target);
};

inline void from_json(const nlohmann::json& j, User& u) {
    j.at("username").get_to(u.username);
    j.at("password").get_to(u.password);
    j.at("balance").get_to(u.balance);
    j.at("transactions").get_to(u.transactions);    
}

inline void to_json(nlohmann::json& j, const User& u) {
    j = nlohmann::json{
        {"username", u.username},
        {"password", u.password},
        {"balance", u.balance},
        {"transactions", u.transactions}
    };
};