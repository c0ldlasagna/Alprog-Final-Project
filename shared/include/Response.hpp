#pragma once
#include <string>
#include "json.hpp"
#include "User.hpp"
#include <optional>

struct Response{
    bool success;
    std::string message;
    std::optional<User> u;
};

inline void to_json(nlohmann::json& json,const Response& r){
    json = nlohmann::json{
        {"success", r.success},
        {"message", r.message},
        {"user", r.u}
    };
}

inline void from_json(const nlohmann::json& json, Response& r){
    json.at("success").get_to(r.success);
    json.at("message").get_to(r.message);
    if (json.contains("user") && !json.at("user").is_null()) {
        r.u = json.at("user").get<User>();
    } else {
        r.u = std::nullopt;
    }
}
