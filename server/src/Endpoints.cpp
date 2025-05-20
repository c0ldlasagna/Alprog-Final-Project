#include "Endpoints.hpp"
#include "json.hpp"
#include "log.hpp"
#include <fstream>

using json = nlohmann::json;

json registerUser(json& db, const json& data) {
    std::string username = data.value("username", "");
    std::string password = data.value("password", "");
    if (username.empty() || password.empty()) {
        return {{"status", "fail"}, {"error", "Missing username or password"}};
    }
    if (db.contains(username)) {
        return {{"status", "fail"}, {"error", "User already exists"}};
    }
    db[username] = {{"password", password}, {"balance", 0.0}};
    log("Registered user: " + username, 0);
    // Write to file on change
    std::ofstream dbFile("data.json");
    if (dbFile.is_open()) {
        dbFile << db.dump(4);
        dbFile.close();
        log("Database saved to data.json after registration", 0);
    } else {
        log("Failed to open data.json for writing after registration", 1);
    }
    return {{"status", "success"}};
}

json loginUser(json& db, const json& data) {
    std::string username = data.value("username", "");
    std::string password = data.value("password", "");
    if (!db.contains(username)) {
        return {{"status", "fail"}, {"error", "User not found"}};
    }
    if (db[username]["password"] != password) {
        return {{"status", "fail"}, {"error", "Incorrect password"}};
    }
    log("User logged in: " + username, 0);
    return {{"status", "success"}};
}

json getUserData(json& db, const json& data) {
    std::string username = data.value("username", "");
    if (!db.contains(username)) {
        return {{"status", "fail"}, {"error", "User not found"}};
    }
    double balance = db[username].value("balance", 0.0);
    return {{"status", "success"}, {"username", username}, {"balance", balance}};
}

