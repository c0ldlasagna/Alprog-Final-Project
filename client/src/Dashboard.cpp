#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include "User.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Transaction.hpp"
#include "ui/components/Logo.hpp"
#include "Client.hpp"
#include <string>
#include <iomanip>
#include <sstream>

using namespace ftxui;

Component Dashboard(std::shared_ptr<Client> c){
    std::shared_ptr<std::string> message = std::make_shared<std::string>("");  
    std::shared_ptr<std::string> amount = std::make_shared<std::string>("");
    std::shared_ptr<std::string> target = std::make_shared<std::string>("");
    std::shared_ptr<int> page = std::make_shared<int>(0);
    std::ostringstream ss;

    InputOption amountOpts = InputOption{
        .multiline = false
    };


    auto menu = Container::Vertical({
        MenuEntry(MenuEntryOption{.label="Deposit"}),
        MenuEntry(MenuEntryOption{.label="Withdraw"}),
        MenuEntry(MenuEntryOption{.label="Transfer"}),
        //MenuEntry(MenuEntryOption{.label ="History"})
    },page.get());

    auto amountInputDeposit = Input(amount.get(),"Amount",amountOpts) | CatchEvent([=](Event event){
        if (!event.is_character())
        return false; // Let non-character events (like Enter, arrows) pass

        char c = event.character()[0];

        if (std::isdigit(c))
            return false; // Allow digits

        if (c == '.' && amount->find('.') == std::string::npos)
            return false; // Allow only one decimal point

        return true; // Block everything else
    });

    auto amountInputWithdraw = Input(amount.get(),"Amount",amountOpts) | CatchEvent([=](Event event){
        if (!event.is_character())
        return false; // Let non-character events (like Enter, arrows) pass

        char c = event.character()[0];

        if (std::isdigit(c))
            return false; // Allow digits

        if (c == '.' && amount->find('.') == std::string::npos)
            return false; // Allow only one decimal point

        return true; // Block everything else
    });

    auto amountInputTransfer = Input(amount.get(),"Amount",amountOpts) | CatchEvent([=](Event event){
        if (!event.is_character())
        return false; // Let non-character events (like Enter, arrows) pass

        char c = event.character()[0];

        if (std::isdigit(c))
            return false; // Allow digits

        if (c == '.' && amount->find('.') == std::string::npos)
            return false; // Allow only one decimal point

        return true; // Block everything else
    });

    auto targetInput = Input(target.get(),"Recipient",amountOpts);

    auto depositButton = Button("Deposit",[=]{
        try{
            Response resp = c->request(Request{
                .method = DEPOSIT, 
                .username = c->session.user.username, 
                .password = c->session.user.password, 
                .amount=std::stod(*amount)});
            if(resp.success){
                c->session.user = resp.u.value();
                *message = "Deposit Successful!";
            }
            else{
                c->session.user = resp.u.value();
                *message = "Something went wrong.";
            }
        }
        catch(std::exception &e){
            *message = "Failed to connect to server.";
        }
    });

    auto withdrawButton = Button("Withdraw",[=]{
        try{
            Response resp = c->request(Request{
                .method = WITHDRAW, 
                .username = c->session.user.username, 
                .password = c->session.user.password, 
                .amount=std::stod(*amount)});
            if(resp.success){
                c->session.user = resp.u.value();
                *message = "Withdraw Successful!";
            }
            else{
                c->session.user = resp.u.value();
                *message = "Something went wrong.";
            }
        }
        catch(std::exception &e){
            *message = "Failed to connect to server.";
        }
    });

    auto transferButton = Button("Transfer",[=]{
        try{
            Response resp = c->request(Request{
                .method = TRANSFER, 
                .username = c->session.user.username, 
                .password = c->session.user.password, 
                .target = *target,
                .amount = std::stod(*amount)
            });
            if(resp.success){
                c->session.user = resp.u.value();
                *message = "Transfer Successful!";
            }
            else{
                c->session.user = resp.u.value();
                *message = "Something went wrong.";
            }
        }
        catch(std::exception &e){
            *message = "Failed to connect to server.";
        }
    });

   auto depositTab = Container::Vertical({
        amountInputDeposit,
        depositButton
    });

    auto withdrawTab = Container::Vertical({
        amountInputWithdraw,
        withdrawButton
    });

    auto transferTab = Container::Vertical({
        amountInputTransfer,
        targetInput,
        transferButton
    });

    auto actionsTab = Container::Tab({
        depositTab,
        withdrawTab,
        transferTab
    }, &*page);

    auto mainContainer = Container::Horizontal({
        menu,
        actionsTab
    });

    auto renderer = Renderer(mainContainer, [=] {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << c->session.user.balance;
        return hbox(
            separatorEmpty(),
            vbox(
            hcenter((Skull()) | color(Color::Color::RGBA(0xFF,0,0,0xFF))),
            //vcenter(text("Hello, " + c->session.user.username))|flex_grow,
            menu->Render(),
            text("Log Out")
            ),
            separatorEmpty(),   
            separator(),
            separatorEmpty(),
            window(
                text("Your balance is "+ ss.str()) |color(Color::RGBA(0x33,0xFF,0x12,0xFF)),
                vbox({
                    *page == 0 ? depositLogo() :
                    *page == 1 ? withdrawLogo() :       
                    *page == 2 ? transferLogo() :
                    *page == 3 ? historyLogo() :
                    text(""),
                    actionsTab->Render()
                })
            )|flex_grow
        );}
    );
return renderer;
}