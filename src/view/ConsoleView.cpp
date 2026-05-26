#include <iostream>

#include "ConsoleView.hpp"
#include "ActionInterpreter.hpp"
#include "IAction.hpp"

std::unique_ptr<IAction> ConsoleView::waitForAction() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        auto action = ActionInterpreter::interpret(input);
        if (action) {
            return action;
        }
        
        displayBroadcastMessage("Command was not recognized. Try again :)");
    }
}

void ConsoleView::waitForContinue() {
    //TODO: delete this function :(
    std::string dummy;
    std::getline(std::cin, dummy);
}

void ConsoleView::displayBroadcastMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleView::displayMessageToPlayer(int playerId, const std::string& message) {
    displayBroadcastMessage(message);
}