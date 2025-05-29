#include <ftxui/component/component.hpp>
#include <string>
#include "Client.hpp"
#include "json.hpp"
#include "Request.hpp"
#include "ui/Login.hpp"
#include "ui/components/Logo.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include "Response.hpp"
#include <ftxui/component/component_options.hpp>

using namespace ftxui;
using json = nlohmann::json;

Component LoginForm(std::shared_ptr<Client> c) {
    auto message = std::make_shared<std::string>("");
    std::string* username = &c->session.user.username;
    std::string* password = &c->session.user.password;
    int* loggedIn = &c->session.loggedIn;

    InputOption usernameOpt;
    usernameOpt.password = false;
    usernameOpt.multiline = false;

    InputOption passwordOpt;
    passwordOpt.password = true;
    passwordOpt.multiline = false;

    auto usernameInput = Input(username, "Username", usernameOpt);
    auto passwordInput = Input(password, "Password", passwordOpt);

    auto loginButton = Button("Login", [=] {
        try{
            Response resp = c->request(Request{.method = LOGIN, .username=*username, .password =*password});
        if (resp.success) {
            *loggedIn = 1;
            *message = "Logged in as"+resp.u.value().username;
        } else {
            *loggedIn = false;
            *message = "Failed to log in";
        }
        }
        catch(const std::exception&){
            *loggedIn = 0;
            *message = "Failed to connect to server.";
        }
        
    },ButtonOption::Animated());

    auto signupButton = Button("Sign Up", [=] {
        try{
            Response resp = c->request(Request{.method=SIGNUP,.username=*username,.password=*password});
        if (resp.success) {
            *loggedIn = 1;
            *message = "Signed up successfully! You can login now.";
        } else {
            *loggedIn = 0;
            *message = "Failed to sign up";
        }}
        catch(std::exception&){
            *loggedIn = 0;
            *message = "Failed to connect to server";
        }
    });

    auto inputs = Container::Vertical({
        usernameInput,
        passwordInput
    });

    auto buttons = Container::Horizontal({
        loginButton,
        signupButton
    });

    auto form = Container::Vertical({
        inputs,
        buttons
    });

    auto renderer = Renderer(form, [=] {
        return vbox(
            flex_grow(vcenter(hcenter(Logo()|color(Color::RGBA(255,0,0,0xFF))))),
            hcenter(window(text(" Login/Sign Up: ") | bold, vbox({
                separatorEmpty(),
                hbox(text(" Username:"), separatorEmpty(), usernameInput->Render()),
                separatorEmpty(),
                hbox(text(" Password:"), separatorEmpty(), passwordInput->Render()),
                separatorEmpty(),
                hbox(filler(),loginButton->Render(),filler(), signupButton->Render(),filler()),
                separatorEmpty(),
                text(*message) | color(Color::RedLight)
            }), BorderStyle::ROUNDED) | size(WIDTH, EQUAL, 75)) | size(HEIGHT, EQUAL, 15),
            filler()
        );
    });

    return renderer;
}
