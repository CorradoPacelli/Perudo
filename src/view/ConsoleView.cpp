#include <iostream>

#include "ConsoleView.hpp"
#include "ActionInterpreter.hpp"
#include "IAction.hpp"
#include "PlayerMessage.hpp"


std::unique_ptr<IAction> ConsoleView::waitForAction() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // TODO: this 0 is really really wrong, but this is the console view, so player are requencial by design...
        auto action = ActionInterpreter::interpret(PlayerMessage(0,input));
        if (action) {
            return action;
        }
        
        displayBroadcastMessage("Command was not recognized. Try again :)");
    }
}

void ConsoleView::displayBroadcastMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleView::displayMessageToPlayer(const PlayerMessage& pMessage) {
    displayBroadcastMessage(pMessage.message);
}