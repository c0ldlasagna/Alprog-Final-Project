#pragma once
#include <string>
#include "json.hpp"

enum Method {
    LOGIN,
    SIGNUP,
    DEPOSIT,
    WITHDRAW,
    TRANSFER
};


struct Request {
    Method method;
    std::string username, password, target;
    double amount;
};

inline void to_json(nlohmann::json& j, const Request& r) {
    j = nlohmann::json{
        {"method", r.method},
        {"username", r.username},
        {"password", r.password},
        {"target", r.target},
        {"amount", r.amount}
    };
}

inline void from_json(const nlohmann::json& j, Request& r) {
    j.at("method").get_to(r.method);
    j.at("username").get_to(r.username);
    j.at("password").get_to(r.password);
    j.at("target").get_to(r.target);
    j.at("amount").get_to(r.amount);
}
