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

using namespace ftxui;
using json = nlohmann::json;

Component LoginForm(std::shared_ptr<Client> c) {
    auto message = std::make_shared<std::string>("");
    std::string* username = &c->session.user.username;
    std::string* password = &c->session.user.password;
    bool* loggedIn = &c->session.loggedIn;

    InputOption usernameOpt;
    usernameOpt.password = false;
    usernameOpt.multiline = false;

    InputOption passwordOpt;
    passwordOpt.password = true;
    passwordOpt.multiline = false;

    auto usernameInput = Input(username, "Username", usernameOpt);
    auto passwordInput = Input(password, "Password", passwordOpt);

    auto loginButton = Button("Login", [=] {
        Response resp = c->request(Request{.method = LOGIN, .username=*username, .password =*password});
        if (resp.success) {
            *loggedIn = true;

            *message = "Logged in as"+resp.u.value().username;
        } else {
            *loggedIn = false;
            *message = "Failed to log in";
        }
    });

    auto signupButton = Button("Sign Up", [=] {
        Response resp = c->request(Request{.method=SIGNUP,.username=*username,.password=*password});
        if (resp.success) {
            *loggedIn = true;
            *message = "Signed up successfully";
        } else {
            *loggedIn = false;
            *message = "Failed to sign up";
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
                hbox(loginButton->Render() | flex, signupButton->Render() | flex),
                separatorEmpty(),
                text(*message) | color(Color::RedLight)
            }), BorderStyle::ROUNDED) | size(WIDTH, EQUAL, 75)) | size(HEIGHT, EQUAL, 15),
            filler()
        )|borderRounded|bgcolor(Color::RGBA(0,0,0,0xFF));
    });

    return renderer;
}
