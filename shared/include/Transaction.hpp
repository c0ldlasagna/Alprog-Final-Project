#pragma once
#include <string>
#include "json.hpp"
#include <vector>
#include <optional>
#include <chrono>

enum TransactionType{
    TDEPOSIT,
    TTRANSFER,
    TRECEIVE,
    TWITHDRAW
};

struct Transaction{
    TransactionType type;
    double amount;
    std::string target;
    long long timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
};

inline void to_json(nlohmann::json& j, const TransactionType& t) {
    j = static_cast<int>(t);
};

inline void from_json(const nlohmann::json& j, TransactionType& t) {
    int value = j.get<int>();
    t = static_cast<TransactionType>(value);
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


