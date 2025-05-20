#include "User.hpp"
#include "log.hpp"

User::User( const std::string &newUsername,const std::string &newPassword):
            username(newUsername),
            password(newPassword),balance(0.0)  {};


std::optional<User> User::from_json(const nlohmann::json &data){
    try{
        User u(data.at("username"),data.at("password"));
        return u;
    }
    catch(const std::exception&){
        log("Error when parsing data",1);
        return std::nullopt;
    }
}

std::string User::getUsername()const{
    return this->username;
}

std::string User::setUsername(const std::string &newUsername){
    this->username = newUsername;
    return this->username;
}

std::string User::getPassword()const{
    return this->password;
}

std::string User::setPassword(const std::string &newPassword){
    this->password = newPassword;
    return this->password;
}

double User::getBalance()const{
    return this->balance;
}

double User::deposit(const double &amount){
    this->balance += amount;
    return this->balance;
}

double User::withdraw(const double &amount){
    this->balance -= amount;
    return this->balance;
}