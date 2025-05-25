#include "Endpoints.hpp"
#include "json.hpp"
#include "log.hpp"
#include <fstream>
#include "Response.hpp"
#include "Request.hpp"

using json = nlohmann::json;

Response login(json& db,const Request& request){
    if(request.username.empty() || request.password.empty()){
        return {false, "Missing username or password", std::nullopt};
    }
    if(!db.contains(request.username)){
        return {false, "User not found", std::nullopt};
    }
    if(db[request.username]["password"] != request.password){
        return {false, "Incorrect password", std::nullopt};
    }
    log("User logged in: " + request.username, 0);
    User user;
    user.username = request.username;
    user.password = request.password;
    user.balance = db[request.username].value("balance", 0.0);  
    user.transactions = db[request.username].value("transactions", std::vector<Transaction>());
    return {true, "Login successful", user};
}

Response signup(json& db, const Request& request) {
    if (request.username.empty() || request.password.empty()) {
        return {false, "Missing username or password", std::nullopt};
    }
    if (db.contains(request.username)) {
        return {false, "Username already exists", std::nullopt};
    }
    db[request.username] = {
        {"password", request.password},
        {"balance", 0.0},
        {"transactions", json::array()}
    };
    log("User signed up: " + request.username, 0);
    User user;
    user.username = request.username;
    user.password = request.password;
    user.balance = 0.0;
    user.transactions = {};
    return {true, "Signup successful", user};
}

