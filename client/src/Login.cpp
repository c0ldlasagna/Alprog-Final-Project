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
            c->session.user = resp.u.value();
        } else {
            *loggedIn = false;
            *message = "Failed to log in";
            username->clear();
            password->clear();
        }
        }
        catch(const std::exception&){
            *loggedIn = 0;
            *message = "Failed to connect to server.";
            username->clear();
            password->clear();
        }
        
    },ButtonOption::Animated(Color::RGBA(0xff,0,0,0xFF)));

    auto signupButton = Button("Sign Up", [=] {
        try{
            Response resp = c->request(Request{.method=SIGNUP,.username=*username,.password=*password});
        if (resp.success) {
            *loggedIn = 0;
            *message = "Signed up successfully! You can login now.";
            username->clear();
            password->clear();
        } else {
            *loggedIn = 0;
            *message = "Failed to sign up";
            username->clear();
            password->clear();
        }}
        catch(std::exception&){
            *loggedIn = 0;
            *message = "Failed to connect to server";
            username->clear();
            password->clear();
        }
    },ButtonOption::Animated(Color::RGBA(0,0xff,0,0xFF)));

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
                hcenter(text(*message))
            }), BorderStyle::ROUNDED) | size(WIDTH, EQUAL, 75)) | size(HEIGHT, EQUAL, 15)| color(Color::RGBA(0xff,0xff,0xff,0xff)),
            filler()
        )| bgcolor(LinearGradient()
                    .Angle(90)
                    .Stop(Color::Black)
                    .Stop(Color::RGBA(25,0,0,127)));
    });

    return renderer;
}
