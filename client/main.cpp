#include "Client.hpp"
#include "json.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <memory>
#include <string>
#include "ui/Login.hpp"

using namespace ftxui;
using json = nlohmann::json;

int main() {
    // Setup client connection
    auto client = std::make_shared<Client>("127.0.0.1", 6969);
    client->initialize();

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(LoginForm(client));

    client->shutdown();
    
    return 0;
}