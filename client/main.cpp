#include "log.hpp"
#include "Client.hpp"
#include "json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

void printMenu() {
    std::cout << "==== something app something idk ====\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Fetch Info\n";
    std::cout << "4. Exit\n";
    std::cout << "Select an option: ";
}

int main() {
    Client c("127.0.0.1", 6969);
    c.initialize();

    bool loggedIn = false;
    std::string username, password;

    while (true) {
        printMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            json request = {
                {"method", "register"},
                {"username", username},
                {"password", password}
            };
            auto resp = c.request(request);
            std::cout << resp.dump(4) << std::endl;
        } else if (choice == 2) {
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            json request = {
                {"method", "login"},
                {"username", username},
                {"password", password}
            };
            auto resp = c.request(request);
            std::cout << resp.dump(4) << std::endl;
            if (resp.contains("status") && resp["status"] == "success") {
                loggedIn = true;
            } else {
                loggedIn = false;
            }
        } else if (choice == 3) {
            if (!loggedIn) {
                std::cout << "You must login first.\n";
                continue;
            }
            json request = {
                {"method", "info"},
                {"username", username}
            };
            auto resp = c.request(request);
            std::cout << resp.dump(4) << std::endl;
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }

    c.shutdown();
    return 0;
}