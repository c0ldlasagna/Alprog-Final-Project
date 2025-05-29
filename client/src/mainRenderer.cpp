#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "ui/Login.hpp"
#include "ui/Dashboard.hpp"
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

Component mainRenderer(std::shared_ptr<Client> c) {
    int* selected = &c->session.loggedIn;
    auto loginForm = LoginForm(c);
    auto dashboard = Dashboard(c);

    auto mainContainer = Container::Tab({
        loginForm,
        dashboard
    },selected) | border | bgcolor(Color::RGBA(0,0,0,0XFF));

    return mainContainer;
}