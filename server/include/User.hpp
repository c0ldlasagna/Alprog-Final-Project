#pragma once
#include <string>
#include "json.hpp"

class User{

    private:
        
        std::string username;
        std::string password;
        double balance;
        User(const std::string &newUsername,const std::string &newPassword);
    
    public:

        //JSON Factory Method 
        static std::optional<User> from_json(const nlohmann::json &data);
        
        std::string getUsername()const;
        std::string setUsername(const std::string &newUserame);

        std::string getPassword()const;
        std::string setPassword(const std::string &newPassword);

        double getBalance()const;
        double deposit(const double &amount);
        double withdraw(const double &amount);

};