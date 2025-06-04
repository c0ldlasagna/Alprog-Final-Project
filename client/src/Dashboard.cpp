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
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/table.hpp>
#include <vector>
#include "sort.hpp"
#include <iterator>

using namespace ftxui;

MenuEntryOption Colored(ftxui::Color c){
    MenuEntryOption option;
    option.animated_colors.foreground.enabled = true;
    option.animated_colors.background.enabled = true;
    option.animated_colors.background.active = c;
    option.animated_colors.background.inactive = Color::Black;
    option.animated_colors.foreground.active = Color::White;
    option.animated_colors.foreground.inactive = c;
    return option;
};

Component Dashboard(std::shared_ptr<Client> c){
    auto message = std::make_shared<std::string>("");  
    auto amount = std::make_shared<std::string>("");
    auto target = std::make_shared<std::string>("");
    auto page = std::make_shared<int>(0);
    auto historyPage = std::make_shared<int>(0);
    std::ostringstream ss;

    InputOption amountOpts = InputOption{
        .multiline = false
    };

    auto logoutButton = Button("Logout",[=]{
        c->session.loggedIn = 0;
        c->session.user = User();
        message->clear();
    }, ButtonOption::Animated(Color::RGBA(0xff,0,0,0xff)));


    auto menu = Container::Vertical({
        MenuEntry("Deposit",Colored(Color::RGBA(0xff,0x80,0x00,0xff))),
        MenuEntry("Withdraw",Colored(Color::RGBA(0xc4,0x5e,0xa7,0xff))),
        MenuEntry("Transfer",Colored(Color::RGBA(0x97,0xcd,0xbe,0xff))),
        MenuEntry(MenuEntryOption{.label ="History"})
        
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
            *message = "Something went wrong.";
        }
        amount->clear();
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
                *message = resp.message;
            }
            else{
                c->session.user = resp.u.value();
                *message = resp.message;
            }
        }
        catch(std::exception &e){
            *message = "Something went wrong.";
        }
        amount->clear();
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
                *message = resp.message;
            }
            else{
                c->session.user = resp.u.value();
                *message = resp.message;
            }
        }
        catch(std::exception &e){
            *message = "Something went wrong.";
        }
        amount->clear();
    });

    auto nextButton = Button("   >   ",[=]{
        int(c->session.user.transactions.size()) > ((*historyPage)+1) * 7 ? (*historyPage)++ : *historyPage+=0;
    });

    auto prevButton = Button("   <   ",[=]{
        *historyPage>0 ? --(*historyPage) : *historyPage = 0;
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

    auto historyTab = Container::Horizontal({
        prevButton,
        nextButton
    });

    auto actionsTab = Container::Tab({
        depositTab,
        withdrawTab,
        transferTab,
        historyTab
    }, &*page);

    auto sidebar = Container::Vertical({
        menu,
        logoutButton
    });

    auto mainContainer = Container::Horizontal({
        sidebar,
        actionsTab
    });

    auto renderer = Renderer(mainContainer, [=] {
        std::vector<Element> table_rows;

        // Header row
        table_rows.push_back(
            hbox({
                text("Date") | bold | size(WIDTH,EQUAL,20),
                text("Type") | bold | flex| size(WIDTH,EQUAL,20),
                text("Amount") | bold | flex| size(WIDTH,EQUAL,15),
                text("Description") | bold | flex | align_right
            })
        );

        // Sort transactions
        quickSortTransactions(c->session.user.transactions, 0, c->session.user.transactions.size() - 1, SortMode::DATE, true);

        // Add each transaction as an hbox row
        int start = (*historyPage) * 7;
        int end = std::min(start + 7, static_cast<int>(c->session.user.transactions.size()));
        for (int i = start; i < end; ++i) {
            const Transaction& t = c->session.user.transactions[i];
            std::vector<std::string> row;
            to_string(t, row); // assuming this fills `row` with 4 strings

            table_rows.push_back(
            hbox({
                text(row[0]) | size(WIDTH,EQUAL,20)|size(HEIGHT,EQUAL,2),
                text(row[1]) | flex | color(row[1] == "Deposit"? Color::RGBA(0xff,0x80,0x00,0xff) :
                row[1] == "Withdraw" ? Color::RGBA(0xc4,0x5e,0xa7,0xff) :
                row[1] == "Transfer" ? Color::RGBA(0x97,0xcd,0xbe,0xff) :
                row[1] == "Receive" ? Color::RGBA(0x6f,0xc0,0xfa,0xff) : Color::White)| size(WIDTH,EQUAL,20)|size(HEIGHT,EQUAL,2),
                text(row[2]) | flex| color(row[1] == "Deposit" || row[1]=="Receive" ? Color::RGBA(0xaa,0xd9,0x4c,0xff) :  
                            Color::RGBA(0xee,0x11,0x11,0xff))|size(WIDTH,EQUAL,15)|size(HEIGHT,EQUAL,2),
                text(row[3]) | flex | align_right | size(HEIGHT,EQUAL,2)
            })
            );
        }

        // Final table as a vbox
        Element table = vbox(table_rows);

        std::ostringstream ss;
        
        ss << std::fixed << std::setprecision(2) << c->session.user.balance;

        return hbox(
            separatorEmpty(),
            vbox(
            hcenter((Logo()) | color(Color::Color::RGBA(0xFF,0,0,0xFF))),
            vcenter(text("Hello, " + c->session.user.username))|flex_grow,
            menu->Render(),
            filler(),
            logoutButton->Render()
            ),
            separatorEmpty(),   
            separator(),
            separatorEmpty(),
            window(
            text("   Your balance is "+ ss.str()+"   ") |color(Color::RGBA(0x00,0xff,0xaa,0xFF))| bold,
            vbox({
                separatorEmpty(),
                hcenter(
                *page == 0 ? depositLogo()  | color(LinearGradient().Angle(0).Stop(Color::RGBA(0xff,0x80,0x00,0xff)).Stop(Color::RGBA(0xff,0x00,0,0xff))) :
                *page == 1 ? withdrawLogo() | color(LinearGradient().Angle(0).Stop(Color::RGBA(0xc4,0x5e,0xa7,0xff)).Stop(Color::RGBA(0xa1,0xdf,0xff,0xff))) :      
                *page == 2 ? transferLogo() | color(LinearGradient().Angle(0).Stop(Color::RGBA(0x97,0xcd,0xbe,0xff)).Stop(Color::RGBA(0x6f,0xc0,0xfa,0xff))) :
                *page == 3 ? historyLogo()  | color(LinearGradient().Angle(0).Stop(Color::RGBA(0xff,0x80,0x00,0xff)).Stop(Color::RGBA(0xff,0x00,0,0xff))) :
                text("")
                )|size(HEIGHT,EQUAL,9),
                vbox({
                    *page == 3? table:text(""),
                    *page == 3? hcenter(actionsTab->Render()):actionsTab->Render(),
                    hcenter(*page ==3? text(std::to_string(*historyPage)) : text(*message))
                }),
            })
            )|flex_grow
        );}
    );
return renderer;
}