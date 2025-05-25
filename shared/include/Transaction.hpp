#pragma once
#include <string>
#include "json.hpp"
#include <vector>
#include <optional>

enum TransactionType{
    TDEPOSIT,
    TSEND,
    TRECEIVE,
    TWITHDRAW
};

struct Transaction{
    TransactionType type;
    double amount;
    std::string target;
    std::string timestamp; 
};

inline void to_json(nlohmann::json& j, const Transaction& t) {
    j = nlohmann::json{
        {"type", t.type},
        {"amount", t.amount},
        {"target", t.target},
        {"timestamp", t.timestamp}
    };
}

inline void from_json(const nlohmann::json& j, Transaction& t) {
    j.at("type").get_to(t.type);
    j.at("amount").get_to(t.amount);
    j.at("target").get_to(t.target);
    j.at("timestamp").get_to(t.timestamp);
}


