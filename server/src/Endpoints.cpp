#include "Endpoints.hpp"
#include "json.hpp"
#include "log.hpp"
#include <fstream>
#include "Response.hpp"
#include "Request.hpp"
#include "Transaction.hpp"
#include <chrono>

using json = nlohmann::json;

Response login(json& db,const Request& request){
    //Check if request is valid
    if(request.username.empty() || request.password.empty()){
        return {false, "Missing username or password", std::nullopt};
    }
    //Check if user exists
    if(!db.contains(request.username)){
        return {false, "User not found", std::nullopt};
    }
    //Check if password matches
    //OPTIONAL TODO: hash the password before storage
    if(db[request.username]["password"] != request.password){
        return {false, "Incorrect password", std::nullopt};
    }
    //Log to stdout that user has logged in
    log("User logged in: " + request.username, 0);
    //Send user object to the client
    User user = db[request.username];

    user.transactions = db[request.username].value("transactions", std::vector<Transaction>());
    return {true, "Login successful", user};
}

Response signup(json& db, const Request& request) {

    //Check if request is valid
    if (request.username.empty() || request.password.empty()) {
        return {false, "Missing username or password", std::nullopt};
    }

    //Check if another user has the same name
    if (db.contains(request.username)) {
        return {false, "Username already exists", std::nullopt};
    }

    //Create the new user
    User user(request.username,request.password);

    //Add User to database
    db[user.username] = user;

    log("User signed up: " + request.username, 0);

    return {true, "Signup successful", user};
}

Response deposit(json& db, const Request& request) {
    
    //Check if request is valid
    if (request.username.empty() || request.amount <= 0) {
        return {false, "Invalid username or amount", std::nullopt};
    }
    
    //Check if User exists
    if (!db.contains(request.username)) {
        return {false, "User not found", std::nullopt};
    }

    //Get user from database
    User user = db[request.username];

    //Deposit money to account
    user.deposit(request.amount);

    log("Deposit made by user: " + request.username + ", Amount: " + std::to_string(request.amount), 0);

    db[request.username] = {user};

    return {true, "Deposit successful", user};
}

Response withdraw(json& db, const Request& request) {
    //Check if request is valid
    if (request.username.empty() || request.amount <= 0) {
        return {false, "Invalid username or amount", std::nullopt};
    }
    //Check if user exists
    if (!db.contains(request.username)) {
        return {false, "User not found", std::nullopt};
    }
    //Get user from database
    User user = db[request.username];
    
    try{
        user.withdraw(request.amount);
    }
    catch(std::exception &e){
        return {false,e.what()};
    }

    log("Withdrawal made by user: " + request.username + ", Amount: " + std::to_string(request.amount), 0);
    return {true, "Withdrawal successful", user};
}

Response transfer(json& db, const Request& request) {

    //Check if request is valid
    if (request.username.empty() || request.target.empty() || request.amount <= 0) {
        return {false, "Invalid username, target or amount", std::nullopt};
    }
    //Check if sender exists
    if (!db.contains(request.username)) {
        return {false, "Sender not found", std::nullopt};
    }
    //Check if recipient exists
    if (!db.contains(request.target)) {
        return {false, "Recipient not found", std::nullopt};
    }
    User sender = db[request.username];
    User recipient = db[request.target];

    //Take money from sender
    try{
        sender.transfer(request.amount,request.target);
    }
    catch(std::exception &e){
        log(e.what(),1);
        return {false,e.what()};
    }

    //Send money to recipient
    recipient.receive(request.amount,request.username);

    log("Transfer made by user: " + request.username + ", Amount: " + std::to_string(request.amount) + ", Target: " + request.target, 0);

    return {true, "Transfer successful", sender};
}