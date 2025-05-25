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
        
        double deposit(const double &amount);
        double withdraw(const double &amount);

        bool canAfford(const double &amount) const;
};

inline void from_json(const nlohmann::json& j, User& u) {
    j.at("username").get_to(u.username);
    j.at("password").get_to(u.password);
    j.at("balance").get_to(u.balance);
    if (j.contains("transactions")) {
        u.transactions = j.at("transactions").get<std::vector<Transaction>>();
    }
};

inline void to_json(nlohmann::json& j, const User& u) {
    j = nlohmann::json{
        {"username", u.username},
        {"password", u.password},
        {"balance", u.balance},
        {"transactions", u.transactions}
    };
};