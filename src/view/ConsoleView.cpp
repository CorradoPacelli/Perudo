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
        
        displayMessage("Command was not recognized. Try again :)");
    }
}

void ConsoleView::waitForContinue() {
    //TODO: delete this function :(
    std::string dummy;
    std::getline(std::cin, dummy);
}

void ConsoleView::displayMessage(const std::string& message) {
    std::cout << message << std::endl;
}