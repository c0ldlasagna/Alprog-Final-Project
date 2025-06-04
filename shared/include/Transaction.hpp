#pragma once
#include <string>
#include "json.hpp"
#include <vector>
#include <optional>
#include <chrono>
#include <iomanip>
#include <sstream>

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

inline void to_string(const Transaction& t, std::vector<std::string> &s){
    std::time_t time = static_cast<std::time_t>(t.timestamp);

    std::tm tm_time;
    #ifdef _WIN32
        localtime_s(&tm_time, &time);
    #else
        localtime_r(&time, &tm_time);
    #endif
        std::ostringstream oss;
        oss << std::put_time(&tm_time, "%d/%m/%Y %H:%M");
        std::string formatted_time = oss.str();
    
    std::string type;
    switch (t.type) {
        case TDEPOSIT:   type = "Deposit"; break;
        case TTRANSFER:  type = "Transfer"; break;
        case TRECEIVE:   type = "Receive"; break;
        case TWITHDRAW:  type = "Withdraw"; break;
        default:         type = "Unknown"; break;
    }

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << t.amount;

    std::string description;
    switch(t.type){
        case TTRANSFER : description = "Transfer to "+ t.target; break;
        case TRECEIVE  : description = "Transfer from "+ t.target; break;
        default : description = ""; break;
    }

    std::string from;
    
    s = {
        formatted_time,
        type,
        ss.str(),
        description
    };
}
