#pragma once
#include <ftxui/component/component.hpp>

ftxui::Element Logo(){
    return ftxui::vbox(
            ftxui::text(R"(          ████████▄  ▄██   ▄    ▄█  ███▄▄▄▄     ███)"),
            ftxui::text(R"(  _____   ███   ▀███ ███   ██▄ ███  ███▀▀▀██▄   ██ )"),
            ftxui::text(R"( /     \  ███    ███ ███▄▄▄███ ███▌ ███   ███   █▀ )"),
            ftxui::text(R"(| () () | ███    ███ ▀▀▀▀▀▀███ ███▌ ███   ███      )"),
            ftxui::text(R"( \  ^  /  ███    ███ ▄██   ███ ███▌ ███   ███      )"),
            ftxui::text(R"(  |||||   ███    ███ ███   ███ ███  ███   ███      )"),
            ftxui::text(R"(  |||||   ███   ▄███ ███   ███ ███  ███   ███      )"),
            ftxui::text(R"(          ████████▀   ▀█████▀  █▀    ▀█   █▀       )")
    );
};

ftxui::Element Skull(){
    return ftxui::vbox(
            ftxui::text(R"(         )"),
            ftxui::text(R"(  _____  )"),
            ftxui::text(R"( /     \ )"),
            ftxui::text(R"(| () () |)"),
            ftxui::text(R"( \  ^  / )"),
            ftxui::text(R"(  |||||  )"),
            ftxui::text(R"(  |||||  )"),
            ftxui::text(R"(         )")
    );
};